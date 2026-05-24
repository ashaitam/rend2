#include "tools.h"

#include <cmath>

double norm(const Point& p) {
    return std::sqrt(p.x * p.x + p.y * p.y);
}

double distance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool intersects(const Square& s1, const Square& s2, double epsilon) {
    bool overlap_x =
        std::abs(s1.center.x - s2.center.x) - (s1.side + s2.side) / 2.0 < epsilon;
    bool overlap_y =
        std::abs(s1.center.y - s2.center.y) - (s1.side + s2.side) / 2.0 < epsilon;

    return overlap_x && overlap_y;
}

bool intersects(const Circle& c1, const Circle& c2, double epsilon) {
    return distance(c1.center, c2.center) - (c1.radius + c2.radius) < epsilon ;
}

bool intersects(const Circle& c, const Square& s, double epsilon) {
    double square_left = s.center.x - (s.side / 2.0);
    double square_right = s.center.x + (s.side / 2.0);
    double square_top = s.center.y + (s.side / 2.0);
    double square_bottom = s.center.y - (s.side / 2.0);

    double closest_px;
    double closest_py;

    if (c.center.x < square_left) {
        closest_px = square_left;
    } else if (c.center.x > square_right) {
        closest_px = square_right;
    } else {
        closest_px = c.center.x;
    }

    if (c.center.y < square_bottom) {
        closest_py = square_bottom;
    } else if (c.center.y > square_top) {
        closest_py = square_top;
    } else {
        closest_py = c.center.y;
    }

    Point closest_p(closest_px, closest_py);
    return distance(c.center, closest_p) - c.radius < epsilon;
}

bool intersects(const Square& s, const Circle& c, double epsilon) {
    return intersects(c, s, epsilon);
}

void Square::draw(Color color) const
{
    draw_square(center.x, center.y, side, color);
}

void Circle::draw(Color color) const
{
    draw_circle(center.x, center.y, radius, color);
}

double dot_product(const Point& u, const Point& v)
{
    return u.x*v.x + u.y*v.y;
}

Point projection(const Point& u, const Point& v)
{
    double v_norm_square = dot_product(v, v);

    if (v_norm_square < epsil_zero * epsil_zero)
    {
        return Point(0.0, 0.0);
    }

    double factor = dot_product(u, v) / v_norm_square;

    return Point(factor * v.x, factor * v.y);
}