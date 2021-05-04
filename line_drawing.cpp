//
// Created by frehml on 03/05/2021.
//

#include "line_drawing.h"

void LineDrawing::parseLine(Figure &figure, const ini::Configuration &configuration){
    std::vector<Face> faces;
    std::string line = "line";
    int nr_lines = configuration["Figure0"]["nrLines"].as_int_or_die();
    for(int i = 0; i < nr_lines; i ++){
        ini::IntTuple tuple_line = configuration["Figure0"][line+std::to_string(i)].as_int_tuple_or_die();
        Face face;
        face.point_indexes.push_back(tuple_line[0]);
        face.point_indexes.push_back(tuple_line[1]);
        faces.push_back(face);
    }
    figure.faces = faces;
}

void LineDrawing::parsePoint(Figure &figure, const ini::Configuration &configuration){
    std::vector<Vector3D> points;
    std::string point = "point";
    int nr_points = configuration["Figure0"]["nrPoints"].as_int_or_die();
    for(int i = 0; i < nr_points; i ++){
        ini::DoubleTuple tuple_point = configuration["Figure0"][point+std::to_string(i)].as_double_tuple_or_die();
        Vector3D vector_point;
        vector_point.x = tuple_point[0];
        vector_point.y = tuple_point[1];
        vector_point.z = tuple_point[2];
        points.push_back(vector_point);
    }
    figure.points = points;
}

void LineDrawing::parse(Figure &figure, const ini::Configuration &configuration){
    make_color(figure.color, configuration["Figure0"]["color"].as_double_tuple_or_die());
    parsePoint(figure, configuration);
    parseLine(figure, configuration);
}

void LineDrawing::doTransformations(Figure &figure, const ini::Configuration &configuration){
    double scale = configuration["Figure0"]["scale"].as_double_or_die();
    double rotateX = configuration["Figure0"]["rotateX"].as_double_or_die();
    double rotateY = configuration["Figure0"]["rotateY"].as_double_or_die();
    double rotateZ = configuration["Figure0"]["rotateZ"].as_double_or_die();

    ini::DoubleTuple eye = configuration["General"]["eye"].as_double_tuple_or_die();
    ini::DoubleTuple center = configuration["Figure0"]["center"].as_double_tuple_or_die();

    Vector3D vector_center;
    vector_center.x = center[0];
    vector_center.y = center[1];
    vector_center.z = center[2];

    Vector3D vector_eye;
    vector_eye.x = eye[0];
    vector_eye.y = eye[1];
    vector_eye.z = eye[2];

    Matrix transformations =    Matrix::scaleFigure(scale) * Matrix::rotateX(degToRad(degToRad(rotateX))) *
                                Matrix::rotateY(degToRad(rotateY)) * Matrix::rotateZ(degToRad(rotateZ)) *
                                Matrix::translate(vector_center) * Matrix::eyePointTrans(vector_eye);

    figure.applyTransformation(transformations);
}

img::EasyImage LineDrawing::generateImage(const ini::Configuration &configuration){
    img::Color background_color;
    Figure figure;
    Figures3D figures;
    Lines2D lines;
    int size = configuration["General"]["size"].as_int_or_die();

    make_color(background_color, configuration["General"]["backgroundcolor"].as_double_tuple_or_die());
    parse(figure, configuration);
    doTransformations(figure, configuration);
    figures.push_back(figure);
    lines = doProjection(figures);

    img::EasyImage image = draw2DLines(lines, size, background_color);
    return image;
}