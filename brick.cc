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

void RainbowBrick::hit()
{
    hit_points -= 1;
}

void BallBrick::draw() const
{
    square.draw(RED);

    Circle new_ball_circle(square.center, new_ball_radius);
    new_ball_circle.draw(BLACK);
}

namespace
{
Color split_color(int layer)
{
    switch (layer)
    {
    case 1:
        return RED;
    case 2:
        return ORANGE;
    case 3:
        return YELLOW;
    case 4:
        return GREEN;
    case 5:
        return CYAN;
    case 6:
        return BLUE;
    case 7:
        return PURPLE;
    default:
        return PURPLE;
    }
}

void draw_split_layers(const Square& square, int layer)
{
    square.draw(split_color(layer));

    double child_side = (square.side - split_brick_gap) / 2.0;
    if (child_side < brick_size_min)
    {
        return;
    }

    double offset = child_side / 2.0 + split_brick_gap / 2.0;

    Square top_left(
        Point(square.center.x - offset, square.center.y + offset),
        child_side);
    Square top_right(
        Point(square.center.x + offset, square.center.y + offset),
        child_side);
    Square bottom_left(
        Point(square.center.x - offset, square.center.y - offset),
        child_side);
    Square bottom_right(
        Point(square.center.x + offset, square.center.y - offset),
        child_side);

    draw_split_layers(top_left, layer + 1);
    draw_split_layers(top_right, layer + 1);
    draw_split_layers(bottom_left, layer + 1);
    draw_split_layers(bottom_right, layer + 1);
}
}

void SplitBrick::draw() const
{
    draw_split_layers(square, 1); // A changer pour le rendu 3
}
