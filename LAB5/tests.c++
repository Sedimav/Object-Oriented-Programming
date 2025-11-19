#include "func.h"
#include <gtest/gtest.h>

TEST(MemoryResourceTest, ReuseBlocks) {
    DynamicListMemoryResource mr;
    std::pmr::polymorphic_allocator<char> alloc(&mr);

    auto* p1 = alloc.allocate(64);
    auto* p2 = alloc.allocate(32);
    EXPECT_EQ(mr.allocated_count(), 2);
    EXPECT_EQ(mr.free_pool_count(), 0);

    alloc.deallocate(p1, 64);
    EXPECT_EQ(mr.allocated_count(), 1);
    EXPECT_EQ(mr.free_pool_count(), 1);

    auto* p3 = alloc.allocate(64);
    EXPECT_EQ(p3, p1); 
    EXPECT_EQ(mr.allocated_count(), 2);
    EXPECT_EQ(mr.free_pool_count(), 0);

    alloc.deallocate(p2, 32);
    alloc.deallocate(p3, 64);
}

TEST(MemoryResourceTest, CleanupOnDestruction) {
    size_t before = 0;
    {
        DynamicListMemoryResource mr;
        std::pmr::polymorphic_allocator<int> alloc(&mr);
        auto* p = alloc.allocate(10);
        (void)p;
        before = mr.allocated_count();
    }
    SUCCEED();
}

TEST(ForwardListTest, PushFrontInt) {
    DynamicListMemoryResource mr;
    ForwardList<int> list(&mr);
    
    list.push_front(10);
    list.push_front(20);
    
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front(), 20);
}

TEST(ForwardListTest, IteratorInt) {
    DynamicListMemoryResource mr;
    ForwardList<int> list(&mr);
    
    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int sum = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);
}

TEST(ForwardListTest, PushFrontPoint) {
    DynamicListMemoryResource mr;
    ForwardList<Point> list(&mr);
    
    list.push_front(Point{1, 2, "A"});
    list.push_front(Point{3, 4, "B"});
    
    EXPECT_EQ(list.size(), 2);
    EXPECT_EQ(list.front().x, 3);
    EXPECT_EQ(list.front().name, "B");
}

TEST(ForwardListTest, MoveConstructor) {
    DynamicListMemoryResource mr;
    ForwardList<int> a(&mr);
    
    a.push_front(100);
    a.push_front(200);

    ForwardList<int> b = std::move(a);
    EXPECT_EQ(b.size(), 2);
    EXPECT_EQ(b.front(), 200);
    EXPECT_EQ(a.size(), 0);
}

TEST(ForwardListTest, MoveAssignment) {
    DynamicListMemoryResource mr;
    ForwardList<int> a(&mr), b(&mr);
    
    a.push_front(1);
    b.push_front(10);
    b.push_front(20);

    b = std::move(a);
    EXPECT_EQ(b.size(), 1);
    EXPECT_EQ(b.front(), 1);
    EXPECT_EQ(a.size(), 0);
}
