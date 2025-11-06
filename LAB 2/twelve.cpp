#include "twelve.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>

Twelve::Twelve() : digits({'0'}) {}

Twelve::Twelve(const size_t& n, unsigned char t) {
    if (n == 0) {
        digits.push_back('0');
        return;
    }
    
    digits.resize(n);
    for (size_t i = 0; i < n; ++i) {
        if (!isValidDigit(t)) {
            throw std::invalid_argument("Цифра должна быть в диапазоне 0-9, A, B");
        }
        digits[i] = t;
    }
    removeLeadingZeros();
}

Twelve::Twelve(const std::initializer_list<unsigned char>& t) {
    if (t.size() == 0) {
        digits.push_back('0');
        return;
    }
    
    for (auto it = t.begin(); it != t.end(); ++it) {
        if (!isValidDigit(*it)) {
            throw std::invalid_argument("Цифра должна быть в диапазоне 0-9, A, B");
        }
    }
    
    digits.assign(t.begin(), t.end());
    removeLeadingZeros();
}

Twelve::Twelve(const std::string& t) {
    if (t.empty()) {
        digits.push_back('0');
        return;
    }
    
    for (char c : t) {
        if (!isValidDigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Символы должны быть в диапазоне 0-9, A, B");
        }
        digits.push_back(static_cast<unsigned char>(c));
    }
    removeLeadingZeros();
}

Twelve::Twelve(const Twelve& other) : digits(other.digits) {}

Twelve::Twelve(Twelve&& other) noexcept : digits(std::move(other.digits)) {
    other.digits = {'0'};
}

Twelve::~Twelve() noexcept {}

void Twelve::removeLeadingZeros() {
    while (digits.size() > 1 && digits[0] == '0') {
        digits.erase(digits.begin());
    }
}

bool Twelve::isValidDigit(unsigned char digit) const {
    return (digit >= '0' && digit <= '9') ||
           (digit == 'A' || digit == 'a') ||
           (digit == 'B' || digit == 'b');
}

int Twelve::digitToValue(unsigned char digit) const {
    if (digit >= '0' && digit <= '9') {
        return digit - '0';
    }
    if (digit == 'A' || digit == 'a') {
        return 10;
    }
    if (digit == 'B' || digit == 'b') {
        return 11;
    }
    throw std::invalid_argument("Неверная цифра для 12-ричной системы");
}

unsigned char Twelve::valueToDigit(int value) const {
    if (value < 0 || value > 11) {
        throw std::invalid_argument("Значение вне диапазона для 12-ричной системы");
    }
    if (value < 10) {
        return '0' + value;
    }
    if (value == 10) {
        return 'A';
    }
    return 'B';
}

Twelve Twelve::add(const Twelve& other) const {
    Twelve result = *this;
    return result.addAssign(other);
}

Twelve Twelve::subtract(const Twelve& other) const {
    Twelve result = *this;
    return result.subtractAssign(other);
}

Twelve Twelve::copy() const {
    return Twelve(*this);
}

bool Twelve::greaterThan(const Twelve& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() > other.digits.size();
    }
    
    for (size_t i = 0; i < digits.size(); ++i) {
        int val1 = digitToValue(digits[i]);
        int val2 = digitToValue(other.digits[i]);
        if (val1 != val2) {
            return val1 > val2;
        }
    }
    return false;
}

bool Twelve::lessThan(const Twelve& other) const {
    if (digits.size() != other.digits.size()) {
        return digits.size() < other.digits.size();
    }
    
    for (size_t i = 0; i < digits.size(); ++i) {
        int val1 = digitToValue(digits[i]);
        int val2 = digitToValue(other.digits[i]);
        if (val1 != val2) {
            return val1 < val2;
        }
    }
    return false;
}

bool Twelve::equals(const Twelve& other) const {
    if (digits.size() != other.digits.size()) {
        return false;
    }
    
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }
    return true;
}

Twelve Twelve::addAssign(const Twelve& other) {
    size_t maxSize = std::max(digits.size(), other.digits.size());
    std::vector<unsigned char> result(maxSize + 1, '0');
    
    int carry = 0;
    for (size_t i = 0; i < maxSize || carry; ++i) {
        int sum = carry;
        
        if (i < digits.size()) {
            sum += digitToValue(digits[digits.size() - 1 - i]);
        }
        
        if (i < other.digits.size()) {
            sum += digitToValue(other.digits[other.digits.size() - 1 - i]);
        }
        
        carry = sum / 12;
        result[result.size() - 1 - i] = valueToDigit(sum % 12);
    }
    
    digits = result;
    removeLeadingZeros();
    return *this;
}

Twelve Twelve::subtractAssign(const Twelve& other) {
    if (lessThan(other)) {
        throw std::invalid_argument("Нельзя вычесть большее число из меньшего");
    }
    
    if (equals(other)) {
        digits = {'0'};
        return *this;
    }
    
    std::vector<unsigned char> result = digits;
    int borrow = 0;
    
    for (size_t i = 0; i < other.digits.size() || borrow; ++i) {
        int top = digitToValue(result[result.size() - 1 - i]) - borrow;
        int bottom = (i < other.digits.size()) ? digitToValue(other.digits[other.digits.size() - 1 - i]) : 0;
        
        if (top < bottom) {
            borrow = 1;
            top += 12;
        } else {
            borrow = 0;
        }
        
        result[result.size() - 1 - i] = valueToDigit(top - bottom);
    }
    
    digits = result;
    removeLeadingZeros();
    return *this;
}

std::string Twelve::toString() const {
    std::string result;
    for (unsigned char digit : digits) {
        result += digit;
    }
    return result;
}

size_t Twelve::size() const {
    return digits.size();
}

bool Twelve::empty() const {
    return digits.empty() || (digits.size() == 1 && digits[0] == '0');
}

Twelve& Twelve::operator=(const Twelve& other) {
    if (this != &other) {
        digits = other.digits;
    }
    return *this;
}

Twelve& Twelve::operator=(Twelve&& other) noexcept {
    if (this != &other) {
        digits = std::move(other.digits);
        other.digits = {'0'};
    }
    return *this;
}
