//
// Created by frehml on 02/05/2021.
//

#include "figure_3d.h"
#include "lines.h"

Face::Face(){
    point_indexes = std::vector<int>();
}

Face::Face(std::vector<int> p_i){
    point_indexes = p_i;
}

void Figure::applyTransformation(const Matrix &matrix){
    for(Vector3D &point : points){
        point = point * matrix;
    }
}

void Figure::createTetrahedron(Figure &figure){
    figure.points = {Vector3D::point(1, -1, -1), Vector3D::point(-1, 1, -1),
                     Vector3D::point(1, 1, 1), Vector3D::point(-1, -1, 1)};

    figure.faces = {Face({0, 1, 2}), Face({1, 3, 2}), Face({0, 3, 1}), Face({0, 2, 3})};
}

void Figure::createCube(Figure &figure){
    figure.points = {Vector3D::point(1, -1, -1), Vector3D::point(-1, 1, -1),
                     Vector3D::point(1, 1, 1), Vector3D::point(-1, -1, 1),
                     Vector3D::point(1, 1, -1), Vector3D::point(-1, -1, -1),
                     Vector3D::point(1, -1, 1), Vector3D::point(-1, 1, 1)};

    figure.faces = {Face({0, 4, 2, 6}), Face({4, 1, 7, 2}), Face({1, 5, 3, 7}),
                    Face({5, 0, 6, 3}), Face({6, 2, 7, 3}), Face({0, 5, 1, 4})};
}

void Figure::createIcosahedron(Figure &figure){
    for(int i = 1; i <= 12; i++){
        if (i == 1)
            figure.points.push_back(Vector3D::point(0, 0, sqrt(5)/2));
        else if (i > 1 && i < 7)
            figure.points.push_back(Vector3D::point(cos((i-2)*(2*M_PI)/5), sin((i-2)*(2*M_PI)/5),0.5));
        else if (i > 6 && i < 12)
            figure.points.push_back(Vector3D::point(cos(M_PI/5+(i-7)*2*M_PI/5), sin(M_PI/5+(i-7)*2*M_PI/5),-0.5));
        else
            figure.points.push_back(Vector3D::point(0, 0, -sqrt(5)/2));
    }

    figure.faces = {Face({0, 1, 2}), Face({0, 2, 3}), Face({0, 3, 4}),
                    Face({0, 4, 5}), Face({0, 5, 1}), Face({1, 6, 2}),
                    Face({2, 6, 7}), Face({2, 7, 3}), Face({3, 7, 8}),
                    Face({3, 8, 4}), Face({4, 8, 9}), Face({4, 9, 5}),
                    Face({5, 9, 10}), Face({5, 10, 1}), Face({1, 10, 6}),
                    Face({11, 7, 6}), Face({11, 8, 7}), Face({11, 9, 8}),
                    Face({11, 10, 9}), Face({11, 6, 10})};
}

void Figure::createOctahedron(Figure &figure){
    figure.faces = {Face({0, 1, 5}), Face({1, 2, 5}), Face({2, 3, 5}),
                    Face({3, 0, 5}), Face({1, 0, 4}), Face({2, 1, 4}),
                    Face({3, 2, 4}), Face({0, 3, 4})};

    figure.points = {Vector3D::point(1, 0, 0), Vector3D::point(0, 1, 0),
                     Vector3D::point(-1, 0, 0), Vector3D::point(0, -1, 0),
                     Vector3D::point(0, 0, -1), Vector3D::point(0, 0, 1)};
}

void Figure::createDodecahedron(Figure &figure){
    Figure ico;
    createIcosahedron(ico);

    for(Face &triangle : ico.faces){
        const Vector3D A = ico.points[triangle.point_indexes[0]];
        const Vector3D B = ico.points[triangle.point_indexes[1]];
        const Vector3D C = ico.points[triangle.point_indexes[2]];
        figure.points.push_back(Vector3D::normalise(A+B+C)/3);
    }

    figure.faces = {Face({0, 1, 2, 3, 4}),
                    Face({0, 5, 6, 7, 1}),
                    Face({1, 7, 8, 9, 2}),
                    Face({2, 9, 10, 11, 3}),
                    Face({3, 11, 12, 13, 4}),
                    Face({4, 13, 14, 5, 0}),
                    Face({19, 18, 17, 16, 15}),
                    Face({19, 14, 13, 12, 18}),
                    Face({18, 12, 11, 10, 17}),
                    Face({17, 10, 9, 8, 16}),
                    Face({16, 8, 7, 6, 15}),
                    Face({15, 6, 5, 14, 19})};
}

void Figure::splitTriangles(){
    std::vector<Face> new_faces;

    for(Face &face : faces){
        Vector3D &A = points[face.point_indexes[0]];
        Vector3D &B = points[face.point_indexes[1]];
        Vector3D &C = points[face.point_indexes[2]];

        Vector3D D = Vector3D::point((A.x+B.x)/2, (A.y+B.y)/2, (A.z+B.z)/2);
        Vector3D E = Vector3D::point((A.x+C.x)/2, (A.y+C.y)/2, (A.z+C.z)/2);
        Vector3D F = Vector3D::point((B.x+C.x)/2, (B.y+C.y)/2, (B.z+C.z)/2);

        points.insert(points.end(), {D, E, F});
        int idx = points.size()-1;

        Face ADE = Face({face.point_indexes[0], idx-2, idx-1});
        Face BFD = Face({face.point_indexes[1], idx, idx-2});
        Face CEF = Face({face.point_indexes[2], idx-1, idx});
        Face DFE = Face({idx-2, idx, idx-1});

        new_faces.insert(new_faces.end(), {ADE, BFD, CEF, DFE});
    }
    faces = new_faces;
}

void Figure::normalise(){
    for(Vector3D &point : points){
        point.normalise();
    }
}

void Figure::createSphere(Figure &figure, const int n){
    createIcosahedron(figure);
    for (int i = 0; i < n; i++){
        figure.splitTriangles();
    }
    figure.normalise();
}

void Figure::createCone(Figure &figure, const int n, const double h){
    std::vector<int> bottom(n);
    std::iota(std::begin(bottom), std::end(bottom), 0);

    for (int i = 0; i < n; i++) {
        figure.points.push_back(Vector3D::point(cos(2 * i * M_PI / n), sin(2 * i * M_PI / n), 0));
        figure.faces.push_back(Face({i%n, (i + 1) % n, n}));
    }

    figure.points.push_back(Vector3D::point(0, 0, h));
    figure.faces.emplace_back(bottom);
}

void Figure::createCylinder(Figure &figure, const int n, const double h){
    std::vector<int> bottom(n);
    std::vector<int> top(n);
    std::iota(std::begin(bottom), std::end(bottom), 0);
    std::iota(std::begin(top), std::end(top), n);

    double height = 0;
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < n; b++) {
            figure.points.push_back(Vector3D::point(cos(2 * b * M_PI / n), sin(2 * b * M_PI / n), height));
            figure.faces.push_back(Face({b%n, (b + 1) % n, (b + 1) % n+n, b%n+n}));
        }
        height = h;
    }

    figure.faces.emplace_back(bottom);
    figure.faces.emplace_back(top);
}

void Figure::createTorus(Figure &figure, const double r, const double R, const int n, const int m){
    for(int circle = 0; circle < n; circle++){
        double u = 2 * circle * M_PI / n;
        for(int point = 0; point < m; point++){
            double v = 2 * point * M_PI / m;
            figure.points.push_back(Vector3D::point((R+r*cos(v)) * cos(u),(R+r*cos(v))*sin(u),r*sin(v)));
            figure.faces.push_back(Face({circle*m+point,
                                         (circle + 1) % n * m + point,
                                         (circle + 1) % n * m+ (point + 1) % m,
                                         circle * m + (point + 1) % m}));
        }
    }
}

//http://mathcentral.uregina.ca/QQ/database/QQ.09.08/h/brian4.html#:~:text=A%20hexagon%20can%20be%20subdivided,the%20area%20of%20the%20hexagon.
void Figure::createBuckyBall(Figure &figure) {
    std::vector<Face> new_faces;
    createIcosahedron(figure);
    for (Face &face : figure.faces) {
        int idx = figure.points.size();
        Vector3D A = figure.points[face.point_indexes[0]];
        Vector3D B = figure.points[face.point_indexes[1]];
        Vector3D C = figure.points[face.point_indexes[2]];
        std::vector<Vector3D> p = {A*2/3+B/3, A/3+B*2/3, B*2/3+C/3, B/3+C*2/3, A/3+C*2/3, A*2/3+C/3};
        figure.points.insert(figure.points.end(), p.begin(), p.end());
        new_faces.push_back(Face({idx, idx+1, idx+2, idx+3, idx+4, idx+5}));
    }
    figure.faces = new_faces;
}

void applyTransformation(Figures3D &figures, const Matrix &matrix){
    for(Figure &figure : figures){
        figure.applyTransformation(matrix);
    }
}

void Figure::triangulate(){
    std::vector<Face> new_faces;
    for(Face const &face : faces){
        int n = face.point_indexes.size();
        for(int i = 1; i < n-1; i++){
            std::vector<int> idx = face.point_indexes;
            new_faces.push_back(Face({idx[0], idx[i], idx[i+1]}));
        }
    }
    faces = new_faces;
}


void Figure::createPlatonic(const ini::Configuration &configuration, Figure &figure, std::string &name, std::string &type){
    double h = configuration[name]["height"].as_double_or_default(0);
    int n = configuration[name]["n"].as_int_or_default(0);
    int m = configuration[name]["m"].as_int_or_default(0);
    double r = configuration[name]["r"].as_double_or_default(0);
    double R = configuration[name]["R"].as_double_or_default(0);

    if (type == "Tetrahedron")
        Figure::createTetrahedron(figure);
    else if (type == "Cube")
        Figure::createCube(figure);
    else if (type == "Icosahedron")
        Figure::createIcosahedron(figure);
    else if (type == "Octahedron")
        Figure::createOctahedron(figure);
    else if (type == "Dodecahedron")
        Figure::createDodecahedron(figure);
    else if (type == "Sphere")
        Figure::createSphere(figure, n);
    else if (type == "Cylinder")
        Figure::createCylinder(figure, n, h);
    else if (type == "Cone")
        Figure::createCone(figure, n, h);
    else if (type == "Torus")
        Figure::createTorus(figure, r, R, m, n);
    else if (type == "BuckyBall")
        Figure::createBuckyBall(figure);
}

img::EasyImage Figure::drawTriangles(std::list<Figure> &figures, const int &size, img::Color &background_color, bool zbuf){
    const Lines2D lines = doProjection(figures);
    double x_range, y_range, x_image ,y_image, d, DCx, DCy, dx, dy;
    calculateImageProperties(x_range, y_range, x_image, y_image, d, DCx, DCy, dx, dy, lines, size);
    img::EasyImage image(roundToInt(x_image), roundToInt(y_image), background_color);

    ZBuffer buffer;
    if (zbuf)
        buffer = ZBuffer(roundToInt(x_image), roundToInt(y_image));

    for(Figure figure : figures){
        for(Face face : figure.faces){
            Vector3D A = figure.points[face.point_indexes[0]];
            Vector3D B = figure.points[face.point_indexes[1]];
            Vector3D C = figure.points[face.point_indexes[2]];
            image.draw_zbuf_triag(buffer, A, B, C, d, dx, dy, figure.color);
        }
    }

    return image;
}