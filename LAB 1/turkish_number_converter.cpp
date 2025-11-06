#include "turkish_number_converter.h"
#include <string>
#include <stdexcept>

std::string convert_to_turkish(int number) {
    if (number < 0 || number > 99) {
        throw std::out_of_range("Число должно быть от 0 до 99");
    }

    const std::string units[] = {
        "sıfır", "bir", "iki", "üç", "dört", 
        "beş", "altı", "yedi", "sekiz", "dokuz"
    };

    const std::string tens[] = {
        "", "on", "yirmi", "otuz", "kırk", 
        "elli", "altmış", "yetmiş", "seksen", "doksan"
    };

    if (number == 0) {
        return units[0];
    }

    int tens_digit = number / 10;
    int units_digit = number % 10;

    std::string result;

    if (tens_digit > 0) {
        result += tens[tens_digit];
    }

    if (units_digit > 0) {
        if (!result.empty()) {
            result += " ";
        }
        result += units[units_digit];
    }

    return result;
}
