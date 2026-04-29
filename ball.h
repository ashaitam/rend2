#ifndef BALL_H
#define BALL_H

#include "tools.h"
#include "message.h"
#include "constants.h"


class Ball {
    public : 

    Ball(const Circle& circle_init, const Point& delta_init);

    Circle get_circle() const;
    Point get_delta() const;
    bool is_valid() const;
    private : 
    Circle circle;
    Point delta;
};

#endif
