#include "bull.h"
#include "game.h"
#include <gtest/gtest.h>
#include <thread>

TEST(BullTest, Constructor) {
    Bull bull("Тест", 10.5, 20.5);
    
    EXPECT_EQ(bull.getName(), "Тест");
    EXPECT_DOUBLE_EQ(bull.getX(), 10.5);
    EXPECT_DOUBLE_EQ(bull.getY(), 20.5);
    EXPECT_TRUE(bull.isAlive());
}

TEST(BullTest, Movement) {
    Bull bull("Движущийся", 0.0, 0.0);
    
    bull.move(5.0, 10.0);
    EXPECT_DOUBLE_EQ(bull.getX(), 5.0);
    EXPECT_DOUBLE_EQ(bull.getY(), 10.0);
}

TEST(BullTest, Kill) {
    Bull bull("Жертва", 0.0, 0.0);
    
    bull.kill();
    EXPECT_FALSE(bull.isAlive());
    
    bull.move(5.0, 5.0);
    EXPECT_DOUBLE_EQ(bull.getX(), 0.0);
}

TEST(BullTest, Constants) {
    EXPECT_DOUBLE_EQ(Bull::MOVE_DISTANCE, 30.0);
    EXPECT_DOUBLE_EQ(Bull::ATTACK_RANGE, 10.0);
}

TEST(GameTest, DiceRoll) {
    for (int i = 0; i < 100; ++i) {
        int roll = Game::rollD6();
        EXPECT_GE(roll, 1);
        EXPECT_LE(roll, 6);
    }
}

TEST(GameTest, RandomDouble) {
    for (int i = 0; i < 100; ++i) {
        double val = Game::randomDouble(0.0, 100.0);
        EXPECT_GE(val, 0.0);
        EXPECT_LE(val, 100.0);
    }
}

TEST(GameTest, GameInitialization) {
    Game game;
    
    EXPECT_GE(game.countAlive(), 0);
}

TEST(ConcurrencyTest, BullThreadSafety) {
    Bull bull("Тестовый", 50.0, 50.0);
    
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back([&bull]() {
            for (int j = 0; j < 10; ++j) {
                bull.getX();
                bull.getY();
                bull.move(1.0, 1.0);
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        });
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    EXPECT_GT(bull.getX(), 50.0);
    EXPECT_GT(bull.getY(), 50.0);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
