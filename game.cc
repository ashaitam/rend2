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
    : score(score_init), lives(lives_init), paddle(nullptr) {}

Game::Game() 
    : score(0), lives(0), paddle(nullptr), bricks(), balls() {}


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
        if (intersects(b->get_square(), bricks[i]->get_square()))
        {
            std::cout << message::collision_bricks(bricks.size(), i);
            return false;
        }
    }

    // Refus en cas de collision avec la raquette.
    if (paddle != nullptr &&
        intersects(b->get_square(), paddle->get_circle()))
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
        if (intersects(b.get_circle(), balls[i].get_circle()))
        {
            std::cout << message::collision_balls(balls.size(), i);
            return false;
        }
    }

    // Collision avec la raquette.
    if (paddle != nullptr &&
        intersects(b.get_circle(), paddle->get_circle()))
    {
        std::cout << message::collision_paddle_ball(balls.size());
        return false;
    }

    // Collision avec les briques.
    for (size_t i = 0; i < bricks.size(); ++i)
    {
        if (intersects(b.get_circle(), bricks[i]->get_square()))
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
        if(intersects(p.get_circle(), bricks[i]->get_square()))
        {
            std::cout << message::collision_paddle_brick(i);
            return false;
        }
    }

    // Collision avec les balles.
    for (size_t i = 0; i < balls.size(); ++i)
    {
        if(intersects(balls[i].get_circle(), p.get_circle()))
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
        if (intersects_with_epsil(moved.get_circle(),
                                  bricks[i]->get_square()))
        {
            return;
        }
    }

    paddle = std::make_unique<Paddle>(moved);
}
