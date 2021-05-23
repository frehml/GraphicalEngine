//
// Created by frehml on 01/05/2021.
//


#include "tools.h"

void make_color(img::Color &color, ini::DoubleTuple color_tuple){
    color.red =  color_tuple[0]*255;
    color.green =  color_tuple[1]*255;
    color.blue =  color_tuple[2]*255;
}

int roundToInt(double d) {
    return static_cast<int>(std::round(d));
}

double degToRad(double d){
    return d *M_PI/180;
}

void make_vector(Vector3D &vec, ini::DoubleTuple vector){
    vec.x = vector[0];
    vec.y = vector[1];
    vec.z = vector[2];
}