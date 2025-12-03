#include "func.h"
#include <new>

using namespace std;

Ork::Ork(const string& name, double x, double y) : name(name), x(x), y(y) {}
string Ork::getType() const { return "Орк"; }
string Ork::getName() const { return name; }
double Ork::getX() const { return x; }
double Ork::getY() const { return y; }

bool Ork::canBeat(NPC* other) const {
    return other->getType() == "Разбойник";
}

void Ork::accept(Visitor& visitor) {
    visitor.visit(this);
}

void Ork::print() const {
    cout << "Орк '" << name << "' в (" << x << ", " << y << ")" << endl;
}

string Ork::saveString() const {
    return "Орк " + name + " " + to_string(x) + " " + to_string(y);
}

Guard::Guard(const string& name, double x, double y) : name(name), x(x), y(y) {}
string Guard::getType() const { return "Оборотень"; }
string Guard::getName() const { return name; }
double Guard::getX() const { return x; }
double Guard::getY() const { return y; }

bool Guard::canBeat(NPC* other) const {
    return other->getType() == "Разбойник";
}

void Guard::accept(Visitor& visitor) {
    visitor.visit(this);
}

void Guard::print() const {
    cout << "Оборотень '" << name << "' в (" << x << ", " << y << ")" << endl;
}

string Guard::saveString() const {
    return "Оборотень " + name + " " + to_string(x) + " " + to_string(y);
}

Bandit::Bandit(const string& name, double x, double y) : name(name), x(x), y(y) {}
string Bandit::getType() const { return "Разбойник"; }
string Bandit::getName() const { return name; }
double Bandit::getX() const { return x; }
double Bandit::getY() const { return y; }

bool Bandit::canBeat(NPC* other) const {
    return other->getType() == "Оборотень";
}

void Bandit::accept(Visitor& visitor) {
    visitor.visit(this);
}

void Bandit::print() const {
    cout << "Разбойник '" << name << "' в (" << x << ", " << y << ")" << endl;
}

string Bandit::saveString() const {
    return "Разбойник " + name + " " + to_string(x) + " " + to_string(y);
}

unique_ptr<NPC> NPCFactory::create(const string& type,
                                   const string& name,
                                   double x, double y) {
    if (type == "Орк") return make_unique<Ork>(name, x, y);
    if (type == "Оборотень") return make_unique<Guard>(name, x, y);
    if (type == "Разбойник") return make_unique<Bandit>(name, x, y);
    return nullptr;
}

unique_ptr<NPC> NPCFactory::load(const string& data) {
    istringstream iss(data);
    string type, name;
    double x, y;
    
    if (iss >> type >> name >> x >> y) {
        return create(type, name, x, y);
    }
    return nullptr;
}

void ConsoleObserver::update(const string& message) {
    cout << "[Журнал] " << message << endl;
}

FileObserver::FileObserver(const string& filename) {
    file.open(filename, ios::app);
}

FileObserver::~FileObserver() {
    if (file.is_open()) file.close();
}

void FileObserver::update(const string& message) {
    if (file.is_open()) {
        file << message << endl;
    }
}

void Observable::addObserver(Observer* observer) {
    observers.push_back(observer);
}

void Observable::removeObserver(Observer* observer) {
    auto it = find(observers.begin(), observers.end(), observer);
    if (it != observers.end()) observers.erase(it);
}

void Observable::notify(const string& message) {
    for (auto observer : observers) {
        observer->update(message);
    }
}

BattleVisitor::BattleVisitor(Observable* observable, double range)
    : observable(observable), range(range), attacker(nullptr) {}

void BattleVisitor::setAttacker(NPC* a) {
    attacker = a;
}

bool BattleVisitor::inRange(NPC* a, NPC* b) {
    double dx = a->getX() - b->getX();
    double dy = a->getY() - b->getY();
    return sqrt(dx*dx + dy*dy) <= range;
}

void BattleVisitor::visit(Ork* ork) {
    if (attacker && inRange(attacker, ork)) {
        fight(attacker, ork);
    }
}

void BattleVisitor::visit(Guard* guard) {
    if (attacker && inRange(attacker, guard)) {
        fight(attacker, guard);
    }
}

void BattleVisitor::visit(Bandit* bandit) {
    if (attacker && inRange(attacker, bandit)) {
        fight(attacker, bandit);
    }
}

void BattleVisitor::fight(NPC* a, NPC* b) {
    string msg = a->getName() + " сражается с " + b->getName();
    observable->notify(msg);
    
    bool aWins = a->canBeat(b);
    bool bWins = b->canBeat(a);
    
    if (aWins && !bWins) {
        observable->notify(a->getName() + " побеждает " + b->getName());
    } else if (!aWins && bWins) {
        observable->notify(b->getName() + " побеждает " + a->getName());
    } else if (aWins && bWins) {
        observable->notify("Оба погибают: " + a->getName() + " и " + b->getName());
    }
}

bool DungeonEditor::validCoords(double x, double y) {
    return x >= 0 && x <= 500 && y >= 0 && y <= 500;
}

bool DungeonEditor::isNameUnique(const string& name) const {
    for (const auto& npc : npcs) {
        if (npc->getName() == name) return false;
    }
    return true;
}

void DungeonEditor::addNPC(const string& type,
                           const string& name,
                           double x, double y) {
    if (!validCoords(x, y)) {
        cout << "[Журнал] Некорректные координаты для " + name + " (" +
               to_string(x) + ", " + to_string(y) + ")" << endl;
        return;
    }
    
    if (!isNameUnique(name)) {
        cout << "[Журнал] Имя '" + name + "' уже используется!" << endl;
        return;
    }
    
    auto npc = NPCFactory::create(type, name, x, y);
    if (npc) {
        npcs.push_back(std::move(npc));
        notify("Добавлен " + type + " '" + name + "' в (" +
               to_string(x) + ", " + to_string(y) + ")");
    }
}

void DungeonEditor::printAll() const {
    cout << "\n NPC в подземелье " << endl;
    cout << "Всего: " << npcs.size() << endl;
    for (const auto& npc : npcs) {
        npc->print();
    }
}

void DungeonEditor::save(const string& filename) const {
    ofstream file(filename);
    if (!file) {
        cout << "[Журнал] Не удалось открыть файл: " + filename << endl;
        return;
    }
    
    for (const auto& npc : npcs) {
        file << npc->saveString() << endl;
    }
    
    file.close();
    cout << "[Журнал] Сохранено " + to_string(npcs.size()) + " NPC в " + filename << endl;
}

void DungeonEditor::load(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "[Журнал] Не удалось открыть файл: " + filename << endl;
        return;
    }
    
    npcs.clear();
    string line;
    int count = 0;
    
    while (getline(file, line)) {
        auto npc = NPCFactory::load(line);
        if (npc) {
            npcs.push_back(std::move(npc));
            count++;
        }
    }
    
    file.close();
    cout << "[Журнал] Загружено " + to_string(count) + " NPC из " + filename << endl;
}

void DungeonEditor::battle(double range) {
    notify(" НАЧАЛО БИТВЫ (дальность: " + to_string(range) + ") ===");
    notify("NPC до битвы: " + to_string(npcs.size()));
    
    BattleVisitor visitor(this, range);
    vector<bool> dead(npcs.size(), false);
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        if (dead[i]) continue;
        
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (dead[j]) continue;
            
            double distance = sqrt(pow(npcs[i]->getX() - npcs[j]->getX(), 2) +
                                  pow(npcs[i]->getY() - npcs[j]->getY(), 2));
            
            if (distance <= range) {
                visitor.setAttacker(npcs[i].get());
                npcs[j]->accept(visitor);
                
                bool iWins = npcs[i]->canBeat(npcs[j].get());
                bool jWins = npcs[j]->canBeat(npcs[i].get());
                
                if (iWins && !jWins) dead[j] = true;
                else if (!iWins && jWins) dead[i] = true;
                else if (iWins && jWins) {
                    dead[i] = true;
                    dead[j] = true;
                }
            }
        }
    }
    
    size_t oldSize = npcs.size();
    for (int i = npcs.size() - 1; i >= 0; --i) {
        if (dead[i]) npcs.erase(npcs.begin() + i);
    }
    
    notify("NPC после битвы: " + to_string(npcs.size()));
    notify("Убито: " + to_string(oldSize - npcs.size()));
}

size_t DungeonEditor::count() const {
    return npcs.size();
}

void* SimpleMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    (void)alignment;
    
    for (auto it = freePool.begin(); it != freePool.end(); ++it) {
        if (it->size >= bytes) {
            void* ptr = it->ptr;
            allocated.push_back({ptr, bytes});
            freePool.erase(it);
            return ptr;
        }
    }
    
    void* ptr = ::operator new(bytes);
    allocated.push_back({ptr, bytes});
    return ptr;
}

void SimpleMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    (void)alignment;
    
    for (auto it = allocated.begin(); it != allocated.end(); ++it) {
        if (it->ptr == p && it->size == bytes) {
            freePool.push_back({p, bytes});
            allocated.erase(it);
            return;
        }
    }
    
    ::operator delete(p);
}

bool SimpleMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

SimpleMemoryResource::~SimpleMemoryResource() {
    for (auto& block : allocated) ::operator delete(block.ptr);
    for (auto& block : freePool) ::operator delete(block.ptr);
}
