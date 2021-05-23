//
// Created by frehml on 30/04/2021.
//

#ifndef ENGINE_LINES_H
#define ENGINE_LINES_H

#include <list>
#include <utility>
#include <algorithm>
#include "z_buffer.h"
#include "easy_image.h"
#include "tools.h"
#include "figure_3d.h"

class Point2D {
public:
    double x;
    double y;
    double z;

    Point2D();
    Point2D(double longitude, double latitude);
    static Point2D doProjection(const Vector3D &point, double d);
};

class Line2D {
public:
    Point2D p1;
    Point2D p2;
    img::Color c;

    Line2D();
    Line2D(double x1, double y1, double x2, double y2, img::Color &color);
    Line2D(Point2D point1, Point2D point2, const img::Color &color);
};

using Lines2D = std::list<Line2D>;
using Points2D = std::vector<Point2D>;

std::pair<std::pair<double, double>, std::pair<double, double>> calculateRange(const Lines2D &lines);
img::EasyImage draw2DLines(const Lines2D &lines, int size, img::Color &background_color, bool zbuf);
Lines2D doProjection(const Figures3D &);
void calculateImageProperties(double &x_range,
                              double &y_range,
                              double &x_image,
                              double &y_image,
                              double &d,
                              double &DCx,
                              double &DCy,
                              double &dx,
                              double &dy,
                              const Lines2D &lines,
                              const int &size);
#endif //ENGINE_LINES_H
