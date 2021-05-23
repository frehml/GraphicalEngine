//
// Created by frehml on 20/05/2021.
//

#include "z_buffer.h"
#include "wireframe.h"
#include "figure_3d.h"

ZBuffer::ZBuffer(const int width, const int height) {
    buffer.resize(width);
    double inf = std::numeric_limits<double>::infinity();
    for(int x = 0; x < width; x++){
        buffer[x].resize(height);
        for(int y = 0; y < height; y++){
            buffer[x][y] = inf;
        }
    }
}

//https://www.youtube.com/watch?v=HyVc0X9JKpg
bool ZBuffer::TestAndSet(int x, int y, double depth){
    if(depth < buffer[x][y]){
        buffer[x][y] = depth;
        return true;
    }
    return false;
}

double ZBuffer::calculateDepth(double i, double a, double za, double zb){
    return ((1.0-(i/a))/za) + (i/a/zb);
}

img::EasyImage ZBuffer::generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size){
    int nr_figures = configuration["General"]["nrFigures"].as_int_or_die();
    Figures3D figures;
    Wireframe::draw3D(nr_figures, configuration, figures);

    for(Figure &figure : figures){
        figure.triangulate();
    }

    return Figure::drawTriangles(figures, size, background_color, true);
}
