//
// Created by frehml on 03/05/2021.
//

#include "Wireframe.h"

void Wireframe::parseLine(Figure &figure, const ini::Configuration &configuration, std::string &name){
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

void Wireframe::parsePoint(Figure &figure, const ini::Configuration &configuration, std::string &name){
    std::vector<Vector3D> points;
    int nr_points = configuration[name]["nrPoints"].as_int_or_die();
    for(int i = 0; i < nr_points; i ++){
        Vector3D vector_point;
        make_vector(vector_point, configuration[name]["point"+std::to_string(i)].as_double_tuple_or_die());
        points.push_back(vector_point);
    }
    figure.points = points;
}

void Wireframe::parse(Figure &figure, const ini::Configuration &configuration, std::string &name){
    make_color(figure.color, configuration[name]["color"].as_double_tuple_or_die());
    parsePoint(figure, configuration, name);
    parseLine(figure, configuration, name);
}

void Wireframe::doTransformations(Figure &figure, const ini::Configuration &configuration, std::string &name){
    double scale = configuration[name]["scale"].as_double_or_die();
    double rotateX = configuration[name]["rotateX"].as_double_or_die();
    double rotateY = configuration[name]["rotateY"].as_double_or_die();
    double rotateZ = configuration[name]["rotateZ"].as_double_or_die();

    Vector3D vector_center;
    make_vector(vector_center, configuration["Figure0"]["center"].as_double_tuple_or_die());
    Vector3D vector_eye;
    make_vector(vector_eye, configuration["General"]["eye"].as_double_tuple_or_die());

    Matrix transformations =    Matrix::scaleFigure(scale) * Matrix::rotateX(degToRad(rotateX)) *
                                Matrix::rotateY(degToRad(rotateY)) * Matrix::rotateZ(degToRad(rotateZ)) *
                                Matrix::translate(vector_center) * Matrix::eyePointTrans(vector_eye);

    figure.applyTransformation(transformations);
}

void Wireframe::createLineDrawing(const ini::Configuration &configuration, std::string &name, Figure &figure){
    parse(figure, configuration, name);
    doTransformations(figure, configuration, name);
}

void Wireframe::createTetrahedron(Figure &figure){
    Vector3D p1; p1.x = 1; p1.y = -1; p1.z = -1;
    Vector3D p2; p2.x = -1; p2.y = 1; p2.z = -1;
    Vector3D p3; p3.x = 1; p3.y = 1; p3.z = 1;
    Vector3D p4; p4.x = -1; p4.y = -1; p4.z = 1;

    figure.points = {p1, p2, p3, p4};

    Face f1; f1.point_indexes = {1, 2, 3};
    Face f2; f2.point_indexes = {2, 4, 3};
    Face f3; f3.point_indexes = {1, 4, 2};
    Face f4; f4.point_indexes = {1, 3, 4};

    figure.faces = {f1, f2, f3, f4};
}

img::EasyImage Wireframe::generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size){
    Figures3D figures;
    int nr_figures = configuration["General"]["nrFigures"].as_int_or_die();

    for(int i = 0; i < nr_figures; i++){
        std::string name = "Figure" + std::to_string(i);
        std::string type = configuration[name]["type"].as_string_or_die();

        img::Color figure_color;
        make_color(figure_color, configuration[name]["color"].as_double_tuple_or_die());

        Figure figure;
        figure.color = figure_color;

        if(type == "LineDrawing")
            createLineDrawing(configuration, name, figure);
        else if (type == "Tetrahedron")
            createTetrahedron(figure);

        figures.push_back(figure);
    }

    Lines2D lines = doProjection(figures);
    return draw2DLines(lines, size, background_color);
}