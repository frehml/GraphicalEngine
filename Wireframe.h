//
// Created by frehml on 03/05/2021.
//

#ifndef ENGINE_WIREFRAME_H
#define ENGINE_WIREFRAME_H

#include "vector3d.h"
#include "figure_3d.h"
#include "lines.h"

class Wireframe {
public:
    static void parseLine(Figure &figure, const ini::Configuration &configuration, std::string &name);
    static void parsePoint(Figure &figure, const ini::Configuration &configuration, std::string &name);
    static void parse(Figure &figure, const ini::Configuration &configuration, std::string &name);
    static void doTransformations(Figure &figure, const ini::Configuration &configuration, std::string &name);
    static void createLineDrawing(const ini::Configuration &configuration, std::string &name, Figure &figure);
    static void createTetrahedron(Figure &figure);
    static img::EasyImage generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size);
};


#endif //ENGINE_WIREFRAME_H
