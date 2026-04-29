#include "brick.h"
#include <iostream>


Brick::Brick(const Square& square_init, int type_init)
    : square(square_init), type(type_init) {}

Square Brick::get_square() const{
    return square;
}

int Brick::get_type() const{
    return type;
}

bool Brick::is_valid() const{

    if(square.side < brick_size_min){
        std::cout<<message::invalid_brick_size(square.side);
        return false;
    }

    if(type != 0 && type != 1 && type != 2 ){
        std::cout<<message::invalid_brick_type(type);
        return false;
    }

    double half_side = square.side / 2.0;

    if (square.center.x - half_side < 0 ||
        square.center.x + half_side > arena_size ||
        square.center.y - half_side < 0 ||
        square.center.y + half_side > arena_size) {
        
        std::cout << message::brick_outside(square.center.x, square.center.y);
        return false;
    }

    return true;
    
}



RainbowBrick::RainbowBrick(const Square& square_init, int hit_points_init)
    : Brick(square_init, 0), hit_points(hit_points_init) {}

int RainbowBrick::get_hit_points() const{
    return hit_points;
}
bool RainbowBrick::is_valid() const{
    if(!Brick::is_valid()){
        return false;
    }
    if(hit_points < 1 || hit_points > 7){
        std::cout<< message::invalid_hit_points(hit_points);
        return false;
    }

    return true;
}


BallBrick::BallBrick(const Square& square_init) 
    : Brick(square_init, 1) {}



SplitBrick::SplitBrick(const Square& square_init) 
    : Brick(square_init, 2) {}


void RainbowBrick::draw() const
{
    Color color = BLACK;
    switch(hit_points)
    {
        case 1 :
            color = RED;
            break;

        case 2 :
            color = ORANGE;
            break;
        
        case 3 : 
            color = YELLOW;
            break;
        
        case 4 : 
            color = GREEN;
            break;
        
        case 5 : 
            color = CYAN;
            break;

        case 6 : 
            color = BLUE;
            break;
        
        case 7 : 
            color = PURPLE;
            break;
        
        default : 
            break;
    }

    square.draw(color);
}

void BallBrick::draw() const
{
    square.draw(RED);

    Circle new_ball_cirlce(square.center, new_ball_radius);
    new_ball_circle.draw(BLACK);
}

void SplitBrick::draw() const
{
    // TO DO 
}
