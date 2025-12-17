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
    std::atomic<bool> gameOver{false};
    
    const int MAP_SIZE = 100;
    const int BULL_COUNT = 50;
    
    std::thread movementThread;
    std::thread battleThread;
    
    void movementLoop();
    void battleLoop();
    void processBattle(Bull* attacker, Bull* defender);
    
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
