//
// Created by frehml on 22/05/2021.
//

#include "fractal.h"
#include "wireframe.h"

void Fractal::createFractal(Figure &figure, const ini::Configuration &configuration, std::string &name, std::string &type, Figures3D &figures){
    int nr_iterations = configuration[name]["nrIterations"].as_int_or_die() ;
    double scale = configuration[name]["fractalScale"].as_double_or_die();
    std::string t = type.substr(7, type.size()-7);

    Figure::createPlatonic(configuration, figure, name, t);
    Wireframe::doTransformations(figure, configuration, name);
    Figures3D new_figures = {figure};

    for(int i = 0; i < nr_iterations; i++){
        Figures3D newest_figures;
        for (Figure const &fig : new_figures){
            for(Vector3D const &point : fig.points){
                Figure new_figure = fig;
                new_figure.applyTransformation(Matrix::scaleFigure(1/(scale-1)));
                new_figure.applyTransformation(Matrix::translate(point));
                newest_figures.push_back(new_figure);
            }
        }
        new_figures = newest_figures;
    }
    figures.insert(figures.end(), new_figures.begin(), new_figures.end());
    figure = Figure();
}