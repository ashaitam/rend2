#include <cmath>
#include "graphic.h"
#include "graphic_gui.h"

using namespace std;

static const Cairo::RefPtr<Cairo::Context> *ptcr(nullptr);
constexpr double pi = 3.14159265358979323846;

// graphic_gui.h
void graphic_set_context(const Cairo::RefPtr<Cairo::Context> &cr)
{
    ptcr = &cr;
}
// local functions
void set_color(Color color)
{
    double r(0.), g(0.), b(0.);

    switch (color)
    {
    case RED:
        r = 1.0;
        break;
    case ORANGE:
        r = 1.0;
        g = 0.5;
        break;
    case YELLOW:
        r = 1.0;
        g = 1.0;
        break;
    case GREEN:
        g = 1.0;
        break;
    case CYAN:
        g = 1.0;
        b = 1.0;
        break;
    case BLUE:
        b = 1.0;
        break;
    case PURPLE:
        r = 0.5;
        b = 1.0;
        break;
    case BLACK:
        r = g = b = 0.0;
        break;
    case GREY:
        r = g = b = 0.5;
        break;
    case WHITE:
        r = g = b = 1.0;
        break;
    default:
        break;
    }
    (*ptcr)->set_source_rgb(r, g, b);
}

void draw_circle(double x, double y, double radius, Color color)
{
    set_color(color);
    (*ptcr)->arc(x, y, radius, 0, 2 * pi);
    (*ptcr)->fill();
}

void draw_square(double x, double y, double side, Color color)
{
    set_color(color);
    (*ptcr)->rectangle(x - side/2, y - side/2, side, side);
    (*ptcr)->fill();
}

void draw_arc(double x, double y, double radius, double angle_start, double angle_end, Color color)
{
    set_color(color);
    (*ptcr)->arc(x, y, radius, angle_start, angle_end);
    (*ptcr)->stroke();
}

void draw_square_outline(double x, double y, double side, Color color)
{
    set_color(color);
    (*ptcr)->set_line_width(0.7);
    (*ptcr)->rectangle(x - side/2, y - side/2, side, side);
    (*ptcr)->stroke();
}
