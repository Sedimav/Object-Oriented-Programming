#include "func.h"
#include <iostream>
#include <limits>

using namespace std;

void showMenu() {
    cout << "\n  РЕДАКТОР ПОДЗЕМЕЛЬЯ " << endl;
    cout << "1. Добавить NPC" << endl;
    cout << "2. Вывести всех NPC" << endl;
    cout << "3. Сохранить в файл" << endl;
    cout << "4. Загрузить из файла" << endl;
    cout << "5. Запустить боевой режим" << endl;
    cout << "6. Выход" << endl;
    cout << "Выберите действие: ";
}

void addNPCInteractive(DungeonEditor& редактор) {
    string тип, имя;
    double x, y;
    
    cout << "\n Добавление NPC " << endl;
    
    cout << "Доступные типы: Орк, Оборотень, Разбойник" << endl;
    cout << "Введите тип: ";
    cin >> тип;
    
    cout << "Введите уникальное имя: ";
    cin >> имя;
    
    cout << "Введите координату x (0-500): ";
    cin >> x;
    
    cout << "Введите координату y (0-500): ";
    cin >> y;
    
    редактор.addNPC(тип, имя, x, y);
}

void battleInteractive(DungeonEditor& редактор) {
    double дальность;
    cout << "\n Боевой режим " << endl;
    cout << "Введите дальность атаки: ";
    cin >> дальность;
    
    редактор.battle(дальность);
}

int main() {
    DungeonEditor редактор;
    редактор.addObserver(new ConsoleObserver());
    редактор.addObserver(new FileObserver("log.txt"));
    
    int выбор = 0;
    string имяФайла;
    
    do {
        showMenu();
        cin >> выбор;
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (выбор) {
            case 1:
                addNPCInteractive(редактор);
                break;
                
            case 2:
                редактор.printAll();
                break;
                
            case 3:
                cout << "Введите имя файла для сохранения: ";
                getline(cin, имяФайла);
                редактор.save(имяФайла);
                break;
                
            case 4:
                cout << "Введите имя файла для загрузки: ";
                getline(cin, имяФайла);
                редактор.load(имяФайла);
                break;
                
            case 5:
                battleInteractive(редактор);
                break;
                
            case 6:
                cout << "Выход..." << endl;
                break;
                
            default:
                cout << "Неверный выбор!" << endl;
                break;
        }
    } while (выбор != 6);
    
    return 0;
}
