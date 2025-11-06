#include <gtest/gtest.h>
#include "twelve.h"

TEST(TwelveTest, DefaultConstructor) {
    Twelve num;
    EXPECT_EQ(num.toString(), "0");
    EXPECT_EQ(num.size(), 1);
}

TEST(TwelveTest, SizeConstructor) {
    Twelve num(4, '5');
    EXPECT_EQ(num.toString(), "5555");
    EXPECT_EQ(num.size(), 4);
}

TEST(TwelveTest, InitializerListConstructor) {
    Twelve num{'1', 'A', 'B', '2'};
    EXPECT_EQ(num.toString(), "1AB2");
}

TEST(TwelveTest, StringConstructor) {
    Twelve num("A5B3");
    EXPECT_EQ(num.toString(), "A5B3");
}

TEST(TwelveTest, CopyConstructor) {
    Twelve original("A5B");
    Twelve copy(original);
    EXPECT_TRUE(original.equals(copy));
    EXPECT_EQ(copy.toString(), "A5B");
}

TEST(TwelveTest, MoveConstructor) {
    Twelve original("1B2");
    std::string originalString = original.toString();
    Twelve moved(std::move(original));
    EXPECT_EQ(moved.toString(), originalString);
    EXPECT_EQ(original.toString(), "0");
}

TEST(TwelveTest, AddOperation) {
    Twelve a("A5");
    Twelve b("2B");
    Twelve result = a.add(b);
    EXPECT_EQ(result.toString(), "114");
}

TEST(TwelveTest, SubtractOperation) {
    Twelve a("A5");
    Twelve b("2B");
    Twelve result = a.subtract(b);
    EXPECT_EQ(result.toString(), "76");
}

TEST(TwelveTest, ComparisonOperations) {
    Twelve a("A5");
    Twelve b("2B");
    Twelve c("A5");
    EXPECT_TRUE(a.greaterThan(b));
    EXPECT_TRUE(b.lessThan(a));
    EXPECT_TRUE(a.equals(c));
    EXPECT_FALSE(a.equals(b));
}

TEST(TwelveTest, InvalidInput) {
    EXPECT_THROW(Twelve("G12"), std::invalid_argument);
    EXPECT_THROW(Twelve({'1', 'C'}), std::invalid_argument);
    EXPECT_THROW(Twelve(3, 'G'), std::invalid_argument);
}

TEST(TwelveTest, LeadingZeros) {
    Twelve num1("000A5");
    EXPECT_EQ(num1.toString(), "A5");
    Twelve num2("0000");
    EXPECT_EQ(num2.toString(), "0");
}

TEST(TwelveTest, EdgeCases) {
    Twelve zero;
    Twelve num("A5");
    Twelve sum = num.add(zero);
    EXPECT_TRUE(num.equals(sum));
    
    Twelve num2("A5");
    Twelve diff = num2.subtract(num2);
    EXPECT_EQ(diff.toString(), "0");
}

TEST(TwelveTest, AddAssign) {
    Twelve a("A5");
    Twelve b("2B");
    Twelve result = a.copy();
    result.addAssign(b);
    EXPECT_EQ(result.toString(), "114");
}

TEST(TwelveTest, SubtractAssign) {
    Twelve a("A5");
    Twelve b("2B");
    Twelve result = a.copy();
    result.subtractAssign(b);
    EXPECT_EQ(result.toString(), "76");
}
