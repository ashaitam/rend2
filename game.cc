#include "game.h"

Game::Game(int score_init, int lives_init)
    : score(score_init), lives(lives_init), paddle(nullptr) {}

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
