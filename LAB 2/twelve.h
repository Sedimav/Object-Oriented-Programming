#ifndef TWELVE_H
#define TWELVE_H

#include <vector>
#include <string>
#include <initializer_list>

class Twelve {
private:
    std::vector<unsigned char> digits;
    
    void removeLeadingZeros();
    bool isValidDigit(unsigned char digit) const;
    int digitToValue(unsigned char digit) const;
    unsigned char valueToDigit(int value) const;

public:
    Twelve();
    Twelve(const size_t& n, unsigned char t = '0');
    Twelve(const std::initializer_list<unsigned char>& t);
    Twelve(const std::string& t);
    Twelve(const Twelve& other);
    Twelve(Twelve&& other) noexcept;
    ~Twelve() noexcept;
    
    Twelve& operator=(const Twelve& other);
    Twelve& operator=(Twelve&& other) noexcept;
    
    Twelve add(const Twelve& other) const;
    Twelve subtract(const Twelve& other) const;
    Twelve copy() const;
    
    bool greaterThan(const Twelve& other) const;
    bool lessThan(const Twelve& other) const;
    bool equals(const Twelve& other) const;
    
    Twelve addAssign(const Twelve& other);
    Twelve subtractAssign(const Twelve& other);
    
    std::string toString() const;
    size_t size() const;
    bool empty() const;
};

#endif
