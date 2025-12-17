#include "game.h"
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>

Game::Game() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, MAP_SIZE);
    
    for (int i = 0; i < BULL_COUNT; ++i) {
        std::string name = "Бык_" + std::to_string(i + 1);
        bulls.push_back(std::make_unique<Bull>(name, dis(gen), dis(gen)));
    }
}

Game::~Game() {
    stop();
}

int Game::rollD6() {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> dist(1, 6);
    return dist(gen);
}

double Game::randomDouble(double min, double max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gen);
}

void Game::movementLoop() {
    while (running.load()) {
        {
            std::unique_lock<std::shared_mutex> lock(bullsMutex);
            
            for (auto& bull : bulls) {
                if (!bull->isAlive()) continue;
                
                double angle = randomDouble(0, 2 * M_PI);
                double distance = randomDouble(0, Bull::MOVE_DISTANCE);
                
                double dx = distance * cos(angle);
                double dy = distance * sin(angle);
                
                double newX = bull->getX() + dx;
                double newY = bull->getY() + dy;
                
                if (newX < 0) newX = 0;
                if (newX > MAP_SIZE) newX = MAP_SIZE;
                if (newY < 0) newY = 0;
                if (newY > MAP_SIZE) newY = MAP_SIZE;
                
                bull->setPosition(newX, newY);
                
                for (auto& other : bulls) {
                    if (!other->isAlive() || other.get() == bull.get()) continue;
                    
                    double dist = sqrt(
                        pow(bull->getX() - other->getX(), 2) +
                        pow(bull->getY() - other->getY(), 2)
                    );
                    
                    if (dist <= Bull::ATTACK_RANGE) {
                        std::lock_guard<std::mutex> queueLock(queueMutex);
                        battleQueue.push({bull.get(), other.get()});
                        queueCV.notify_one();
                    }
                }
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void Game::processBattle(Bull* attacker, Bull* defender) {
    if (!attacker || !defender || !attacker->isAlive() || !defender->isAlive()) {
        return;
    }
    
    int attackRoll = rollD6();
    int defenseRoll = rollD6();
    
    {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << attacker->getName() << " атакует " << defender->getName()
                  << " (Атака: " << attackRoll << ", Защита: " << defenseRoll << ")" << std::endl;
    }
    
    if (attackRoll > defenseRoll) {
        defender->kill();
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << defender->getName() << " убит!" << std::endl;
    } else {
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << defender->getName() << " защитился!" << std::endl;
    }
}

void Game::battleLoop() {
    while (running.load() || !battleQueue.empty()) {
        Battle battle;
        
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [this]() {
                return !battleQueue.empty() || !running.load();
            });
            
            if (!running.load() && battleQueue.empty()) break;
            
            if (!battleQueue.empty()) {
                battle = battleQueue.front();
                battleQueue.pop();
            }
        }
        
        if (battle.attacker && battle.defender) {
            processBattle(battle.attacker, battle.defender);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void Game::printMap() const {
    const int GRID = 10;
    double cellSize = MAP_SIZE / GRID;
    
    std::vector<std::vector<int>> grid(GRID, std::vector<int>(GRID, 0));
    
    {
        std::shared_lock<std::shared_mutex> lock(bullsMutex);
        
        for (const auto& bull : bulls) {
            if (bull->isAlive()) {
                int gridX = std::min(GRID - 1,
                                   static_cast<int>(bull->getX() / cellSize));
                int gridY = std::min(GRID - 1,
                                   static_cast<int>(bull->getY() / cellSize));
                grid[gridY][gridX]++;
            }
        }
    }
    
    std::lock_guard<std::mutex> coutLock(coutMutex);
    
    std::cout << "\n" << std::string(30, '=') << std::endl;
    std::cout << "КАРТА 100x100" << std::endl;
    std::cout << "Живых быков: " << countAlive()
              << "/" << bulls.size() << std::endl;
    
    for (int y = 0; y < GRID; ++y) {
        for (int x = 0; x < GRID; ++x) {
            int count = grid[y][x];
            if (count == 0) {
                std::cout << ". ";
            } else if (count < 10) {
                std::cout << count << " ";
            } else {
                std::cout << "* ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::string(30, '=') << std::endl;
}

int Game::countAlive() const {
    std::shared_lock<std::shared_mutex> lock(bullsMutex);
    int count = 0;
    for (const auto& bull : bulls) {
        if (bull->isAlive()) count++;
    }
    return count;
}

void Game::printSurvivors() const {
    std::shared_lock<std::shared_mutex> lock(bullsMutex);
    
    std::lock_guard<std::mutex> coutLock(coutMutex);
    
    std::cout << "\n" << std::string(40, '=') << std::endl;
    std::cout << "ВЫЖИВШИЕ БЫКИ" << std::endl;
    std::cout << std::string(40, '=') << std::endl;
    
    int alive = 0;
    for (const auto& bull : bulls) {
        if (bull->isAlive()) {
            std::cout << "  • " << bull->getName() << " @("
                     << std::fixed << std::setprecision(1)
                     << bull->getX() << ", " << bull->getY() << ")" << std::endl;
            alive++;
        }
    }
    
    if (alive == 0) {
        std::cout << "  Никто не выжил..." << std::endl;
    }
    
    std::cout << "Всего выжило: " << alive << " из " << bulls.size() << std::endl;
}

void Game::start() {
    running.store(true);
    
    movementThread = std::thread([this]() { movementLoop(); });
    battleThread = std::thread([this]() { battleLoop(); });
    
    std::lock_guard<std::mutex> coutLock(coutMutex);
    std::cout << "Игра началась. Быков: " << bulls.size() << std::endl;
}

void Game::stop() {
    if (!running.load()) return;
    
    running.store(false);
    queueCV.notify_all();
    
    if (movementThread.joinable()) movementThread.join();
    if (battleThread.joinable()) battleThread.join();
    
    std::lock_guard<std::mutex> coutLock(coutMutex);
    std::cout << "Игра остановлена." << std::endl;
}

void Game::run(int seconds) {
    start();
    
    for (int i = 0; i < seconds; ++i) {
        printMap();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::lock_guard<std::mutex> coutLock(coutMutex);
        std::cout << "Прошло: " << i + 1 << "/" << seconds << " секунд" << std::endl;
    }
    
    stop();
    printSurvivors();
}
