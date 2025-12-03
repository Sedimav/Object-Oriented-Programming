#include "func.h"
#include <gtest/gtest.h>
#include <fstream>

TEST(NPCTest, OrkCreation) {
    Ork ork("Гром", 10, 20);
    EXPECT_EQ(ork.getType(), "Орк");
    EXPECT_EQ(ork.getName(), "Гром");
    EXPECT_EQ(ork.getX(), 10);
    EXPECT_EQ(ork.getY(), 20);
}

TEST(NPCTest, GuardCreation) {
    Guard guard("Луна", 30, 40);
    EXPECT_EQ(guard.getType(), "Оборотень");
    EXPECT_EQ(guard.getName(), "Луна");
    EXPECT_EQ(guard.getX(), 30);
    EXPECT_EQ(guard.getY(), 40);
}

TEST(NPCTest, BanditCreation) {
    Bandit bandit("Тень", 50, 60);
    EXPECT_EQ(bandit.getType(), "Разбойник");
    EXPECT_EQ(bandit.getName(), "Тень");
    EXPECT_EQ(bandit.getX(), 50);
    EXPECT_EQ(bandit.getY(), 60);
}

TEST(NPCTest, CombatRules) {
    Ork ork("Гром", 0, 0);
    Guard guard("Луна", 0, 0);
    Bandit bandit("Тень", 0, 0);
    
    EXPECT_TRUE(ork.canBeat(&bandit));
    EXPECT_FALSE(ork.canBeat(&guard));
    
    EXPECT_TRUE(guard.canBeat(&bandit));
    EXPECT_FALSE(guard.canBeat(&ork));
    
    EXPECT_TRUE(bandit.canBeat(&guard));
    EXPECT_FALSE(bandit.canBeat(&ork));
}

TEST(FactoryTest, CreateNPC) {
    auto ork = NPCFactory::create("Орк", "Гром", 10, 20);
    ASSERT_NE(ork, nullptr);
    EXPECT_EQ(ork->getType(), "Орк");
    EXPECT_EQ(ork->getName(), "Гром");
}

TEST(FactoryTest, LoadNPC) {
    std::string data = "Орк Гром 100 200";
    auto npc = NPCFactory::load(data);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Орк");
    EXPECT_EQ(npc->getName(), "Гром");
    EXPECT_EQ(npc->getX(), 100);
    EXPECT_EQ(npc->getY(), 200);
}

TEST(DungeonEditorTest, AddNPC) {
    DungeonEditor editor;
    
    editor.addNPC("Орк", "Гром", 100, 200);
    editor.addNPC("Оборотень", "Луна", 300, 400);
    EXPECT_EQ(editor.count(), 2);
}

TEST(ObserverTest, ConsoleObserver) {
    ConsoleObserver observer;
    testing::internal::CaptureStdout();
    observer.update("Тестовое сообщение");
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_TRUE(output.find("Тестовое сообщение") != std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
