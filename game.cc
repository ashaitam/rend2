#include "game.h"
#include <cmath>

bool read_data(std::ifstream& file, std::string& line)
{
    while (std::getline(file, line))
    {
        std::size_t begin(0);
        while (begin < line.size()
               && (line[begin] == ' '
                   || line[begin] == '\t'
                   || line[begin] == '\r'))
        {
            ++begin;
        }

        if (begin == line.size())
        {
            continue;
        }

        if (line[begin] == '#')
        {
            continue;
        }

        std::size_t end(line.size());
        while (end > begin
               && (line[end - 1] == ' '
                   || line[end - 1] == '\t'
                   || line[end - 1] == '\r'))
        {
            --end;
        }

        line = line.substr(begin, end - begin);
        return true;
    }

    return false;
}

Game::Game(int score_init, int lives_init)
    : score(score_init), lives(lives_init), status(ON_GOING), paddle(nullptr) {}

Game::Game() 
    : score(0), lives(0), status(ON_GOING), paddle(nullptr), bricks(), balls() {}


bool Game::add_brick(std::unique_ptr<Brick> b)
{   
    // Verification de validite avant insertion.
    if (b == nullptr || !b->is_valid())
    {
        return false;
    }

    // Refus en cas de collision avec une brique existante.
    for (size_t i = 0; i < bricks.size(); ++i)
    {
        if (intersects(b->get_square(), bricks[i]->get_square(), 0.0))
        {
            std::cout << message::collision_bricks(bricks.size(), i);
            return false;
        }
    }

    // Refus en cas de collision avec la raquette.
    if (paddle != nullptr &&
        intersects(b->get_square(), paddle->get_circle(), 0.0))
    {
        std::cout << message::collision_paddle_brick(bricks.size());
        return false;
    }

    bricks.push_back(std::move(b));
    return true;
}

bool Game::add_ball(const Ball& b)
{
    // Verification de validite avant insertion.
    if(!b.is_valid())
    {
        return false;
    }

    // Refus en cas de collision avec une autre balle.
    for (size_t i = 0; i < balls.size(); ++i)
    {
        if (intersects(b.get_circle(), balls[i].get_circle(), 0.0))
        {
            std::cout << message::collision_balls(balls.size(), i);
            return false;
        }
    }

    // Collision avec la raquette.
    if (paddle != nullptr &&
        intersects(b.get_circle(), paddle->get_circle(), 0.0))
    {
        std::cout << message::collision_paddle_ball(balls.size());
        return false;
    }

    // Collision avec les briques.
    for (size_t i = 0; i < bricks.size(); ++i)
    {
        if (intersects(b.get_circle(), bricks[i]->get_square(), 0.0))
        {
            std::cout << message::collision_ball_brick(balls.size(), i);
            return false;
        }
    }

    balls.push_back(b);
    return true;
}

bool Game::set_paddle(const Paddle& p)
{
    // On ne remplace la raquette que si la nouvelle est valide.
    if (!p.is_valid())
    {
        return false;
    }

    // Collision avec les briques.
    for (size_t i = 0; i < bricks.size(); ++i)
    {
        if(intersects(p.get_circle(), bricks[i]->get_square(), 0.0))
        {
            std::cout << message::collision_paddle_brick(i);
            return false;
        }
    }

    // Collision avec les balles.
    for (size_t i = 0; i < balls.size(); ++i)
    {
        if(intersects(balls[i].get_circle(), p.get_circle(), 0.0))
    {
        std::cout << message::collision_paddle_ball(i);
        return false;
    }
    }

    paddle = std::make_unique<Paddle>(p);
    return true;
}

int Game::get_score() const
{
    return score;
}

int Game::get_lives() const
{
    return lives;
}

Status Game::get_status() const
{
    return status;
}

std::size_t Game::get_nb_bricks() const
{
    return bricks.size();
}

std::size_t Game::get_nb_balls() const
{
    return balls.size();
}

const Paddle* Game::get_paddle() const
{
    return paddle.get();
}

const std::vector<std::unique_ptr<Brick>>& Game::get_bricks() const
{
    return bricks;
}

const std::vector<Ball>& Game::get_balls() const
{
    return balls;
}

void Game::clear()
{
    score = 0;
    lives = 0;
    paddle.reset();
    balls.clear();
    bricks.clear();
}

bool Game::load(const std::string& file_name)
{
    clear();
    std::ifstream file(file_name);

    if(!file)
    {
        return false;
    }
    
    std::string line;
    std::istringstream iss;

    int score_read(0);
    int lives_read(0);
    
    if (!read_data(file,line))
    {
        clear();
        return false;
    }

    iss.clear();
    iss.str(line);
    if (!(iss >> score_read))
    {
        clear();
        return false;
    }

    if  (score_read < 0)
    {
        std::cout << message::invalid_score(score_read);
        clear();
        return false;
    }
    if (!read_data(file, line))
    {
        return false;
    }

    iss.clear();
    iss.str(line);
    if (!(iss >> lives_read))
    {
        clear();
        return false;
    }

    if (lives_read < 0)
    {
        std::cout << message::invalid_lives(lives_read);
        clear();
        return false;
    }

    score = score_read;
    lives = lives_read;

    double x(0.0);
    double y(0.0);
    double r(0.0);

    if (!read_data(file,line))
    {   
        clear();
        return false;
    }
    
    iss.clear();
    iss.str(line);
    if (!(iss >> x >> y >> r))
    {
        clear();
        return false;
    }

    Point center(x,y);
    Circle circle(center,r);
    Paddle paddle(circle);

    if (!set_paddle(paddle))
    {
        clear();
        return false;
    }

    int nb_bricks(0);

    if (!read_data(file, line))
    {
        clear();
        return false;
    }

    iss.clear();
    iss.str(line);
    if (!(iss >> nb_bricks) || nb_bricks < 0)
    {
        clear();
        return false;
    }

    for (int i = 0; i < nb_bricks; ++i)
    {   
        int t(0);
        double sx(0.0);
        double sy(0.0);
        double c(0.0);

         if (!read_data(file, line))
        {
            clear();
            return false;
        }

        iss.clear();
        iss.str(line);
        if (!(iss >> t >> sx >> sy >> c))
        {
            clear();
            return false;
        }

        Point center(sx,sy);
        Square square(center,c);

        std::unique_ptr<Brick> brick;

        if (t == 0)
        {
            int h(0);
            if (!(iss >> h))
            {
                clear();
                return false;
            }
            brick = std::make_unique<RainbowBrick>(square, h);
         }

        else if (t == 1) 
        {
            brick = std::make_unique<BallBrick>(square);

        }
        
        else if (t == 2)
        {
            brick = std::make_unique<SplitBrick>(square);
        }

        else
        {
            std::cout << message::invalid_brick_type(t);
            clear();
            return false;
        }

        if (!add_brick(std::move(brick)))
        {
            clear();
            return false;
        }

    }

     int nb_balls(0);

    if (!read_data(file, line))
    {
        clear();
        return false;
    }

    iss.clear();
    iss.str(line);
    if (!(iss >> nb_balls) || nb_balls < 0)
    {
        clear();
        return false;
    }

    for (int i = 0; i < nb_balls; ++i)
    {   
        double x(0.0);
        double y(0.0);
        double r(0.0);
        double dx(0.0);
        double dy(0.0);

        if (!read_data(file, line))
        {
            clear();
            return false;
        }
        iss.clear();
        iss.str(line);
        if (!(iss >> x >> y >> r >> dx >> dy))
        {
            clear();
            return false;
        }

        Point center(x,y);
        Circle circle(center, r);
        Point delta(dx,dy);

        Ball ball(circle,delta);

        if (!add_ball(ball))
        {
            clear();
            return false;
        }
    }

    update_status();
    std::cout << message::success();
    return true;

}

bool Game::save(const std::string& file_name) const
{
    std::ofstream file(file_name);

    if (!file)
    {
        return false;
    }

    if (paddle == nullptr)
    {
        return false;
    }

    file <<"# score" << '\n';
    file << score << '\n';
    file <<"# lives" << '\n';
    file << lives << '\n';


    Circle circle = paddle->get_circle();

    file <<"# paddle"<< '\n';
    file << circle.center.x << " " << circle.center.y << " " << circle.radius << '\n';

    file <<"# bricks"<< '\n';
    file << bricks.size() << '\n';

    for(size_t i = 0; i < bricks.size(); ++i)
    {   
        Square square = bricks[i]->get_square();
        int t = bricks[i]->get_type();
        if (t == 0)
        {
            const RainbowBrick* rainbow_brick = dynamic_cast<const RainbowBrick*>(bricks[i].get());

            if (rainbow_brick == nullptr)
            {
                return false;
            }

            int h = rainbow_brick->get_hit_points();
            file << t << " " << square.center.x << " " << square.center.y << " " << square.side << " " << h << '\n';
        }

        else 
        {
            file << t << " " << square.center.x << " " << square.center.y << " " << square.side << '\n';
        }
    }
     

    file <<"# balls"<< '\n';
    file << balls.size() << '\n';

    for(size_t i = 0; i < balls.size(); ++i)
    {
        Circle circle = balls[i].get_circle();
        Point delta = balls[i].get_delta();
        file << circle.center.x << " " << circle.center.y << " " << circle.radius << " " << delta.x << " " << delta.y << '\n';
    }

    return true;

}

void Game::draw() const
{
    draw_square_outline(arena_size/2.0, arena_size/2.0, arena_size, GREY);

    if(paddle != nullptr)
    {
        paddle->draw();
    }

    for(size_t i = 0; i < bricks.size(); ++i)
    {
        bricks[i]->draw();
    }

     for(size_t i = 0; i < balls.size(); ++i)
    {
        balls[i].draw();
    }

}

void Game::update_paddle_pos(double new_x)
{
    if (paddle == nullptr)
    {
        return;
    }

    Circle current = paddle->get_circle();
    double dx = new_x - current.center.x;

    if (std::abs(dx) > delta_norm_max)
    {
        dx = (dx > 0.0) ? delta_norm_max : -delta_norm_max;
    }

    Circle next_circle(Point(current.center.x + dx, current.center.y),
                       current.radius);
    double discriminant = next_circle.radius * next_circle.radius
                          - next_circle.center.y * next_circle.center.y;

    if (discriminant < -epsil_zero)
    {
        return;
    }
    if (discriminant < 0.0)
    {
        discriminant = 0.0;
    }

    double delta_x = std::sqrt(discriminant);
    if (next_circle.center.y > 0 ||
        next_circle.center.y + next_circle.radius <= 0 ||
        next_circle.center.x - delta_x < 0 ||
        next_circle.center.x + delta_x > arena_size)

    {
        return;
    }

    Paddle moved(next_circle);

    for (size_t i = 0; i < bricks.size(); ++i)
    {
        if (intersects(moved.get_circle(),
                                  bricks[i]->get_square(), epsil_zero))
        {
            return;
        }
    }

    paddle = std::make_unique<Paddle>(moved);
}

void Game::update_status()
{
    if(status == LOST || status == WON)
    {
        return;
    }

    else if(balls.size() == 0 && lives == 0)
    {
        status = LOST;
    }

    else if(bricks.size() == 0)
    {
        score += lives*score_per_life;
        status = WON;
    }
}

void Game::update()
{   
    if(status != ON_GOING)
    {
        return;
    }

    size_t i(0);
    while(i < balls.size())
    {   
        balls[i].move();

        if(balls[i].get_circle().center.y < -epsil_zero)
        {
            balls.erase(balls.begin() + i);
        }
        else
        {
            unsigned int nb_bounce = 0;
            bool impact = true;
            bool paddle_bounced = false;

            while(impact && nb_bounce < nb_bounce_max)
            {
                impact = false;

                Circle circle = balls[i].get_circle();

                bool left_impact = circle.center.x - circle.radius < epsil_zero;
                bool right_impact = circle.center.x + circle.radius > arena_size - epsil_zero;
                bool top_impact = circle.center.y + circle.radius > arena_size - epsil_zero;

                if ((left_impact || right_impact) && top_impact)
                {
                    double overflow_x;
                    double overflow_y = (circle.center.y + circle.radius)
                                        - (arena_size - epsil_zero);

                    Point new_delta = balls[i].get_delta();

                    if (left_impact)
                    {
                        overflow_x = epsil_zero - (circle.center.x - circle.radius);
                    }
                    else
                    {
                        overflow_x = (circle.center.x + circle.radius) - (arena_size - epsil_zero);
                    }

                    if (overflow_x >= overflow_y)
                    {
                        new_delta.x = -new_delta.x;
                    }
                    else
                    {   
                        new_delta.y = -new_delta.y;
                    }

                    balls[i].reverse_move();
                    balls[i].set_delta(new_delta);
                    balls[i].move();

                    impact = true;
                    nb_bounce++;
                }

                else if (left_impact || right_impact) 
                {
                    Point new_delta = balls[i].get_delta();
                    new_delta.x = -new_delta.x;

                    balls[i].reverse_move();
                    balls[i].set_delta(new_delta);
                    balls[i].move();

                    impact = true;
                    nb_bounce++;
                }

                else if (top_impact)
                {
                    Point new_delta = balls[i].get_delta();
                    new_delta.y = -new_delta.y;

                    balls[i].reverse_move();
                    balls[i].set_delta(new_delta);
                    balls[i].move();

                    impact = true;
                    nb_bounce++;
                }

                else
                {
                    bool brick_impact = false;
                    size_t brick_index = 0;

                    for (size_t j(0); j < bricks.size(); j++)
                    {
                        if (intersects(balls[i].get_circle(), bricks[j]->get_square(), epsil_zero))
                        {
                            brick_impact = true;
                            brick_index = j;
                            break;
                        }
                    }

                    if (brick_impact)
                    {
                        score += score_per_hit;

                        Circle ball_circle = balls[i].get_circle();
                        Square brick_square = bricks[brick_index]->get_square();

                        Point d;
                        d.x = ball_circle.center.x - brick_square.center.x;
                        d.y = ball_circle.center.y - brick_square.center.y;

                        double half_side = brick_square.side / 2.0;

                        Point db;
                        db.x = d.x;
                        db.y = d.y;

                        if (db.x > half_side)
                        {
                            db.x = half_side;
                        }
                        else if (db.x < -half_side)
                        {
                            db.x = -half_side;
                        }

                        if (db.y > half_side)
                        {
                            db.y = half_side;
                        }
                        else if (db.y < -half_side)
                        {
                            db.y = -half_side;
                        }

                        Point dn;
                        dn.x = d.x - db.x;
                        dn.y = d.y - db.y;

                        Point delta = balls[i].get_delta();
                        Point new_delta = delta;
                        double dn_norm = std::sqrt(dn.x * dn.x + dn.y * dn.y);

                        if (dn_norm < epsil_zero)
                        {
                            if (std::abs(d.x) >= std::abs(d.y))
                            {
                                new_delta.x = -new_delta.x;
                            }
                            else
                            {
                                new_delta.y = -new_delta.y;
                            }
                        }

                        else
                        {
                            Point delta_normal;
                            delta_normal.x = (dot_product(dn, delta) / (dn_norm * dn_norm)) * dn.x;
                            delta_normal.y = (dot_product(dn, delta) / (dn_norm * dn_norm)) * dn.y;

                            new_delta.x = delta.x - 2.0 * delta_normal.x;
                            new_delta.y = delta.y - 2.0 * delta_normal.y;
                        }
                        
                        double new_delta_norm = norm(new_delta);

                        if (new_delta_norm > delta_norm_max)
                        {
                            new_delta.x = new_delta.x * delta_norm_max / new_delta_norm;
                            new_delta.y = new_delta.y * delta_norm_max / new_delta_norm;
                        }

                        balls[i].reverse_move();
                        balls[i].set_delta(new_delta);
                        balls[i].move();

                        int brick_type = bricks[brick_index]->get_type();

                        if (brick_type == 0)
                        {   
                            RainbowBrick* rainbow = dynamic_cast<RainbowBrick*>(bricks[brick_index].get());

                            if (rainbow != nullptr)
                            {
                                rainbow->hit();

                                if (rainbow->get_hit_points() == 0)
                                {
                                    bricks.erase(bricks.begin() + brick_index);
                                }
                            }
                        }

                        else if (brick_type == 1)
                        {
                            bricks.erase(bricks.begin() + brick_index);

                            Circle new_ball_circle(brick_square.center,
                                                   new_ball_radius);
                            Ball new_ball(new_ball_circle, delta);

                            balls.push_back(new_ball);
                        }

                        else if (brick_type == 2)
                        {
                            double child_side =
                                (brick_square.side - split_brick_gap) / 2.0;

                            bricks.erase(bricks.begin() + brick_index);

                            if (child_side >= brick_size_min)
                            {
                                double offset = child_side / 2.0 + split_brick_gap / 2.0;

                                Square top_left(
                                    Point(brick_square.center.x - offset, brick_square.center.y + offset), child_side);

                                Square top_right(
                                    Point(brick_square.center.x + offset, brick_square.center.y + offset), child_side);

                                Square bottom_left(
                                    Point(brick_square.center.x - offset, brick_square.center.y - offset), child_side);

                                Square bottom_right(
                                    Point(brick_square.center.x + offset, brick_square.center.y - offset), child_side);

                                bricks.push_back(std::make_unique<SplitBrick>(top_left));
                                bricks.push_back(std::make_unique<SplitBrick>(top_right));
                                bricks.push_back(std::make_unique<SplitBrick>(bottom_left));
                                bricks.push_back(std::make_unique<SplitBrick>(bottom_right));
                            }
                        }

                        impact = true;
                        nb_bounce++;
                    }
                }

                if (!paddle_bounced && paddle != nullptr && intersects(balls[i].get_circle(), paddle->get_circle(), epsil_zero))
                {
                    Point paddle_circle = paddle->get_circle().center;
                    Point ball_circle = balls[i].get_circle().center;
                    Point ball_delta = balls[i].get_delta();
                    Point paddle_delta(std::sqrt(3),std::sqrt(3));

                    Point dn;
                    dn.x = ball_circle.x - paddle_circle.x;
                    dn.y = ball_circle.y - paddle_circle.y;
                    double dn_norm = std::sqrt(dn.x * dn.x + dn.y * dn.y);

                    if (dn_norm < epsil_zero)
                    {
                        Point escape_delta = balls[i].get_delta();
                        escape_delta.y = std::abs(escape_delta.y);

                        balls[i].reverse_move();
                        balls[i].set_delta(escape_delta);
                        balls[i].move();

                        impact = true;
                        paddle_bounced = true;
                    }
                    else
                    {
                        Point v_ball_n;
                        v_ball_n.x = (dot_product(dn, ball_delta) / (dn_norm * dn_norm)) * dn.x;
                        v_ball_n.y = (dot_product(dn, ball_delta) / (dn_norm * dn_norm)) * dn.y;

                        Point v_paddle_n;
                        v_paddle_n.x = (dot_product(dn, paddle_delta) / (dn_norm * dn_norm)) * dn.x;
                        v_paddle_n.y = (dot_product(dn, paddle_delta) / (dn_norm * dn_norm)) * dn.y;

                        Point new_delta;
                        new_delta.x = ball_delta.x - 2.0 * v_ball_n.x + 2.0 * v_paddle_n.x;
                        new_delta.y = ball_delta.y - 2.0 * v_ball_n.y + 2.0 * v_paddle_n.y;


                        double new_delta_norm = norm(new_delta);

                        if (new_delta_norm > delta_norm_max)
                        {
                            new_delta.x = new_delta.x * delta_norm_max / new_delta_norm;
                            new_delta.y = new_delta.y * delta_norm_max / new_delta_norm;
                        }

                        balls[i].reverse_move();
                        balls[i].set_delta(new_delta);
                        balls[i].move();

                        impact = true;
                        paddle_bounced = true;
                    }
                }
            }

            i++;
        }
    }
    
    update_status();
}