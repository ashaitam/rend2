#include "graphic.h"

#ifndef TOOLS_H
#define TOOLS_H

constexpr double epsil_zero = 0.125;

struct Point {
    double x;
    double y;

    Point(double x_init = 0.0, double y_init = 0.0) 
        : x(x_init), y(y_init) {}
    
};

struct Circle {
    Point center;
    double radius;

    Circle(Point c_init = Point(), double r_init = 0.0)
        : center(c_init), radius(r_init) {}

    void draw(Color color) const;
};

struct Square {
    Point center;
    double side;

    Square(Point c_init = Point(), double s_init = 0.0)
        : center(c_init), side(s_init) {}

    void draw(Color color) const;
};

// Prototypes des fonctions

double norm(const Point& p);

double distance(const Point& p1, const Point& p2);

bool intersects(const Square& s1, const Square& s2, double epsilon);

bool intersects(const Circle& c1, const Circle& c2, double epsilon);

bool intersects(const Circle& c, const Square& s, double epsilon);
bool intersects(const Square& s, const Circle& c, double epsilon);


double dot_product(const Point& u, const Point& v);

#endif
