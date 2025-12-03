#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory_resource>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class NPC {
public:
    virtual ~NPC() = default;
    virtual std::string getType() const = 0;
    virtual std::string getName() const = 0;
    virtual double getX() const = 0;
    virtual double getY() const = 0;
    virtual bool canBeat(NPC* other) const = 0;
    virtual void accept(class Visitor& visitor) = 0;
    virtual void print() const = 0;
    virtual std::string saveString() const = 0;
};

class Ork : public NPC {
    std::string name;
    double x, y;
public:
    Ork(const std::string& name, double x, double y);
    std::string getType() const override;
    std::string getName() const override;
    double getX() const override;
    double getY() const override;
    bool canBeat(NPC* other) const override;
    void accept(class Visitor& visitor) override;
    void print() const override;
    std::string saveString() const override;
};

class Guard : public NPC {
    std::string name;
    double x, y;
public:
    Guard(const std::string& name, double x, double y);
    std::string getType() const override;
    std::string getName() const override;
    double getX() const override;
    double getY() const override;
    bool canBeat(NPC* other) const override;
    void accept(class Visitor& visitor) override;
    void print() const override;
    std::string saveString() const override;
};

class Bandit : public NPC {
    std::string name;
    double x, y;
public:
    Bandit(const std::string& name, double x, double y);
    std::string getType() const override;
    std::string getName() const override;
    double getX() const override;
    double getY() const override;
    bool canBeat(NPC* other) const override;
    void accept(class Visitor& visitor) override;
    void print() const override;
    std::string saveString() const override;
};

class NPCFactory {
public:
    static std::unique_ptr<NPC> create(const std::string& type,
                                       const std::string& name,
                                       double x, double y);
    static std::unique_ptr<NPC> load(const std::string& data);
};

class ConsoleObserver : public Observer {
public:
    void update(const std::string& message) override;
};

class FileObserver : public Observer {
    std::ofstream file;
public:
    FileObserver(const std::string& filename);
    ~FileObserver();
    void update(const std::string& message) override;
};

class Observable {
    std::vector<Observer*> observers;
public:
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void notify(const std::string& message);
};

class Visitor {
public:
    virtual ~Visitor() = default;
    virtual void visit(Ork* ork) = 0;
    virtual void visit(Guard* guard) = 0;
    virtual void visit(Bandit* bandit) = 0;
};

class BattleVisitor : public Visitor {
    NPC* attacker;
    Observable* observable;
    double range;
    
    bool inRange(NPC* a, NPC* b);
public:
    BattleVisitor(Observable* observable, double range);
    void setAttacker(NPC* a);
    void visit(Ork* ork) override;
    void visit(Guard* guard) override;
    void visit(Bandit* bandit) override;
    void fight(NPC* a, NPC* b);
};

class DungeonEditor : public Observable {
    std::vector<std::unique_ptr<NPC>> npcs;
    
    bool validCoords(double x, double y);
    bool isNameUnique(const std::string& name) const;
public:
    void addNPC(const std::string& type,
                const std::string& name,
                double x, double y);
    void printAll() const;
    void save(const std::string& filename) const;
    void load(const std::string& filename);
    void battle(double range);
    size_t count() const;
};

class SimpleMemoryResource : public std::pmr::memory_resource {
    struct Block {
        void* ptr;
        size_t size;
        Block(void* p, size_t s) : ptr(p), size(s) {}
    };
    
    std::vector<Block> allocated;
    std::vector<Block> freePool;
    
protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;
    
public:
    ~SimpleMemoryResource();
};

template<typename T>
class SimpleList {
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {}
    };
    
    Node* head = nullptr;
    size_t size_ = 0;
    std::pmr::memory_resource* mr;
    
public:
    SimpleList(std::pmr::memory_resource* mr = std::pmr::get_default_resource()) : mr(mr) {}
    
    ~SimpleList() { clear(); }
    
    void push_front(const T& value) {
        std::pmr::polymorphic_allocator<Node> alloc(mr);
        Node* newNode = alloc.allocate(1);
        alloc.construct(newNode, value, head);
        head = newNode;
        size_++;
    }
    
    void pop_front() {
        if (!head) return;
        std::pmr::polymorphic_allocator<Node> alloc(mr);
        Node* old = head;
        head = head->next;
        alloc.destroy(old);
        alloc.deallocate(old, 1);
        size_--;
    }
    
    void clear() {
        while (head) pop_front();
    }
    
    T& front() { return head->data; }
    const T& front() const { return head->data; }
    
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }
    
    class iterator {
        Node* node;
    public:
        iterator(Node* n = nullptr) : node(n) {}
        T& operator*() { return node->data; }
        T* operator->() { return &node->data; }
        iterator& operator++() { node = node->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }
        bool operator==(const iterator& other) const { return node == other.node; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
    };
    
    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};
