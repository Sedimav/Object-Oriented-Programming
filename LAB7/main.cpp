#include "game.h"
#include <iostream>

int main() {
    std::cout << "Параметры:" << std::endl;
    std::cout << "  Дистанция перемещения: 30" << std::endl;
    std::cout << "  Дистанция атаки: 10" << std::endl;
    std::cout << "  Карта: 100x100" << std::endl;
    std::cout << "  Быков: 50" << std::endl;
    std::cout << "  Время: 30 секунд" << std::endl;
    
    std::cout << "\nНажмите Enter для запуска...";
    std::cin.get();
    
    Game game;
    game.run(30);
    
    std::cout << "        Игра завершена!" << std::endl;
    
    return 0;
}
