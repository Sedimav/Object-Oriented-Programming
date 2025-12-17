#include "bull.h"

Bull::Bull(const std::string& name, double x, double y)
    : name(name), x(x), y(y), alive(true) {}

std::string Bull::getName() const {
    std::lock_guard<std::mutex> lock(mutex);
    return name;
}

double Bull::getX() const {
    std::lock_guard<std::mutex> lock(mutex);
    return x;
}

double Bull::getY() const {
    std::lock_guard<std::mutex> lock(mutex);
    return y;
}

bool Bull::isAlive() const {
    return alive.load();
}

void Bull::kill() {
    alive.store(false);
}

void Bull::move(double dx, double dy) {
    if (!alive.load()) return;
    
    std::lock_guard<std::mutex> lock(mutex);
    x += dx;
    y += dy;
}

void Bull::setPosition(double newX, double newY) {
    if (!alive.load()) return;
    
    std::lock_guard<std::mutex> lock(mutex);
    x = newX;
    y = newY;
}
