#ifdef TESTING

#include <gtest/gtest.h>

int main(int numOfArg, char *args[])
{
    ::testing::InitGoogleTest(&numOfArg, args);
    return RUN_ALL_TESTS();
}

#else

#include <array>
#include <string>
#include <sstream>
#include <iostream>
#include "HexConverter.hpp"
#include "CryptionAesCbc.hpp"


std::array<char, 16> hex = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
auto ivByte = HexConverter().convertHex("0ac1174f6a70db02674139d89812163f");
auto keyByte = HexConverter().convertHex("8a223e095fd90f91e13e0a4c596e5d3f27edff4f5ca5dad41ecd4148ed1549aa");
std::string cipher = "B6TpizazNZrNxpdJng5Or1RBtIpTpJQkkgS2ak0nmo5pi57A+qVESfeSBAhRJWIhIWVNiJWuF2wJmAK3C8THJNNJTYAzeBoAMeTTkGQpKljrHQkg2g3zQjtz5cYVeUoaLoiSiYTLHHBswoL7Xqzozg==";

//auto ivByte = HexConverter().convertHex("6ab9d1c8460182aac9cee2da649cc2a6");
//auto keyByte = HexConverter().convertHex("000000000b76bf1952cf38b9f6f4e5da73b7ed6d9b3dfb18f5a1a0811713b747");
//std::string cipher = "k1qNBtj7N09fu/RtIBpt/Fwrn5KwcwnWYY2o6j1yxJiE3Z8nsTomDSJVfB/GiFWRCIb2lN56mOZZS3s0vR5nWLkkuOwHBbI/uzwheJrI9BoKI0wk6AEzmrU4o6HsUiCCqvyOiwovOMgeAmnKopUp388KIS+ambCSGUaR2JbAfoJ4RpL+OTbFMyvaW1kVkpFAl6K3oycLYixrOOiPVVJJ0S1WPCy+bytM2cC/y16YE9UXXbNEdRqy4CYkQSFGpl0V";


auto decrypt()
{
    CryptoPP::SecByteBlock key = {keyByte.data(), keyByte.size()};
    CryptoPP::SecByteBlock iv =  {ivByte.data(), ivByte.size()};
    CryptionAesCbc aesCbc = {key, iv};
    std::stringstream in(cipher);

    return std::move(aesCbc.decrypt(in));
}

int main(int numOfArg, char* args[])
{
    CryptoPP::byte i = 0;
    do
    {
        keyByte[0] = i;

        CryptoPP::byte j = 0;
        do
        {
            keyByte[1] = j;


//            CryptoPP::byte k = 0;
//            do
//            {
//                keyByte[2] = k;
                std::cout << decrypt().str();
//                ++k;
//            } while (k < 255);

            ++j;
        } while (j < 255);
        ++i;
    } while (i < 255);
    return 0;
}

#endif
