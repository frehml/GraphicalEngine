//
// Created by frehml on 02/05/2021.
//

#ifndef ENGINE_FIGURE_3D_H
#define ENGINE_FIGURE_3D_H

#include <vector>
#include "vector3d.h"
#include "easy_image.h"
#include "tools.h"
#include <numeric>

class Face
{
public:
    //De indexen refereren naar
    //punten in de ‘points’ vector
    //van de Figure-klasse
    Face();
    explicit Face(std::vector<int> p_i);
    std::vector<int> point_indexes;
};


class Figure
{
public:
    std::vector<Vector3D> points;
    std::vector<Face> faces;
    img::Color color;

    void applyTransformation(const Matrix &);
    static void createTetrahedron(Figure &figure);
    static void createCube(Figure &figure);
    static void createIcosahedron(Figure &figure);
    static void createOctahedron(Figure &figure);
    static void createDodecahedron(Figure &figure);
    static void createSphere(Figure &figure, int n);
    void splitTriangles();
    void normalise();
    static void createCylinder(Figure &figure, int n, double h);
    static void createCone(Figure &figure, int n, double h);
    static void createTorus(Figure &figure, double r, double R, int n, int m);
    static void createBuckyBall(Figure &figure);
    static void createMengerSponge(Figure &figure, int it);
    void triangulate();
    static void createPlatonic(const ini::Configuration &configuration, Figure &figure, std::string &name, std::string &type);
    static img::EasyImage drawTriangles(std::list<Figure> &figures, const int &size, img::Color &background_color, bool zbuf);
};

typedef std::list<Figure> Figures3D;
void applyTransformation(Figures3D &figures, const Matrix &matrix);

#endif //ENGINE_FIGURE_3D_H
