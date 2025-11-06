#include "figures.c++"
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <cmath>

// Вспомогательная функция для создания правильного треугольника
std::vector<Point> createRegularTriangle(Point center, double radius) {
    std::vector<Point> vertices;
    for (int i = 0; i < 3; ++i) {
        double angle = 2 * 3.1415 * i / 3;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

// Вспомогательная функция для создания правильного шестиугольника
std::vector<Point> createRegularHexagon(Point center, double radius) {
    std::vector<Point> vertices;
    for (int i = 0; i < 6; ++i) {
        double angle = 2 * 3.1415 * i / 6;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

// Вспомогательная функция для создания правильного восьмиугольника
std::vector<Point> createRegularOctagon(Point center, double radius) {
    std::vector<Point> vertices;
    for (int i = 0; i < 8; ++i) {
        double angle = 2 * 3.1415 * i / 8;
        double x = center.x + radius * cos(angle);
        double y = center.y + radius * sin(angle);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

TEST(TriangleTest, ConstructorAndArea) {
    auto vertices = createRegularTriangle(Point(0, 0), 1.0);
    Triangle triangle(vertices);
    
    EXPECT_NEAR(triangle.area(), 0.5 * 3 * 1.0 * 1.0 * sin(2 * 3.1415 / 3), 0.01);
    
    Point center = triangle.center();
    EXPECT_NEAR(center.x, 0.0, 0.01);
    EXPECT_NEAR(center.y, 0.0, 0.01);
}

TEST(HexagonTest, ConstructorAndArea) {
    auto vertices = createRegularHexagon(Point(1, 1), 2.0);
    Hexagon hexagon(vertices);
    
    EXPECT_NEAR(hexagon.area(), 0.5 * 6 * 2.0 * 2.0 * sin(2 * 3.1415 / 6), 0.01);
    
    Point center = hexagon.center();
    EXPECT_NEAR(center.x, 1.0, 0.01);
    EXPECT_NEAR(center.y, 1.0, 0.01);
}

TEST(OctagonTest, ConstructorAndArea) {
    auto vertices = createRegularOctagon(Point(-1, -1), 1.5);
    Octagon octagon(vertices);
    
    EXPECT_NEAR(octagon.area(), 0.5 * 8 * 1.5 * 1.5 * sin(2 * 3.1415 / 8), 0.01);
    
    Point center = octagon.center();
    EXPECT_NEAR(center.x, -1.0, 0.01);
    EXPECT_NEAR(center.y, -1.0, 0.01);
}

TEST(FigureArrayTest, AddAndRemove) {
    FigureArray array;
    
    auto triangle_vertices = createRegularTriangle(Point(0, 0), 1.0);
    auto hexagon_vertices = createRegularHexagon(Point(1, 1), 2.0);
    
    array.addFigure(std::make_shared<Triangle>(triangle_vertices));
    array.addFigure(std::make_shared<Hexagon>(hexagon_vertices));
    
    EXPECT_EQ(array.size(), 2);
    
    array.removeFigure(0);
    EXPECT_EQ(array.size(), 1);
    
    array.removeFigure(0);
    EXPECT_EQ(array.size(), 0);
    
    // Тест удаления с неверным индексом
    testing::internal::CaptureStdout();
    array.removeFigure(5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Ошибка") != std::string::npos);
}

TEST(FigureArrayTest, TotalArea) {
    FigureArray array;
    
    auto triangle_vertices = createRegularTriangle(Point(0, 0), 1.0);
    auto hexagon_vertices = createRegularHexagon(Point(0, 0), 1.0);
    
    array.addFigure(std::make_shared<Triangle>(triangle_vertices));
    array.addFigure(std::make_shared<Hexagon>(hexagon_vertices));
    
    double total = array.totalArea();
    double expected = 0.5 * 3 * 1.0 * 1.0 * sin(2 * 3.1415 / 3) +
                     0.5 * 6 * 1.0 * 1.0 * sin(2 * 3.1415 / 6);
    
    EXPECT_NEAR(total, expected, 0.01);
}

TEST(OperatorOverloadTest, OutputOperator) {
    auto vertices = createRegularTriangle(Point(0, 0), 1.0);
    Triangle triangle(vertices);
    
    std::stringstream ss;
    ss << triangle;
    
    std::string output = ss.str();
    EXPECT_TRUE(output.find("Вершины") != std::string::npos);
    EXPECT_TRUE(output.find("Геом. центр") != std::string::npos);
    EXPECT_TRUE(output.find("Площадь") != std::string::npos);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
