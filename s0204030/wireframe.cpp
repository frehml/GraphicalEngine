//
// Created by frehml on 03/05/2021.
//

#include "wireframe.h"
#include "fractal.h"

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

void Wireframe::doTransformations(Figure &figure, const ini::Configuration &configuration, std::string &name){
    double scale = configuration[name]["scale"].as_double_or_die();
    double rotateX = configuration[name]["rotateX"].as_double_or_die();
    double rotateY = configuration[name]["rotateY"].as_double_or_die();
    double rotateZ = configuration[name]["rotateZ"].as_double_or_die();

    Vector3D vector_center;
    make_vector(vector_center, configuration[name]["center"].as_double_tuple_or_die());
    Vector3D vector_eye;
    make_vector(vector_eye, configuration["General"]["eye"].as_double_tuple_or_die());

    Matrix transformations =    Matrix::scaleFigure(scale) * Matrix::rotateX(degToRad(rotateX)) *
                                Matrix::rotateY(degToRad(rotateY)) * Matrix::rotateZ(degToRad(rotateZ)) *
                                Matrix::translate(vector_center) * Matrix::eyePointTrans(vector_eye);

    figure.applyTransformation(transformations);
}

void Wireframe::createLineDrawing(const ini::Configuration &configuration, std::string &name, Figure &figure){
    make_color(figure.color, configuration[name]["color"].as_double_tuple_or_die());
    parsePoint(figure, configuration, name);
    parseLine(figure, configuration, name);
}

void Wireframe::draw3D(int &nr_figures, const ini::Configuration &configuration, Figures3D &figures){
    for(int i = 0; i < nr_figures; i++){
        std::string name = "Figure" + std::to_string(i);
        std::string type = configuration[name]["type"].as_string_or_die();
        img::Color figure_color;
        make_color(figure_color, configuration[name]["color"].as_double_tuple_or_die());
        Figure figure;
        figure.color = figure_color;

        if (type.find("Fractal") != std::string::npos)
            Fractal::createFractal(figure, configuration, name, type, figures);
        else if(type == "LineDrawing")
            createLineDrawing(configuration, name, figure);
        else if (type == "3DLSystem")
            LSystem::create3DLSystem(figure, configuration, name);
        else
            Figure::createPlatonic(configuration, figure, name, type);

        doTransformations(figure, configuration, name);
        figures.push_back(figure);
    }
}

img::EasyImage Wireframe::generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size, bool zbuf){
    Figures3D figures;
    int nr_figures = configuration["General"]["nrFigures"].as_int_or_die();
    draw3D(nr_figures, configuration, figures);
    Lines2D lines = doProjection(figures);
    return draw2DLines(lines, size, background_color, zbuf);
}