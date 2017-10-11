#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "HexConverter.hpp"
#include <sstream>

class HexConverterTest : public ::testing::Test
{
public:
    HexConverter sut;
};

TEST_F(HexConverterTest, shouldAddSpaceEveryTwoCharacters)
{
    std::string expected = "0a c1 17 4f 6a 70 db 02 67 41 39 d8 98 12 16 3f ";
    std::string in = "0ac1174f6a70db02674139d89812163f";
    EXPECT_EQ(expected, sut.addSpace(in));
}

TEST_F(HexConverterTest, shouldCorrectConvertStringToHex)
{
    auto result = sut.convertHex("01a1ff");
    std::vector<CryptoPP::byte> expected = { 0x01, 0xa1, 0xff };
    EXPECT_EQ(expected, result);
}



