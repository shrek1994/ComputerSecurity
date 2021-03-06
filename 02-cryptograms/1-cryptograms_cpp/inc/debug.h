#pragma once
#include <iostream>

constexpr bool DEBUG = false;

inline void printHex(const std::string& tab)
{
    printHex(tab.c_str());
}

inline void printHex(const char* tab)
{
    if (DEBUG)
    {
        std::cout << "HEX: 0x" << std::hex << tab << "\n";
    }
}

#define LOG \
    if (DEBUG) std::cout << __FILE__ << ":" << __LINE__ << ": "
