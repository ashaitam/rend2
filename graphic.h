#ifndef GRAPHIC_H
#define GRAPHIC_H

enum Color
{
    RED,
    ORANGE,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    PURPLE,
    BLACK,
    GREY,
    WHITE,
};

void draw_circle(double x, double y, double radius, Color color);
void draw_square(double x, double y, double side, Color color);
void draw_arc(double x, double y, double radius, double angle_start, double angle_end, Color color);
void draw_square_outline(double x, double y, double side, Color color);


#endif
