//
// Created by frehml on 01/05/2021.
//


#include "tools.h"

void make_color(img::Color &color, ini::DoubleTuple color_tuple){
    color.red = (uint8_t) color_tuple[0]*255;
    color.green = (uint8_t) color_tuple[1]*255;
    color.blue = (uint8_t) color_tuple[2]*255;
}

int roundToInt(double d) {
    return static_cast<int>(std::round(d));
}

double degToRad(double d){
    return d *M_PI/180;
}