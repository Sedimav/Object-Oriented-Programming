#include "figures.c++"
#include <iostream>
#include <memory>
#include <limits>

int main() {
    Array<Pentagon<int>> pentagons_int;
    Array<Hexagon<int>> hexagons_int;
    Array<Octagon<int>> octagons_int;
    Array<Pentagon<double>> pentagons_double;
    Array<Hexagon<double>> hexagons_double;
    Array<Octagon<double>> octagons_double;
    
    int choice;
    
    do {
        showMenu();
        std::cin >> choice;
        std::cin.ignore();
        
        switch (choice) {
            case 1: {
                auto pentagon = std::make_shared<Pentagon<int>>();
                std::cin >> *pentagon;
                if (!std::cin.fail()) {
                    pentagons_int.add(pentagon);
                    std::cout << "Пятиугольник (int) добавлен" << std::endl;
                } else {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cin.clear();
                }
                break;
            }
            case 2: {
                auto hexagon = std::make_shared<Hexagon<int>>();
                std::cin >> *hexagon;
                if (!std::cin.fail()) {
                    hexagons_int.add(hexagon);
                    std::cout << "Шестиугольник (int) добавлен" << std::endl;
                } else {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cin.clear();
                }
                break;
            }
            case 3: {
                auto octagon = std::make_shared<Octagon<int>>();
                std::cin >> *octagon;
                if (!std::cin.fail()) {
                    octagons_int.add(octagon);
                    std::cout << "Восьмиугольник (int) добавлен" << std::endl;
                } else {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cin.clear();
                }
                break;
            }
            case 4: {
                auto pentagon = std::make_shared<Pentagon<double>>();
                std::cin >> *pentagon;
                if (!std::cin.fail()) {
                    pentagons_double.add(pentagon);
                    std::cout << "Пятиугольник (double) добавлен" << std::endl;
                } else {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cin.clear();
                }
                break;
            }
            case 5: {
                auto hexagon = std::make_shared<Hexagon<double>>();
                std::cin >> *hexagon;
                if (!std::cin.fail()) {
                    hexagons_double.add(hexagon);
                    std::cout << "Шестиугольник (double) добавлен" << std::endl;
                } else {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cin.clear();
                }
                break;
            }
            case 6: {
                auto octagon = std::make_shared<Octagon<double>>();
                std::cin >> *octagon;
                if (!std::cin.fail()) {
                    octagons_double.add(octagon);
                    std::cout << "Восьмиугольник (double) добавлен" << std::endl;
                } else {
                    std::cout << "Ошибка ввода" << std::endl;
                    std::cin.clear();
                }
                break;
            }
            case 7: {
                std::cout << "\n=== Пятиугольники int ===" << std::endl;
                pentagons_int.printAll(std::cout);
                std::cout << "\n=== Шестиугольники int ===" << std::endl;
                hexagons_int.printAll(std::cout);
                std::cout << "\n=== Восьмиугольники int ===" << std::endl;
                octagons_int.printAll(std::cout);
                std::cout << "\n=== Пятиугольники double ===" << std::endl;
                pentagons_double.printAll(std::cout);
                std::cout << "\n=== Шестиугольники double ===" << std::endl;
                hexagons_double.printAll(std::cout);
                std::cout << "\n=== Восьмиугольники double ===" << std::endl;
                octagons_double.printAll(std::cout);
                break;
            }
            case 8: {
                double total = 0.0;
                
                for (size_t i = 0; i < pentagons_int.size(); ++i) {
                    if (auto fig = pentagons_int[i]) total += fig->area();
                }
                for (size_t i = 0; i < hexagons_int.size(); ++i) {
                    if (auto fig = hexagons_int[i]) total += fig->area();
                }
                for (size_t i = 0; i < octagons_int.size(); ++i) {
                    if (auto fig = octagons_int[i]) total += fig->area();
                }
                for (size_t i = 0; i < pentagons_double.size(); ++i) {
                    if (auto fig = pentagons_double[i]) total += fig->area();
                }
                for (size_t i = 0; i < hexagons_double.size(); ++i) {
                    if (auto fig = hexagons_double[i]) total += fig->area();
                }
                for (size_t i = 0; i < octagons_double.size(); ++i) {
                    if (auto fig = octagons_double[i]) total += fig->area();
                }
                
                std::cout << "Общая площадь всех фигур: " << total << std::endl;
                break;
            }
            case 9: {
                std::cout << "Выберите тип фигуры для удаления:" << std::endl;
                std::cout << "1. Пятиугольник int" << std::endl;
                std::cout << "2. Шестиугольник int" << std::endl;
                std::cout << "3. Восьмиугольник int" << std::endl;
                std::cout << "4. Пятиугольник double" << std::endl;
                std::cout << "5. Шестиугольник double" << std::endl;
                std::cout << "6. Восьмиугольник double" << std::endl;
                std::cout << "Выберите тип: ";
                int type;
                std::cin >> type;
                std::cout << "Введите индекс: ";
                size_t index;
                std::cin >> index;
                
                switch (type) {
                    case 1:
                        if (index < pentagons_int.size()) {
                            pentagons_int.remove(index);
                            std::cout << "Пятиугольник int удален" << std::endl;
                        } else {
                            std::cout << "Неверный индекс" << std::endl;
                        }
                        break;
                    case 2:
                        if (index < hexagons_int.size()) {
                            hexagons_int.remove(index);
                            std::cout << "Шестиугольник int удален" << std::endl;
                        } else {
                            std::cout << "Неверный индекс" << std::endl;
                        }
                        break;
                    case 3:
                        if (index < octagons_int.size()) {
                            octagons_int.remove(index);
                            std::cout << "Восьмиугольник int удален" << std::endl;
                        } else {
                            std::cout << "Неверный индекс" << std::endl;
                        }
                        break;
                    case 4:
                        if (index < pentagons_double.size()) {
                            pentagons_double.remove(index);
                            std::cout << "Пятиугольник double удален" << std::endl;
                        } else {
                            std::cout << "Неверный индекс" << std::endl;
                        }
                        break;
                    case 5:
                        if (index < hexagons_double.size()) {
                            hexagons_double.remove(index);
                            std::cout << "Шестиугольник double удален" << std::endl;
                        } else {
                            std::cout << "Неверный индекс" << std::endl;
                        }
                        break;
                    case 6:
                        if (index < octagons_double.size()) {
                            octagons_double.remove(index);
                            std::cout << "Восьмиугольник double удален" << std::endl;
                        } else {
                            std::cout << "Неверный индекс" << std::endl;
                        }
                        break;
                    default:
                        std::cout << "Неверный тип" << std::endl;
                }
                break;
            }
            case 0: {
                std::cout << "Выход" << std::endl;
                break;
            }
            default: {
                std::cout << "Неверная опция" << std::endl;
                break;
            }
        }
        
    } while (choice != 0);
    
    return 0;
}
