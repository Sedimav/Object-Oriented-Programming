#include "func.h"
#include <iostream>

int main() {
    std::cout << " ДЕМОНСТРАЦИЯ ForwardList \n";
    DynamicListMemoryResource mr;
    ForwardList<int> int_list(&mr);
    for (int i = 1; i <= 4; ++i) {
        int_list.push_front(i * 5);
    }
    std::cout << "Целые числа: ";
    for (auto it = int_list.begin(); it != int_list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\nРазмер=" << int_list.size()
              << ", первый элемент=" << int_list.front() << "\n";
    ForwardList<Point> point_list(&mr);
    point_list.push_front(Point{0, 0, "O"});
    point_list.push_front(Point{1, 2, "A"});
    std::cout << "Points:\n";
    for (const auto& p : point_list) {
        std::cout << "  (" << p.x << ", " << p.y << ") \"" << p.name << "\"\n";
    }
    std::cout << "Память: выделено блоков=" << mr.allocated_count()
              << ", свободных блоков=" << mr.free_pool_count() << "\n";
    
    return 0;
}
