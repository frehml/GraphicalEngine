//
// Created by frehml on 03/05/2021.
//

#ifndef ENGINE_LINE_DRAWING_H
#define ENGINE_LINE_DRAWING_H

#include "vector3d.h"
#include "figure_3d.h"
#include "lines.h"

class LineDrawing {
public:
    static void parseLine(Figure &figure, const ini::Configuration &configuration);
    static void parsePoint(Figure &figure, const ini::Configuration &configuration);
    static void parse(Figure &figure, const ini::Configuration &configuration);
    static void doTransformations(Figure &figure, const ini::Configuration &configuration);
    static img::EasyImage generateImage(const ini::Configuration &configuration);
};


#endif //ENGINE_LINE_DRAWING_H
