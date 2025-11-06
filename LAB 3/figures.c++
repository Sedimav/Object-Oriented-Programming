#ifndef FIGURE_H
#define FIGURE_H

#include <vector>
#include <memory>
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>

void showMenu();

struct Point {
    double x, y;
    Point(double x = 0, double y = 0) : x(x), y(y) {}
};

class Figure {
public:
    virtual ~Figure() = default;
    virtual double area() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
    virtual Point center() const = 0;
};

class RegularPolygon : public Figure {
protected:
    std::vector<Point> vertices;
    Point center_;
    int sides = 0;

    void calculateCenter();  

public:
    RegularPolygon() = default;
    RegularPolygon(const std::vector<Point>& vertices);
    
    double area() const override;
    void print(std::ostream& os) const override;
    void read(std::istream& is) override;
    Point center() const override;
};

class Triangle : public RegularPolygon {
public:
    Triangle();
    Triangle(const std::vector<Point>& vertices);
    void read(std::istream& is) override;
};

class Hexagon : public RegularPolygon {
public:
    Hexagon();
    Hexagon(const std::vector<Point>& vertices);
    void read(std::istream& is) override;
};

class Octagon : public RegularPolygon {
public:
    Octagon();
    Octagon(const std::vector<Point>& vertices);
    void read(std::istream& is) override;
};

class FigureArray {
private:
    std::vector<std::shared_ptr<Figure>> figures;

public:
    void addFigure(std::shared_ptr<Figure> figure);
    void removeFigure(int index);
    void printAll() const;
    double totalArea() const;
    std::shared_ptr<Figure> operator[](size_t index) const;
    size_t size() const;
};

std::ostream& operator<<(std::ostream& os, const Figure& figure);
std::istream& operator>>(std::istream& is, Figure& figure);

// Реализации методов
void showMenu() {
    std::cout << "1. Добавить треугольник" << std::endl;
    std::cout << "2. Добавить шестиугольник" << std::endl;
    std::cout << "3. Добавить восьмиугольник" << std::endl;
    std::cout << "4. Показать все фигуры" << std::endl;
    std::cout << "5. Вычислить общую площадь" << std::endl;
    std::cout << "6. Удалить фигуру по индексу" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
}

RegularPolygon::RegularPolygon(const std::vector<Point>& vertices) 
    : vertices(vertices) {
    calculateCenter();
}

void RegularPolygon::calculateCenter() {
    double sum_x = 0, sum_y = 0;
    for (const auto& vertex : vertices) {
        sum_x += vertex.x;
        sum_y += vertex.y;
    }
    center_.x = sum_x / vertices.size();
    center_.y = sum_y / vertices.size();
}

double RegularPolygon::area() const {
    double area = 0.0;
    int n = vertices.size();
    
    for (int i = 0; i < n; i++) {
        int j = (i + 1) % n;
        area += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
    }
    
    return std::abs(area) / 2.0;
}

void RegularPolygon::print(std::ostream& os) const {
    os << "Вершины: ";
    for (const auto& vertex : vertices) {
        os << "(" << vertex.x << ", " << vertex.y << ") ";
    }
}

void RegularPolygon::read(std::istream& is) {
    vertices.clear();
    std::string line;
    
    std::cout << "Введите " << sides << " вершин (x y для каждой):" << std::endl;
    
    for (int i = 0; i < sides; ++i) {
        std::cout << "Вершина " << (i + 1) << ": ";
        std::getline(is, line);
        
        std::istringstream iss(line);
        double x, y;
        iss >> x >> y;
        
        char extra;
        if (iss.fail() || iss.get(extra)) {
            std::cout << "Ошибка: для вершины нужно ровно два числа!" << std::endl;
            is.setstate(std::ios::failbit);
            return;
        }
        
        vertices.push_back(Point(x, y));
    }
    calculateCenter();
}

Point RegularPolygon::center() const {
    return center_;
}

// Triangle
Triangle::Triangle(const std::vector<Point>& vertices) 
    : RegularPolygon(vertices) {
    sides = 3;
}

Triangle::Triangle() {
    sides = 3;
}

void Triangle::read(std::istream& is) {
    std::cout << "~~~ Треугольник ~~~" << std::endl;
    sides = 3;
    RegularPolygon::read(is);
}

// Hexagon 
Hexagon::Hexagon(const std::vector<Point>& vertices) 
    : RegularPolygon(vertices) {
    sides = 6;
}

Hexagon::Hexagon() {
    sides = 6;
}

void Hexagon::read(std::istream& is) {
    std::cout << "~~~ Шестиугольник ~~~" << std::endl;
    sides = 6;
    RegularPolygon::read(is);
}

// Octagon
Octagon::Octagon(const std::vector<Point>& vertices) 
    : RegularPolygon(vertices) {
    sides = 8;
}

Octagon::Octagon() {
    sides = 8;
}

void Octagon::read(std::istream& is) {
    std::cout << "~~~ Восьмиугольник ~~~" << std::endl;
    sides = 8;
    RegularPolygon::read(is);
}

std::ostream& operator<<(std::ostream& os, const Figure& figure) {
    figure.print(os);
    Point center = figure.center();
    os << " | Геом. центр: (" << center.x << ", " << center.y << ")";
    os << " | Площадь: " << figure.area();
    return os;
}

std::istream& operator>>(std::istream& is, Figure& figure) {
    figure.read(is);
    return is;
}

void FigureArray::addFigure(std::shared_ptr<Figure> figure) {
    figures.push_back(figure);
}

void FigureArray::removeFigure(int index) {
    if (index >= 0 && index < static_cast<int>(figures.size())) {
        figures.erase(figures.begin() + index);
    } else {
        std::cout << "Ошибка: индекс " << index << " вне диапазона!" << std::endl;
    }
}

void FigureArray::printAll() const {
    for (size_t i = 0; i < figures.size(); ++i) {
        std::cout << "Фигура " << i << ": " << *figures[i] << std::endl;
    }
}

double FigureArray::totalArea() const {
    double total = 0;
    for (const auto& figure : figures) {
        total += figure->area();
    }
    return total;
}

std::shared_ptr<Figure> FigureArray::operator[](size_t index) const {
    if (index < figures.size()) {
        return figures[index];
    }
    return nullptr;
}

size_t FigureArray::size() const {
    return figures.size();
}

#endif
