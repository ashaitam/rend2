#include "ball.h"
#include <iostream>

Ball::Ball(const Circle& circle_init, const Point& delta_init) 
    : circle(circle_init), delta(delta_init) {}

Circle Ball::get_circle() const {
    return circle;
}

Point Ball::get_delta() const {
    return delta;
}

bool Ball::is_valid() const {
    if( circle.center.x - circle.radius < 0 ||
        circle.center.x + circle.radius > arena_size ||
        circle.center.y < 0 ||   //selon l'enonce, on ignore le rayon pour le bord inferieur
        circle.center.y + circle.radius > arena_size)
        {
            std::cout<<message::ball_outside(circle.center.x, circle.center.y);
            return false;
        }

    if(norm(delta) > delta_norm_max){
        std::cout<<message::invalid_delta(delta.x, delta.y);
        return false;
    }

    return true;
}

void Ball::draw() const
{
    circle.draw(BLACK);
}
