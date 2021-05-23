//
// Created by frehml on 30/04/2021.
//

#ifndef ENGINE_L_SYSTEM_H
#define ENGINE_L_SYSTEM_H

#include <fstream>
#include <cmath>
#include "ini_configuration.h"
#include "easy_image.h"
#include "lines.h"
#include "tools.h"
#include "l_parser.h"

class LSystem {
public:
    static img::EasyImage generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size);
    static Lines2D draw2DLSystem(LParser::LSystem2D &l_system, img::Color &color);
    static void create3DLSystem(Figure &figure, const ini::Configuration &configuration, std::string const &name);
    static void draw3DLSystem(Figure &figure, LParser::LSystem3D &l_system);
    static void doOperations(Vector3D &one, Vector3D &two, Vector3D HLU1, Vector3D HLU2, Vector3D HLU3, Vector3D HLU4, double angle);
    template<class LSys>
    static std::string calculateSequence(LSys &l_system, std::string sequence, unsigned int itterations);
};


#endif //ENGINE_L_SYSTEM_H
