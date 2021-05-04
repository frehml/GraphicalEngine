//
// Created by frehml on 30/04/2021.
//

#include "lines.h"
Point2D::Point2D(){
    x = 0;
    y = 0;
}

Point2D::Point2D(double longitude, double latitude) {
    x = longitude;
    y = latitude;
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

img::EasyImage draw2DLines(const Lines2D &lines, const int size, img::Color &background_color){
    std::pair<std::pair<double, double>, std::pair<double, double>> ranges = calculateRange(lines);
    double x_range = ranges.first.second - ranges.first.first;
    double y_range = ranges.second.second - ranges.second.first;
    double x_image = size*(x_range/(std::max(x_range, y_range)));
    double y_image = size*(y_range/(std::max(x_range, y_range)));
    double d = 0.95 * x_image/x_range;
    double DCx = d*((ranges.first.first+ranges.first.second)/2);
    double DCy = d*((ranges.second.first+ranges.second.second)/2);
    double dx = (x_image/2)-DCx;
    double dy = (y_image/2)-DCy;

    img::EasyImage image(roundToInt(x_image), roundToInt(y_image), background_color);

    for(auto const &line : lines){
        unsigned int x1 = roundToInt(line.p1.x*d+dx);
        unsigned int y1 = roundToInt(line.p1.y*d+dy);
        unsigned int x2 = roundToInt(line.p2.x*d+dx);
        unsigned int y2 = roundToInt(line.p2.y*d+dy);
        image.draw_line(x1, y1, x2, y2, line.c);
    }

    return image;
}

Point2D Point2D::doProjection(const Vector3D &point, const double d) {
    Point2D new_point;

    new_point.x = (d*point.x)/-point.z;
    new_point.y = (d*point.y)/-point.z;
    return new_point;
}

Lines2D doProjection(const Figures3D &figures){
    Lines2D lines;

    for(Figure const &figure : figures){
        Points2D points;
        for(Vector3D const &point : figure.points){
            points.push_back(Point2D::doProjection(point, 1));
        }
        for(Face face : figure.faces){
            double x1 = points[face.point_indexes[0]].x;
            double y1 = points[face.point_indexes[0]].y;
            double x2 = points[face.point_indexes[1]].x;
            double y2 = points[face.point_indexes[1]].y;
            img::Color color = figure.color;
            Line2D line(x1, y1, x2, y2, color);
            lines.push_back(line);
        }
    }
    return lines;
}
