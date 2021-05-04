//
// Created by frehml on 30/04/2021.
//

#ifndef ENGINE_L_SYSTEM_2D_H
#define ENGINE_L_SYSTEM_2D_H

#include <fstream>
#include <cmath>
#include "ini_configuration.h"
#include "easy_image.h"
#include "lines.h"
#include "tools.h"
#include "l_parser.h"

class LSystem {
public:
    static img::EasyImage generateImage(const ini::Configuration &configuration);
    static Lines2D drawLSystem(LParser::LSystem2D &l_system, img::Color &color);
    static std::string calculateSequence(LParser::LSystem2D &l_system, std::string sequence, unsigned int itterations);
};


#endif //ENGINE_L_SYSTEM_2D_H
