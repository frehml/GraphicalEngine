//
// Created by frehml on 01/05/2021.
//

#ifndef ENGINE_TOOLS_H
#define ENGINE_TOOLS_H

#include <cmath>
#include "easy_image.h"
#include "ini_configuration.h"


void make_color(img::Color &color, ini::DoubleTuple color_tuple);
int roundToInt(double d);
double degToRad(double d);

#endif //ENGINE_TOOLS_H
