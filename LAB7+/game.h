#ifndef GAME_H
#define GAME_H

#include "bull.h"
#include <memory>
#include <vector>
#include <mutex>
#include <shared_mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>

#include <cppcoro/task.hpp>

class Game {
private:
    std::vector<std::unique_ptr<Bull>> bulls;
    mutable std::shared_mutex bullsMutex;
    
    struct Battle {
        Bull* attacker;
        Bull* defender;
    };
    std::queue<Battle> battleQueue;
    mutable std::mutex queueMutex;
    std::condition_variable queueCV;
    
    mutable std::mutex coutMutex;
    
    std::atomic<bool> running{false};
    
    const int MAP_SIZE = 100;
    const int BULL_COUNT = 50;
    
    std::thread movementThread;
    
    cppcoro::task<> moveBull(Bull* bull);
    
    cppcoro::task<> battleCoroutine();
    
    cppcoro::task<> gameLoop();
    
    void movementLoop();
    void battleLoop() {}
    
public:
    Game();
    ~Game();
    
    void start();
    void stop();
    void run(int seconds);
    void printMap() const;
    void printSurvivors() const;
    int countAlive() const;
    
    static int rollD6();
    static double randomDouble(double min, double max);
};

#endif
