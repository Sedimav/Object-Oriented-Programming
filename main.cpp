#include <iostream>
#include <string>
#include <limits>
#include "turkish_number_converter.h"

int main() {
    std::cout << "=== Конвертер чисел в турецкий язык ===" << std::endl;
    std::cout << "Введите число от 0 до 99: ";
    
    int number;
    
    while (!(std::cin >> number)) {
        std::cout << "Ошибка: введите целое число: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    try {
        std::string turkish_name = convert_to_turkish(number);
        std::cout << number << " --> \"" << turkish_name << "\"" << std::endl;
        
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
