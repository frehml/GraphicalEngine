//
// Created by frehml on 30/04/2021.
//

#ifndef ENGINE_L_SYSTEMS_2D_H
#define ENGINE_L_SYSTEMS_2D_H

#include <fstream>
#include <cmath>
#include "ini_configuration.h"
#include "easy_image.h"
#include "lines.h"
#include "tools.h"
#include "l_parser.h"

class l_systems_2d {
public:
    static img::EasyImage generate_image(const ini::Configuration &configuration);
    static Lines2D drawLSystem(LParser::LSystem2D &l_system, img::Color &color);
    static std::string calculateSequence(LParser::LSystem2D &l_system, std::string sequence, unsigned int itterations);
};


#endif //ENGINE_L_SYSTEMS_2D_H
