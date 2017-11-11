#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cryptopp/aes.h>
#include <string>
#include <sstream>

#include "CryptionAesCbc.hpp"
#include "HexConverter.hpp"

using byte = CryptoPP::byte;

#define EXPECT_BYTEEQ(expected, result) \
    for(unsigned i = 0 ; i < sizeof(expected); ++i) \
        EXPECT_EQ((byte)expected[i], (byte)result[i]) << "i = " << i << "/" << sizeof(expected) - 1



namespace {
const char text[] = "Hello World !!!\nEncrypt Test!";
const std::string textString = text;
const byte encryptedText[] = {0xA, 0xB, 0x75, 0xB6, 0x11, 0xAF, 0xEF, 0xD7, 0x61, 0x31, 0xD3, 0x13, 0x16, 0x8A,
                              0xEC, 0x18, 0x78, 0xE6, 0x38, 0xC9, 0x9E, 0xF, 0xB0, 0x7B, 0xD3, 0x26, 0x4E, 0xBD,
                              0x17, 0x75, 0x67, 0x4C, 0x0};
const byte keyByte[CryptoPP::AES::DEFAULT_KEYLENGTH] = { 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78, 0x89, 0x90 };
const byte ivByte[CryptoPP::AES::DEFAULT_KEYLENGTH] = { 0x98, 0x76, 0x54, 0x32, 0x10 };

class CryptionCBCTest : public ::testing::Test {

protected:
    CryptoPP::SecByteBlock key = {keyByte, sizeof(keyByte)};
    CryptoPP::SecByteBlock iv =  {ivByte, sizeof(ivByte)};
    CryptionAesCbc sut = {key, iv};
};

TEST_F(CryptionCBCTest, DISABLED_encryptFile)
{
    std::stringstream in(text);

    auto out = sut.encrypt(in);
    EXPECT_BYTEEQ(encryptedText, out.str().c_str());
}

TEST_F(CryptionCBCTest, DISABLED_decryptFile)
{
    std::stringstream in;
    in << encryptedText;

    auto out = sut.decrypt(in.str());
    EXPECT_BYTEEQ(text, out.c_str());
}

TEST_F(CryptionCBCTest, DISABLED_encryptAndDecryptFile)
{
    std::stringstream in(text);

    auto outEncrypted = sut.encrypt(in);
    auto out = sut.decrypt(outEncrypted.str());

    EXPECT_BYTEEQ(text, out.c_str());
}


TEST_F(CryptionCBCTest, shouldDecryptCorrectlyAsInExample)
{
    char expected[] = "Ala ma kota. Kot ma, ale... to jednak ona go posiada. Jednakże gdy przeczytamy to ponownie to...\n";

    auto ivByte = HexConverter().convertHex("0ac1174f6a70db02674139d89812163f");
    auto keyByte = HexConverter().convertHex("8a223e095fd90f91e13e0a4c596e5d3f27edff4f5ca5dad41ecd4148ed1549aa");
    std::string cipher = "B6TpizazNZrNxpdJng5Or1RBtIpTpJQkkgS2ak0nmo5pi57A+qVESfeSBAhRJWIhIWVNiJWuF2wJmAK3C8THJNNJTYAzeBoAMeTTkGQpKljrHQkg2g3zQjtz5cYVeUoaLoiSiYTLHHBswoL7Xqzozg==";
    std::stringstream in(cipher);

    CryptoPP::SecByteBlock key = {keyByte.data(), keyByte.size()};
    CryptoPP::SecByteBlock iv =  {ivByte.data(), ivByte.size()};
    CryptionAesCbc sut = {key, iv};
    auto out = sut.decrypt(in.str());

    EXPECT_BYTEEQ(expected, out.c_str());
}

}  // namespace
