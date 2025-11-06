#include "twelve.h"
#include <iostream>
#include <string>

Twelve read(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::cin >> input;
        try {
            return Twelve(input);
        } catch (const std::exception& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
            std::cout << "Пожалуйста, введите корректное 12-ричное число (цифры 0-9, A, B): ";
        }
    }
}

void demonstrate() {
    try {
        Twelve num1 = read("Введите первое 12-ричное число: ");
        Twelve num2 = read("Введите второе 12-ричное число: ");
        std::cout << "\nВведенные числа:" << std::endl;
        std::cout << "Число 1: " << num1.toString() << std::endl;
        std::cout << "Число 2: " << num2.toString() << std::endl;
        
        int choice;
        std::cout << "\nВыберите операцию:" << std::endl;
        std::cout << "1 - Сложение" << std::endl;
        std::cout << "2 - Вычитание" << std::endl;
        std::cout << "3 - Сравнение" << std::endl;
        std::cout << "4 - Все операции" << std::endl;
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        std::cout << "\nРезультаты:" << std::endl;
        
        switch (choice) {
            case 1: {
                Twelve sum = num1.add(num2);
                std::cout << num1.toString() << " + " << num2.toString()
                          << " = " << sum.toString() << std::endl;
                break;
            }
            case 2: {
                try {
                    Twelve diff = num1.subtract(num2);
                    std::cout << num1.toString() << " - " << num2.toString()
                              << " = " << diff.toString() << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Ошибка вычитания: " << e.what() << std::endl;
                }
                break;
            }
            case 3: {
                std::cout << num1.toString() << " > " << num2.toString() << " : "
                          << (num1.greaterThan(num2) ? "true" : "false") << std::endl;
                std::cout << num1.toString() << " < " << num2.toString() << " : "
                          << (num1.lessThan(num2) ? "true" : "false") << std::endl;
                std::cout << num1.toString() << " == " << num2.toString() << " : "
                          << (num1.equals(num2) ? "true" : "false") << std::endl;
                break;
            }
            case 4: {
                Twelve sum = num1.add(num2);
                std::cout << "Сложение: " << num1.toString() << " + " << num2.toString()
                          << " = " << sum.toString() << std::endl;
                
                try {
                    Twelve diff = num1.subtract(num2);
                    std::cout << "Вычитание: " << num1.toString() << " - " << num2.toString()
                              << " = " << diff.toString() << std::endl;
                } catch (const std::exception& e) {
                    std::cout << "Вычитание: " << e.what() << std::endl;
                }
                
                std::cout << "Сравнение:" << std::endl;
                std::cout << "  " << num1.toString() << " > " << num2.toString() << " : "
                          << (num1.greaterThan(num2) ? "true" : "false") << std::endl;
                std::cout << "  " << num1.toString() << " < " << num2.toString() << " : "
                          << (num1.lessThan(num2) ? "true" : "false") << std::endl;
                std::cout << "  " << num1.toString() << " == " << num2.toString() << " : "
                          << (num1.equals(num2) ? "true" : "false") << std::endl;
                
                Twelve copy1 = num1.copy();
                copy1.addAssign(num2);
                std::cout << "Сложение с присваиванием: " << num1.toString() << " += " << num2.toString()
                          << " = " << copy1.toString() << std::endl;
                break;
            }
            default:
                std::cout << "Неверный выбор операции!" << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Произошла ошибка: " << e.what() << std::endl;
    }
}

int main() {
    demonstrate();
    return 0;
}
