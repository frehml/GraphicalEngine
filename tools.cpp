//
// Created by frehml on 01/05/2021.
//

#include "tools.h"

img::Color make_color(ini::DoubleTuple color_tuple){
    uint8_t red = color_tuple[0]*255;
    uint8_t green = color_tuple[1]*255;
    uint8_t blue = color_tuple[2]*255;
    img::Color color(red, green, blue);
    return color;
}