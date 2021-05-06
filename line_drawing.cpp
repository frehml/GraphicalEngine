//
// Created by frehml on 03/05/2021.
//

#include "line_drawing.h"

void LineDrawing::parseLine(Figure &figure, const ini::Configuration &configuration, std::string &name){
    std::vector<Face> faces;
    int nr_lines = configuration[name]["nrLines"].as_int_or_die();
    for(int i = 0; i < nr_lines; i ++){
        ini::IntTuple tuple_line = configuration[name]["line"+std::to_string(i)].as_int_tuple_or_die();
        Face face;
        face.point_indexes.push_back(tuple_line[0]);
        face.point_indexes.push_back(tuple_line[1]);
        faces.push_back(face);
    }
    figure.faces = faces;
}

void LineDrawing::parsePoint(Figure &figure, const ini::Configuration &configuration, std::string &name){
    std::vector<Vector3D> points;
    int nr_points = configuration[name]["nrPoints"].as_int_or_die();
    for(int i = 0; i < nr_points; i ++){
        ini::DoubleTuple tuple_point = configuration[name]["point"+std::to_string(i)].as_double_tuple_or_die();
        Vector3D vector_point;
        vector_point.x = tuple_point[0];
        vector_point.y = tuple_point[1];
        vector_point.z = tuple_point[2];
        points.push_back(vector_point);
    }
    figure.points = points;
}

void LineDrawing::parse(Figure &figure, const ini::Configuration &configuration, std::string &name){
    make_color(figure.color, configuration[name]["color"].as_double_tuple_or_die());
    parsePoint(figure, configuration, name);
    parseLine(figure, configuration, name);
}

void LineDrawing::doTransformations(Figure &figure, const ini::Configuration &configuration, std::string &name){
    double scale = configuration[name]["scale"].as_double_or_die();
    double rotateX = configuration[name]["rotateX"].as_double_or_die();
    double rotateY = configuration[name]["rotateY"].as_double_or_die();
    double rotateZ = configuration[name]["rotateZ"].as_double_or_die();

    ini::DoubleTuple eye = configuration["General"]["eye"].as_double_tuple_or_die();
    ini::DoubleTuple center = configuration["Figure0"]["center"].as_double_tuple_or_die();

    Vector3D vector_center;
    vector_center.x = center[0];
    vector_center.y = center[1];
    vector_center.z = center[2];



    Matrix transformations =    Matrix::scaleFigure(scale) * Matrix::rotateX(degToRad(rotateX)) *
                                Matrix::rotateY(degToRad(rotateY)) * Matrix::rotateZ(degToRad(rotateZ)) *
                                Matrix::translate(vector_center);

    figure.applyTransformation(transformations);
}

img::EasyImage LineDrawing::generateImage(const ini::Configuration &configuration){
    img::Color background_color;
    int size = configuration["General"]["size"].as_int_or_die();
    make_color(background_color, configuration["General"]["backgroundcolor"].as_double_tuple_or_die());

    Figures3D figures;
    Lines2D lines;
    int nr_figures = configuration["General"]["nrFigures"].as_int_or_die();

    for(int i = 0; i < nr_figures; i++){
        Figure figure;
        std::string name = "Figure" + std::to_string(i);
        parse(figure, configuration, name);
        doTransformations(figure, configuration, name);
        figures.push_back(figure);
    }
    ini::DoubleTuple eye = configuration["General"]["eye"].as_double_tuple_or_die();

    Vector3D vector_eye;
    vector_eye.x = eye[0];
    vector_eye.y = eye[1];
    vector_eye.z = eye[2];

    applyTransformation(figures, Matrix::eyePointTrans(vector_eye));

    lines = doProjection(figures);

    img::EasyImage image = draw2DLines(lines, size, background_color);
    return image;
}