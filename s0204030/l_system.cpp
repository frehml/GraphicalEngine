//
// Created by frehml on 30/04/2021.
//

#include "l_system.h"

img::EasyImage LSystem::generateImage(const ini::Configuration &configuration, img::Color &background_color, int &size) {
    img::Color line_color;
    make_color(line_color, configuration["2DLSystem"]["color"].as_double_tuple_or_die());

    LParser::LSystem2D l_system;
    std::ifstream input_stream(configuration["2DLSystem"]["inputfile"]);
    input_stream >> l_system;
    input_stream.close();

    Lines2D lines = draw2DLSystem(l_system, line_color);
    return draw2DLines(lines, size, background_color, false);
}

Lines2D LSystem::draw2DLSystem(LParser::LSystem2D &l_system, img::Color &color){
    Lines2D lines;
    Point2D point1(0, 0);
    Point2D point2(0, 0);
    std::stack<std::pair<std::pair<Point2D, Point2D>, double>> bracket_stack;

    double cur_angle = l_system.get_starting_angle();
    double add_angle = l_system.get_angle();
    std::string sequence = calculateSequence(l_system, l_system.get_initiator(), l_system.get_nr_iterations());

    for (char &c : sequence){
        switch (c) {
            case ('+'):
                cur_angle += add_angle;
                break;
            case ('-'):
                cur_angle -= add_angle;
                break;
            case ('('):
                bracket_stack.emplace(std::pair<std::pair<Point2D, Point2D>, double>{{point1, point2}, cur_angle});
                break;
            case (')'):
                point1 = bracket_stack.top().first.first;
                point2 = bracket_stack.top().first.second;
                cur_angle = bracket_stack.top().second;
                bracket_stack.pop();
                break;
            default:
                point1 = point2;
                point2 = Point2D(point2.x+cos(degToRad(cur_angle)), point2.y+sin(degToRad(cur_angle)));
                if(l_system.draw(c))
                    lines.push_back(Line2D(point1, point2, color));
        }
    }

    return lines;
}

void LSystem::create3DLSystem(Figure &figure, const ini::Configuration &configuration, std::string const &name){
    LParser::LSystem3D l_system;
    std::string inputfile = configuration[name]["inputfile"].as_string_or_die();
    std::ifstream input_stream(inputfile);
    input_stream >> l_system;
    input_stream.close();
    draw3DLSystem(figure, l_system);
}

void LSystem::doOperations(Vector3D &one, Vector3D &two, Vector3D const HLU1, Vector3D const HLU2, Vector3D const HLU3, Vector3D const HLU4, double angle){
    one = HLU1 * cos(angle) + HLU2 * sin(angle);
    two = HLU3 * sin(angle) + HLU4 * cos(angle);
}

void LSystem::draw3DLSystem(Figure &figure, LParser::LSystem3D &l_system){
    std::string sequence = calculateSequence(l_system, l_system.get_initiator(), l_system.get_nr_iterations());
    Vector3D H = Vector3D::vector(1, 0, 0);
    Vector3D L = Vector3D::vector(0, 1, 0);
    Vector3D U = Vector3D::vector(0, 0, 1);
    Vector3D cur = Vector3D::point(0, 0, 0);
    std::stack<std::vector<Vector3D>> bracket_stack;
    double angle = degToRad(l_system.get_angle());

    for(char &c: sequence){
        switch (c) {
            case ('+'):
                doOperations(H, L, H, L, -H, L, angle);
                break;
            case ('-'):
                doOperations(H, L, H, L, -H, L, -angle);
                break;
            case ('^'):
                doOperations(H, U, H, U, -H, U, angle);
                break;
            case ('&'):
                doOperations(H, U, H, U, -H, U, -angle);
                break;
            case ('\\'):
                doOperations(L, U, L, -U, L, U, angle);
                break;
            case ('/'):
                doOperations(L, U, L, -U, L, U, -angle);
                break;
            case ('('):
                bracket_stack.emplace(std::vector<Vector3D>{cur, H, L, U});
                break;
            case ('|'):
                H = -H;
                L = -L;
                break;
            case (')'):
                cur = bracket_stack.top()[0];
                H = bracket_stack.top()[1];
                L = bracket_stack.top()[2];
                U = bracket_stack.top()[3];
                bracket_stack.pop();
                break;
            default:
                Vector3D old = cur;
                cur += H;
                if (l_system.draw(c)) {
                    figure.points.push_back(old);
                    figure.points.push_back(cur);
                    figure.faces.emplace_back(Face({roundToInt(figure.points.size() - 1),
                                                    roundToInt(figure.points.size() - 2)}));
                }
                break;
        }
    }
}

template<class LSys>
std::string LSystem::calculateSequence(LSys &l_system, std::string sequence, unsigned int itterations){
    if(itterations == 0){return sequence;}
    std::string new_sequence;

    for (char c : sequence){
        if (c == '+' || c == '-' || c == '(' || c == ')' || c == '&' || c == '/' || c == '|' || c == '^' || c == '\\')
            new_sequence += c;
        else
            new_sequence += l_system.get_replacement(c);
    }
    return calculateSequence(l_system, new_sequence, itterations-1);
}