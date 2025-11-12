#ifndef FIGURES_H
#define FIGURES_H

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <sstream>
#include <string>
#include <concepts>
#include <type_traits>

template<typename T>
concept Scalar = std::is_scalar_v<T>;

template<Scalar T>
class Point {
private:
    T x_, y_;
public:
    Point(T x = T(), T y = T()) : x_(x), y_(y) {}
    
    T x() const { return x_; }
    T y() const { return y_; }
    
    bool operator==(const Point<T>& other) const {
        if constexpr (std::is_integral_v<T>) {
            return x_ == other.x_ && y_ == other.y_;
        } else {
            return std::abs(x_ - other.x_) < 1e-9 && std::abs(y_ - other.y_) < 1e-9;
        }
    }
    
    friend std::ostream& operator<<(std::ostream& os, const Point<T>& point) {
        os << "(" << point.x_ << ", " << point.y_ << ")";
        return os;
    }
};

template<Scalar T>
class Figure {
public:
    virtual ~Figure() = default;
    virtual double area() const = 0;
    virtual void print(std::ostream& os) const = 0;
    virtual void read(std::istream& is) = 0;
    virtual Point<T> center() const = 0;
    
    explicit virtual operator double() const { return area(); }
    
    virtual bool operator==(const Figure<T>& other) const = 0;
    bool operator!=(const Figure<T>& other) const { return !(*this == other); }
};

template<Scalar T>
class RegularPolygon : public Figure<T> {
protected:
    std::vector<std::unique_ptr<Point<T>>> vertices;
    Point<T> center_;
    int sides = 0;

    void calculateCenter() {
        T sum_x = 0, sum_y = 0;
        for (const auto& vertex : vertices) {
            sum_x += vertex->x();
            sum_y += vertex->y();
        }
        center_ = Point<T>(sum_x / sides, sum_y / sides);
    }

public:
    RegularPolygon() = default;
    
    RegularPolygon(const std::vector<Point<T>>& points) {
        sides = points.size();
        for (const auto& point : points) {
            vertices.push_back(std::make_unique<Point<T>>(point));
        }
        calculateCenter();
    }
    
    double area() const override {
        if (vertices.size() < 3) return 0.0;
        
        double area = 0.0;
        for (size_t i = 0; i < vertices.size(); ++i) {
            size_t j = (i + 1) % vertices.size();
            area += static_cast<double>(vertices[i]->x()) * static_cast<double>(vertices[j]->y()) 
                   - static_cast<double>(vertices[j]->x()) * static_cast<double>(vertices[i]->y());
        }
        return std::abs(area) / 2.0;
    }
    
    void print(std::ostream& os) const override {
        os << "Вершины: ";
        for (const auto& vertex : vertices) {
            os << *vertex << " ";
        }
    }
    
    void read(std::istream& is) override {
        vertices.clear();
        std::string line;
        
        std::cout << "Введите " << sides << " вершин (x y для каждой):" << std::endl;
        
        for (int i = 0; i < sides; ++i) {
            std::cout << "Вершина " << (i + 1) << ": ";
            std::getline(is, line);
            
            std::istringstream iss(line);
            T x, y;
            iss >> x >> y;
            
            char extra;
            if (iss.fail() || iss.get(extra)) {
                std::cout << "Ошибка: для вершины нужно ровно два числа!" << std::endl;
                is.setstate(std::ios::failbit);
                return;
            }
            
            vertices.push_back(std::make_unique<Point<T>>(x, y));
        }
        calculateCenter();
    }
    
    Point<T> center() const override {
        return center_;
    }
    
    bool operator==(const Figure<T>& other) const override {
        const auto* otherPoly = dynamic_cast<const RegularPolygon<T>*>(&other);
        if (!otherPoly || vertices.size() != otherPoly->vertices.size()) return false;
        
        for (size_t i = 0; i < vertices.size(); ++i) {
            if (*vertices[i] != *otherPoly->vertices[i]) return false;
        }
        return true;
    }
};

template<Scalar T>
class Pentagon : public RegularPolygon<T> {
public:
    Pentagon() { this->sides = 5; }
    Pentagon(const std::vector<Point<T>>& vertices) : RegularPolygon<T>(vertices) {}
    
    void read(std::istream& is) override {
        std::cout << "~~~ Пятиугольник ~~~" << std::endl;
        this->sides = 5;
        RegularPolygon<T>::read(is);
    }
};

template<Scalar T>
class Hexagon : public RegularPolygon<T> {
public:
    Hexagon() { this->sides = 6; }
    Hexagon(const std::vector<Point<T>>& vertices) : RegularPolygon<T>(vertices) {}
    
    void read(std::istream& is) override {
        std::cout << "~~~ Шестиугольник ~~~" << std::endl;
        this->sides = 6;
        RegularPolygon<T>::read(is);
    }
};

template<Scalar T>
class Octagon : public RegularPolygon<T> {
public:
    Octagon() { this->sides = 8; }
    Octagon(const std::vector<Point<T>>& vertices) : RegularPolygon<T>(vertices) {}
    
    void read(std::istream& is) override {
        std::cout << "~~~ Восьмиугольник ~~~" << std::endl;
        this->sides = 8;
        RegularPolygon<T>::read(is);
    }
};

template<typename T>
class Array {
private:
    std::vector<std::shared_ptr<T>> data_;
    size_t capacity_ = 10;
    size_t size_ = 0;

    void resize(size_t new_capacity) {
        std::vector<std::shared_ptr<T>> new_data(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }

public:
    Array() {
        data_.resize(capacity_);
    }
    
    ~Array() = default;
    
    Array(const Array& other) : capacity_(other.capacity_), size_(other.size_) {
        data_.resize(capacity_);
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    Array(Array&& other) noexcept 
        : data_(std::move(other.data_)), capacity_(other.capacity_), size_(other.size_) {
        other.capacity_ = 0;
        other.size_ = 0;
    }
    
    Array& operator=(const Array& other) {
        if (this != &other) {
            capacity_ = other.capacity_;
            size_ = other.size_;
            data_.resize(capacity_);
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    
    Array& operator=(Array&& other) noexcept {
        if (this != &other) {
            data_ = std::move(other.data_);
            capacity_ = other.capacity_;
            size_ = other.size_;
            other.capacity_ = 0;
            other.size_ = 0;
        }
        return *this;
    }
    
    void add(std::shared_ptr<T> element) {
        if (size_ >= capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_++] = std::move(element);
    }
    
    void remove(size_t index) {
        if (index >= size_) return;
        
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = std::move(data_[i + 1]);
        }
        data_[--size_] = nullptr;
    }
    
    std::shared_ptr<T> operator[](size_t index) const {
        if (index >= size_) return nullptr;
        return data_[index];
    }
    
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    
    double totalArea() const {
        double total = 0.0;
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i]) {
                total += data_[i]->area();
            }
        }
        return total;
    }
    
    void printAll(std::ostream& os) const {
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i]) {
                os << "Фигура " << i << ": ";
                data_[i]->print(os);
                auto center = data_[i]->center();
                os << " | Геом. центр: " << center << " | Площадь: " << data_[i]->area() << std::endl;
            }
        }
    }
};

template<Scalar T>
class Array<Figure<T>*> {
private:
    std::vector<Figure<T>*> data_;
    size_t capacity_ = 10;
    size_t size_ = 0;

    void resize(size_t new_capacity) {
        std::vector<Figure<T>*> new_data(new_capacity, nullptr);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        data_ = std::move(new_data);
        capacity_ = new_capacity;
    }

public:
    Array() {
        data_.resize(capacity_, nullptr);
    }
    
    ~Array() {
        for (size_t i = 0; i < size_; ++i) {
            delete data_[i];
        }
    }
    
    void add(Figure<T>* element) {
        if (size_ >= capacity_) {
            resize(capacity_ * 2);
        }
        data_[size_++] = element;
    }
    
    void remove(size_t index) {
        if (index >= size_) return;
        
        delete data_[index];
        for (size_t i = index; i < size_ - 1; ++i) {
            data_[i] = data_[i + 1];
        }
        data_[--size_] = nullptr;
    }
    
    Figure<T>* operator[](size_t index) const {
        if (index >= size_) return nullptr;
        return data_[index];
    }
    
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    
    double totalArea() const {
        double total = 0.0;
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i]) {
                total += static_cast<double>(*data_[i]);
            }
        }
        return total;
    }
    
    void printAll(std::ostream& os) const {
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i]) {
                os << "Фигура " << i << ": ";
                data_[i]->print(os);
                auto center = data_[i]->center();
                os << " | Геом. центр: " << center << " | Площадь: " << data_[i]->area() << std::endl;
            }
        }
    }
};

template<Scalar T>
std::ostream& operator<<(std::ostream& os, const Figure<T>& figure) {
    figure.print(os);
    auto center = figure.center();
    os << " | Геом. центр: " << center << " | Площадь: " << figure.area();
    return os;
}

template<Scalar T>
std::istream& operator>>(std::istream& is, Figure<T>& figure) {
    figure.read(is);
    return is;
}

void showMenu() {
    std::cout << "\n=== МЕНЮ ===" << std::endl;
    std::cout << "1. Добавить пятиугольник (int)" << std::endl;
    std::cout << "2. Добавить шестиугольник (int)" << std::endl;
    std::cout << "3. Добавить восьмиугольник (int)" << std::endl;
    std::cout << "4. Добавить пятиугольник (double)" << std::endl;
    std::cout << "5. Добавить шестиугольник (double)" << std::endl;
    std::cout << "6. Добавить восьмиугольник (double)" << std::endl;
    std::cout << "7. Показать все фигуры" << std::endl;
    std::cout << "8. Вычислить общую площадь" << std::endl;
    std::cout << "9. Удалить фигуру по индексу" << std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << "Выберите опцию: ";
}

#endif
