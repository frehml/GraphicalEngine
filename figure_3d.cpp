//
// Created by frehml on 02/05/2021.
//

#include "figure_3d.h"

void Figure::applyTransformation(const Matrix &matrix){
    for(Vector3D &point : points){
        point = point * matrix;
    }
}

void applyTransformation(Figures3D &figures, const Matrix &matrix){
    for(Figure &figure : figures){
        figure.applyTransformation(matrix);
    }
}