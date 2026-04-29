#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <memory>


#include "ball.h"
#include "brick.h"
#include "game.h"
#include "message.h"
#include "paddle.h"
#include "tools.h"


bool read_data(std::ifstream& file, std::string& line)
{
    while (std::getline(file, line))
    {
        if (!line.empty() && line[0] != '#')
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    std::ifstream file(argv[1]);

    if (!file)
    {
        return 1;
    }

    std::string line;
    std::istringstream iss;

    int score;
    int lives;
    
    if (!read_data(file,line))
    {
        return 1;
    }

    iss.clear();
    iss.str(line);
    iss >> score;

    if  (score < 0)
    {
        std::cout << message::invalid_score(score);
        return 1;
    }
    if (!read_data(file, line))
    {
        return 1;
    }

    iss.clear();
    iss.str(line);
    iss >> lives;

    if (lives < 0)
    {
        std::cout << message::invalid_lives(lives);
        return 1;
    }

    Game game(score,lives);

    double x;
    double y;
    double r;

    if (!read_data(file,line))
    {
        return 1;
    }
    
    iss.clear();
    iss.str(line);
    iss >> x >> y >> r;

    Point center(x,y);
    Circle circle(center,r);
    Paddle paddle(circle);

    if (!game.set_paddle(paddle))
    {
        return 1;
    }

    int nb_bricks;

    if (!read_data(file, line))
    {
        return 1;
    }

    iss.clear();
    iss.str(line);
    iss >> nb_bricks;

    for (int i = 0; i < nb_bricks; ++i)
    {   
        int t;
        double sx;
        double sy;
        double c;

         if (!read_data(file, line))
        {
            return 1;
        }

        iss.clear();
        iss.str(line);
        iss >> t >> sx >> sy >> c;

        Point center(sx,sy);
        Square square(center,c);

        std::unique_ptr<Brick> brick;

        if (t == 0)
        {
            int h;
            iss >> h;
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
            return 1;
        }

        if (!game.add_brick(std::move(brick)))
        {
            return 1;
        }

    }

     int nb_balls;

    if (!read_data(file, line))
    {
        return 1;
    }

    iss.clear();
    iss.str(line);
    iss >> nb_balls;

    for (int i = 0; i < nb_balls; ++i)
    {   
        double x;
        double y;
        double r;
        double dx;
        double dy;

        if (!read_data(file, line))
        {
            return 1;
        }
        iss.clear();
        iss.str(line);
        iss >> x >> y >> r >> dx >> dy;

        Point center(x,y);
        Circle circle(center, r);
        Point delta(dx,dy);

        Ball ball(circle,delta);

        if (!game.add_ball(ball))
        {
            return 1;
        }
    }

    std::cout << message::success();
    return 0;
}
