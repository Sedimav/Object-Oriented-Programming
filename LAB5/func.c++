#include "func.h"
#include <new>

void* DynamicListMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    (void)alignment;
    for (auto it = free_pool_.begin(); it != free_pool_.end(); ++it) {
        if (it->size >= bytes) {
            void* p = it->ptr;
            allocated_.emplace_back(p, bytes);
            free_pool_.erase(it);
            return p;
        }
    }
    void* p = ::operator new(bytes);
    allocated_.emplace_back(p, bytes);
    return p;
}

void DynamicListMemoryResource::do_deallocate(void* p, size_t bytes, size_t alignment) {
    (void)alignment;
    for (auto it = allocated_.begin(); it != allocated_.end(); ++it) {
        if (it->ptr == p && it->size == bytes) {
            free_pool_.emplace_back(p, bytes);
            allocated_.erase(it);
            return;
        }
    }
}

bool DynamicListMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

DynamicListMemoryResource::~DynamicListMemoryResource() {
    for (auto& b : allocated_) ::operator delete(b.ptr);
    for (auto& b : free_pool_) ::operator delete(b.ptr);
}

template class ForwardList<int>;
template class ForwardList<Point>;
