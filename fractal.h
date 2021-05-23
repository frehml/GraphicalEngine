//
// Created by frehml on 22/05/2021.
//

#ifndef ENGINE_FRACTAL_H
#define ENGINE_FRACTAL_H

#include "figure_3d.h"
#include "ini_configuration.h"

class Fractal {
public:
    static void createFractal(Figure &figure, const ini::Configuration &configuration, std::string &name, std::string &type, Figures3D &figures);
};


#endif //ENGINE_FRACTAL_H
