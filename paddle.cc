#include "paddle.h"
#include "constants.h"
#include "message.h"
#include <iostream>
#include <cmath>

Paddle::Paddle(const Circle& circle_init) 
    : circle(circle_init) {}

Circle Paddle::get_circle() const {
    return circle;
}

bool Paddle::is_valid() const
{
    // Equation d'un cercle: (x - x_c)^2 + (y - y_c)^2 = r^2.
    // Pour y = 0 dans le repere du centre, l'ecart horizontal vaut sqrt(r^2 - y_c^2).
    // On doit verifier que le discriminant (r^2 - y_c^2) >= 0 avant de calculer sqrt
    const double discriminant = circle.radius * circle.radius
                                - circle.center.y * circle.center.y;
    
    // Si le discriminant est negatif, la raquette ne peut pas atteindre y = 0
    if (discriminant < 0) {
        std::cout << message::paddle_outside(circle.center.x, circle.center.y);
        return false;
    }
    
    const double delta_x = std::sqrt(discriminant);

    if (circle.center.y > 0 ||
        circle.center.y + circle.radius <= 0 ||
        circle.center.x - delta_x < 0 ||
        circle.center.x + delta_x > arena_size)
    {
        std::cout << message::paddle_outside(circle.center.x, circle.center.y);
        return false;
    }

    return true;
}

