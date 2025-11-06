#include <gtest/gtest.h>
#include "turkish_number_converter.h"

TEST(TurkishNumberConverterTest, BasicNumbers) {
    EXPECT_EQ(convert_to_turkish(0), "sıfır");
    EXPECT_EQ(convert_to_turkish(1), "bir");
    EXPECT_EQ(convert_to_turkish(5), "beş");
    EXPECT_EQ(convert_to_turkish(9), "dokuz");
}

TEST(TurkishNumberConverterTest, Tens) {
    EXPECT_EQ(convert_to_turkish(10), "on");
    EXPECT_EQ(convert_to_turkish(20), "yirmi");
    EXPECT_EQ(convert_to_turkish(30), "otuz");
    EXPECT_EQ(convert_to_turkish(40), "kırk");
    EXPECT_EQ(convert_to_turkish(50), "elli");
    EXPECT_EQ(convert_to_turkish(60), "altmış");
    EXPECT_EQ(convert_to_turkish(70), "yetmiş");
    EXPECT_EQ(convert_to_turkish(80), "seksen");
    EXPECT_EQ(convert_to_turkish(90), "doksan");
}

TEST(TurkishNumberConverterTest, CombinedNumbers) {
    EXPECT_EQ(convert_to_turkish(13), "on üç");
    EXPECT_EQ(convert_to_turkish(27), "yirmi yedi");
    EXPECT_EQ(convert_to_turkish(38), "otuz sekiz");
    EXPECT_EQ(convert_to_turkish(77), "yetmiş yedi");
    EXPECT_EQ(convert_to_turkish(94), "doksan dört");
}

TEST(TurkishNumberConverterTest, EdgeCases) {
    EXPECT_EQ(convert_to_turkish(11), "on bir");
    EXPECT_EQ(convert_to_turkish(22), "yirmi iki");
    EXPECT_EQ(convert_to_turkish(99), "doksan dokuz");
}

TEST(TurkishNumberConverterTest, OutOfRangeThrowsException) {
    EXPECT_THROW(convert_to_turkish(-1), std::out_of_range);
    EXPECT_THROW(convert_to_turkish(100), std::out_of_range);
}
