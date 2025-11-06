#include "figures.c++"
#include <iostream>
#include <memory>
#include <limits>

int main() {
    FigureArray figures;
    int choice;
    
    do {
        showMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: {
                auto triangle = std::make_shared<Triangle>();
                std::cin >> *triangle;
                
                if (std::cin.fail()) {
                    std::cout << "Ошибка ввода. Фигура не добавлена" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    figures.addFigure(triangle);
                    std::cout << "Треугольник успешно добавлен" << std::endl;
                }
                break;
            }
            case 2: {
                auto hexagon = std::make_shared<Hexagon>();
                std::cin >> *hexagon;
                if (std::cin.fail()) {
                    std::cout << "Ошибка ввода. Фигура не добавлена" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    figures.addFigure(hexagon);
                    std::cout << "Шестиугольник успешно добавлен" << std::endl;
                }
                break;
            }
            case 3: {
                auto octagon = std::make_shared<Octagon>();
                std::cin >> *octagon;
                if (std::cin.fail()) {
                    std::cout << "Ошибка ввода. Фигура не добавлена" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                } else {
                    figures.addFigure(octagon);
                    std::cout << "Восьмиугольник успешно добавлен" << std::endl;
                }
                break;
            }
            case 4: {
                if (figures.size() == 0) {
                    std::cout << "В массиве нет фигур." << std::endl;
                } else {
                    std::cout << "\n~~~ Все фигуры ~~~" << std::endl;
                    figures.printAll();
                }
                break;
            }
            case 5: {
                double total = figures.totalArea();
                std::cout << "Общая площадь всех фигур: " << total << std::endl;
                break;
            }
            case 6: {
                if (figures.size() == 0) {
                    std::cout << "Нет фигур для удаления" << std::endl;
                } else {
                    std::cout << "Введите индекс для удаления (0-" << figures.size()-1 << "): ";
                    int index;
                    std::cin >> index;
                    if (index >= 0 && index < static_cast<int>(figures.size())) {
                        figures.removeFigure(index);
                        std::cout << "Фигура удалена" << std::endl;
                    } else {
                        std::cout << "Ошибка: неверный индекс" << std::endl;
                    }
                }
                break;
            }
            case 0: {
                std::cout << "Выход из программы." << std::endl;
                break;
            }
            default: {
                std::cout << "Неверная опция! Попробуйте снова." << std::endl;
                break;
            }
        }
        
    } while (choice != 0);
    
    return 0;
}
