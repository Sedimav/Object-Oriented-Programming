#include "figures.c++"
#include <gtest/gtest.h>
#include <sstream>
#include <vector>
#include <cmath>

template<Scalar T>
std::vector<Point<T>> createRegularPolygon(Point<T> center, T radius, int sides) {
    std::vector<Point<T>> vertices;
    for (int i = 0; i < sides; ++i) {
        double angle = 2 * 3.1415 * i / sides;
        T x = center.x() + radius * cos(angle);
        T y = center.y() + radius * sin(angle);
        vertices.push_back(Point<T>(x, y));
    }
    return vertices;
}

TEST(TemplateTest, PointCreation) {
    Point<int> p1(5, 10);
    EXPECT_EQ(p1.x(), 5);
    EXPECT_EQ(p1.y(), 10);
    
    Point<double> p2(3.14, 2.71);
    EXPECT_DOUBLE_EQ(p2.x(), 3.14);
    EXPECT_DOUBLE_EQ(p2.y(), 2.71);
}

TEST(TemplateTest, PentagonArea) {
    auto vertices = createRegularPolygon<double>(Point<double>(0, 0), 1.0, 5);
    Pentagon<double> pentagon(vertices);
    
    EXPECT_GT(pentagon.area(), 0);
    Point<double> center = pentagon.center();
    EXPECT_NEAR(center.x(), 0.0, 0.01);
    EXPECT_NEAR(center.y(), 0.0, 0.01);
}

TEST(TemplateTest, HexagonArea) {
    auto vertices = createRegularPolygon<int>(Point<int>(0, 0), 10, 6);
    Hexagon<int> hexagon(vertices);
    
    EXPECT_GT(hexagon.area(), 0);
    Point<int> center = hexagon.center();
    EXPECT_NEAR(center.x(), 0, 1);
    EXPECT_NEAR(center.y(), 0, 1);
}

TEST(TemplateTest, ArrayOperations) {
    Array<Figure<int>> array;
    
    auto vertices = createRegularPolygon<int>(Point<int>(0, 0), 10, 5);
    auto pentagon = std::make_shared<Pentagon<int>>(vertices);
    
    array.add(pentagon);
    EXPECT_EQ(array.size(), 1);
    
    array.remove(0);
    EXPECT_EQ(array.size(), 0);
}

TEST(TemplateTest, MoveSemantics) {
    Array<Figure<double>> array1;
    auto vertices = createRegularPolygon<double>(Point<double>(0, 0), 1.0, 6);
    auto hexagon = std::make_shared<Hexagon<double>>(vertices);
    array1.add(hexagon);
    
    Array<Figure<double>> array2 = std::move(array1);
    EXPECT_EQ(array2.size(), 1);
    EXPECT_EQ(array1.size(), 0);
}

TEST(TemplateTest, TotalArea) {
    Array<Figure<double>> array;
    
    auto vertices1 = createRegularPolygon<double>(Point<double>(0, 0), 1.0, 5);
    auto vertices2 = createRegularPolygon<double>(Point<double>(0, 0), 1.0, 6);
    
    array.add(std::make_shared<Pentagon<double>>(vertices1));
    array.add(std::make_shared<Hexagon<double>>(vertices2));
    
    double total = array.totalArea();
    EXPECT_GT(total, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
