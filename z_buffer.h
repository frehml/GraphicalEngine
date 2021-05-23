//
// Created by frehml on 20/05/2021.
//

#ifndef ENGINE_Z_BUFFER_H
#define ENGINE_Z_BUFFER_H

#include <vector>
#include <limits>
#include <iostream>
#include "ini_configuration.h"
#include "easy_image.h"

namespace img{
    class EasyImage;
    class Color;
}

class ZBuffer : public std::vector<std::vector<double>> {
public:
    //Constructor: maakt een Z-Buffer van de correcte
    //grootte aan en initialiseert alle velden op +inf
    vector <vector<double>> buffer;

    ZBuffer(const int width, const int height);
    ZBuffer(){};

    bool TestAndSet(int x, int y, double depth);

    static double calculateDepth(double i, double a, double za, double zb);

    static img::EasyImage generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size);
};


#endif //ENGINE_Z_BUFFER_H
