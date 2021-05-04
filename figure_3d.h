//
// Created by frehml on 02/05/2021.
//

#ifndef ENGINE_FIGURE_3D_H
#define ENGINE_FIGURE_3D_H

#include <vector>
#include "vector3d.h"
#include "easy_image.h"
#include "tools.h"

class Face
{
public:
    //De indexen refereren naar
    //punten in de ‘points’ vector
    //van de Figure-klasse
    std::vector<int> point_indexes;
};


class Figure
{
public:
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    img::Color color;

    void applyTransformation(const Matrix &);
};

typedef std::list<Figure> Figures3D;
void applyTransformation(Figures3D &figures, const Matrix &matrix);

#endif //ENGINE_FIGURE_3D_H
