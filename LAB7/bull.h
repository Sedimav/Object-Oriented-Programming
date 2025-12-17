#ifndef BULL_H
#define BULL_H

#include <string>
#include <atomic>
#include <mutex>

class Bull {
private:
    std::string name;
    double x, y;
    std::atomic<bool> alive;
    mutable std::mutex mutex;
    
public:
    Bull(const std::string& name, double x, double y);
    
    std::string getName() const;
    double getX() const;
    double getY() const;
    bool isAlive() const;
    void kill();
    
    void move(double dx, double dy);
    void setPosition(double newX, double newY);
    
    static constexpr double MOVE_DISTANCE = 30.0;
    static constexpr double ATTACK_RANGE = 10.0;
};

#endif
