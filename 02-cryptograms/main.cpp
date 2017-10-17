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
#include <thread>
#include <climits>
#include <chrono>
#include "HexConverter.hpp"
#include "CryptionAesCbc.hpp"
#include "debug.h"

constexpr int numberOfThreads = 4;

auto ivByte = HexConverter().convertHex("0ac1174f6a70db02674139d89812163f");
CryptoPP::SecByteBlock iv =  {ivByte.data(), ivByte.size()};
auto keyByte = HexConverter().convertHex("8a223e095fd90f91e13e0a4c596e5d3f27edff4f5ca5dad41ecd4148ed1549aa");
std::string cipher = "B6TpizazNZrNxpdJng5Or1RBtIpTpJQkkgS2ak0nmo5pi57A+qVESfeSBAhRJWIhIWVNiJWuF2wJmAK3C8THJNNJTYAzeBoAMeTTkGQpKljrHQkg2g3zQjtz5cYVeUoaLoiSiYTLHHBswoL7Xqzozg==";

//auto ivByte = HexConverter().convertHex("6ab9d1c8460182aac9cee2da649cc2a6");
//CryptoPP::SecByteBlock iv =  {ivByte.data(), ivByte.size()};
//auto keyByte = HexConverter().convertHex("000000000b76bf1952cf38b9f6f4e5da73b7ed6d9b3dfb18f5a1a0811713b747");
//std::string cipher = "k1qNBtj7N09fu/RtIBpt/Fwrn5KwcwnWYY2o6j1yxJiE3Z8nsTomDSJVfB/GiFWRCIb2lN56mOZZS3s0vR5nWLkkuOwHBbI/uzwheJrI9BoKI0wk6AEzmrU4o6HsUiCCqvyOiwovOMgeAmnKopUp388KIS+ambCSGUaR2JbAfoJ4RpL+OTbFMyvaW1kVkpFAl6K3oycLYixrOOiPVVJJ0S1WPCy+bytM2cC/y16YE9UXXbNEdRqy4CYkQSFGpl0V";


auto decrypt(const std::vector<CryptoPP::byte>& keyByte = ::keyByte)
{
    CryptoPP::SecByteBlock key = {keyByte.data(), keyByte.size()};
    CryptionAesCbc aesCbc = {key, iv};
//    std::stringstream in(cipher);

    return std::move(aesCbc.decrypt(cipher));
}


void findText(std::vector<CryptoPP::byte> keyByte, int begin, int end)
{
    CryptoPP::byte i =  begin -1;
    do
    {
        auto startTime = std::chrono::system_clock::now();
        ++i;
        keyByte[0] = i;

        CryptoPP::byte j = -1;
        do
        {
            ++j;
            keyByte[1] = j;

            CryptoPP::byte k = -1;
            do
            {
                ++k;
                keyByte[2] = k;

                CryptoPP::byte l = -1;
                do
                {
                    ++l;
                    keyByte[3] = l;
                    std::cout << decrypt(keyByte);
                } while (l < UCHAR_MAX);
            } while (k < UCHAR_MAX);
        } while (j < UCHAR_MAX);

        auto endTime = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = endTime - startTime;
        std::cout << "i = " << i << "/" << end << ", begin: "<< begin
                  << ", elapsed time: " << elapsed_seconds.count() << "s\n";
    } while (i < end);
}

void runThreads()
{
    auto startTime = std::chrono::system_clock::now();
    std::vector<std::thread> threads;
    int numberOfFinding = (UCHAR_MAX + 1)  / numberOfThreads;
    LOG << "uchar max = " << UCHAR_MAX << ", numOfFind = " << numberOfFinding << "\n";
    int begin = 0;
    int end = 0;

    for (int i = 0 ; i < numberOfThreads ; ++i)
    {
        begin = end;
        end = (i+1) * numberOfFinding - 1;
        LOG << i <<". begin = " << begin << ", end = " << end << "\n";
        threads.push_back(std::thread(findText, keyByte, begin, end));
    }

    for(auto& thread : threads) {
        thread.join();
    }
    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    std::cout/* << "started computation at  " << std::ctime(&startTime) << "\n"
              << "finished computation at " << std::ctime(&endTime) << "\n"*/
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

}

int main(int, char*[])
{
    auto startTime = std::chrono::system_clock::now();
    CryptoPP::byte i = 0;
    do
    {
        keyByte[0] = i;

        CryptoPP::byte j = 0;
        do
        {
            keyByte[1] = j;


            CryptoPP::byte k = 0;
            do
            {
                keyByte[2] = k;
                std::cout << decrypt();
                ++k;
            } while (k < UCHAR_MAX);

            ++j;
        } while (j < UCHAR_MAX);
        ++i;
    } while (i < UCHAR_MAX);

    auto endTime = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = endTime - startTime;
    std::cout/* << "started computation at  " << std::ctime(&startTime) << "\n"
              << "finished computation at " << std::ctime(&endTime) << "\n"*/
              << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return 0;
}

#endif
