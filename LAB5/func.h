#pragma once
#include <cstddef>
#include <list>
#include <memory_resource>
#include <string>

class DynamicListMemoryResource : public std::pmr::memory_resource {
private:
    struct Block {
        void* ptr;
        size_t size;
        Block(void* p, size_t s) : ptr(p), size(s) {}
    };

    std::list<Block> allocated_;
    std::list<Block> free_pool_;

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void do_deallocate(void* p, size_t bytes, size_t alignment) override;
    bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

public:
    ~DynamicListMemoryResource() override;

    size_t allocated_count() const { return allocated_.size(); }
    size_t free_pool_count() const { return free_pool_.size(); }
};

template <typename T>
class ForwardList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& d, Node* n) : data(d), next(n) {}
        Node(T&& d, Node* n) : data(std::move(d)), next(n) {}
    };

    Node* head_ = nullptr;
    size_t size_ = 0;
    std::pmr::memory_resource* mr_;

public:
    using value_type = T;

    class iterator {
        Node* node_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        explicit iterator(Node* node = nullptr) : node_(node) {}

        reference operator*() const { return node_->data; }
        pointer operator->() const { return &node_->data; }

        iterator& operator++() {
            node_ = node_->next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return node_ == other.node_;
        }
        
        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    class const_iterator {
        const Node* node_;
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = const T;
        using difference_type = std::ptrdiff_t;
        using pointer = const T*;
        using reference = const T&;

        explicit const_iterator(const Node* node = nullptr) : node_(node) {}

        reference operator*() const { return node_->data; }
        pointer operator->() const { return &node_->data; }

        const_iterator& operator++() {
            node_ = node_->next;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const const_iterator& other) const {
            return node_ == other.node_;
        }
        
        bool operator!=(const const_iterator& other) const {
            return !(*this == other);
        }
    };

    explicit ForwardList(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : mr_(mr) {}

    ForwardList(const ForwardList& other) = delete;
    ForwardList& operator=(const ForwardList& other) = delete;

    ForwardList(ForwardList&& other) noexcept
        : head_(other.head_), size_(other.size_), mr_(other.mr_) {
        other.head_ = nullptr;
        other.size_ = 0;
    }

    ForwardList& operator=(ForwardList&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = other.head_;
            size_ = other.size_;
            mr_ = other.mr_;
            other.head_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~ForwardList() {
        clear();
    }

    void push_front(const T& value) {
        std::pmr::polymorphic_allocator<Node> alloc(mr_);
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, value, head_);
        head_ = new_node;
        ++size_;
    }

    void push_front(T&& value) {
        std::pmr::polymorphic_allocator<Node> alloc(mr_);
        Node* new_node = alloc.allocate(1);
        alloc.construct(new_node, std::move(value), head_);
        head_ = new_node;
        ++size_;
    }

    void pop_front() {
        if (!head_) return;
        std::pmr::polymorphic_allocator<Node> alloc(mr_);
        Node* to_delete = head_;
        head_ = head_->next;
        alloc.destroy(to_delete);
        alloc.deallocate(to_delete, 1);
        --size_;
    }

    void clear() {
        while (head_) {
            pop_front();
        }
    }

    T& front() { return head_->data; }
    const T& front() const { return head_->data; }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
    const_iterator begin() const { return const_iterator(head_); }
    const_iterator end() const { return const_iterator(nullptr); }
};

struct Point {
    int x = 0, y = 0;
    std::string name;
    Point() = default;
    Point(int x_, int y_, std::string n) : x(x_), y(y_), name(std::move(n)) {}
};
