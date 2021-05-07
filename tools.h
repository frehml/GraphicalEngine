//
// Created by frehml on 01/05/2021.
//

#ifndef ENGINE_TOOLS_H
#define ENGINE_TOOLS_H

#include <cmath>
#include "easy_image.h"
#include "ini_configuration.h"
#include "vector3d.h"

void make_color(img::Color &color, ini::DoubleTuple color_tuple);
int roundToInt(double d);
double degToRad(double d);
void make_vector(Vector3D &vec, ini::DoubleTuple vector);

#endif //ENGINE_TOOLS_H
