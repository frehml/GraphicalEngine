//
// Created by frehml on 30/04/2021.
//

#include "lines.h"

#include <chrono>
using namespace std::chrono;

Point2D::Point2D(){
    x = 0;
    y = 0;
    z = 0;
}

Point2D::Point2D(double longitude, double latitude) {
    x = longitude;
    y = latitude;
    z = 0;
}

Line2D::Line2D(){
    p1 = Point2D();
    p2 = Point2D();
    c = img::Color();
}

Line2D::Line2D(double x1, double y1, double x2, double y2, img::Color &color){
    p1 = Point2D(x1, y1);
    p2 = Point2D(x2, y2);
    c = color;
}

Line2D::Line2D(Point2D point1, Point2D point2, const img::Color &color) {
    p1 = point1;
    p2 = point2;
    c = color;
}

std::pair<std::pair<double, double>, std::pair<double, double>> calculateRange(const Lines2D &lines){
    std::pair<double, double> x_min_max(lines.front().p1.x, lines.front().p1.x);
    std::pair<double, double> y_min_max(lines.front().p1.y, lines.front().p1.y);

    for (auto const &line : lines){
        const double x1 = line.p1.x;
        const double x2 = line.p2.x;
        const double y1 = line.p1.y;
        const double y2 = line.p2.y;

        if(x1 < x_min_max.first) {x_min_max.first = x1;}
        if(x2 < x_min_max.first) {x_min_max.first = x2;}
        if(x1 > x_min_max.second) {x_min_max.second = x1;}
        if(x2 > x_min_max.second) {x_min_max.second = x2;}

        if(y1 < y_min_max.first) {y_min_max.first = y1;}
        if(y2 < y_min_max.first) {y_min_max.first = y2;}
        if(y1 > y_min_max.second) {y_min_max.second = y1;}
        if(y2 > y_min_max.second) {y_min_max.second = y2;}
    }

    return {x_min_max, y_min_max};
}

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
                              const int &size){
    std::pair<std::pair<double, double>, std::pair<double, double>> ranges = calculateRange(lines);
    x_range = ranges.first.second - ranges.first.first;
    y_range = ranges.second.second - ranges.second.first;
    x_image = size*(x_range/(std::max(x_range, y_range)));
    y_image = size*(y_range/(std::max(x_range, y_range)));
    d = 0.95 * x_image/x_range;
    DCx = d*((ranges.first.first+ranges.first.second)/2);
    DCy = d*((ranges.second.first+ranges.second.second)/2);
    dx = (x_image/2)-DCx;
    dy = (y_image/2)-DCy;
}

img::EasyImage draw2DLines(const Lines2D &lines, const int size, img::Color &background_color, bool zbuf){
    double x_range, y_range, x_image ,y_image, d, DCx, DCy, dx, dy;
    calculateImageProperties(x_range, y_range, x_image, y_image, d, DCx, DCy, dx, dy, lines, size);

    img::EasyImage image(roundToInt(x_image), roundToInt(y_image), background_color);

    ZBuffer buffer;
    if (zbuf)
        buffer = ZBuffer(roundToInt(x_image), roundToInt(y_image));

    for(auto &line : lines){
        unsigned int x1 = roundToInt(line.p1.x*d+dx);
        unsigned int y1 = roundToInt(line.p1.y*d+dy);
        unsigned int x2 = roundToInt(line.p2.x*d+dx);
        unsigned int y2 = roundToInt(line.p2.y*d+dy);
        if (zbuf)
            image.draw_zbuf_line(buffer, x1, y1, line.p1.z, x2, y2, line.p2.z, line.c);
        else
            image.draw_line(x1, y1, x2, y2, line.c);
    }
    return image;
}

Point2D Point2D::doProjection(const Vector3D &point, const double d) {
    Point2D new_point;

    new_point.x = (d*point.x)/-point.z;
    new_point.y = (d*point.y)/-point.z;
    new_point.z = point.z;
    return new_point;
}

Lines2D doProjection(const Figures3D &figures){
    Lines2D lines;
    for(Figure const &figure : figures){
        Points2D points;
        img::Color color = figure.color;

        for(Vector3D const &point : figure.points){
            points.push_back(Point2D::doProjection(point, 1));
        }

        for(Face face : figure.faces){
            int size = face.point_indexes.size();
            for(int a = 0; a < size; a++){
                Point2D p1 = points[face.point_indexes[a%size]];
                Point2D p2 = points[face.point_indexes[(a+1)%size]];

                Line2D line(p1, p2, color);
                lines.push_back(line);
            }
        }
    }
    return lines;
}