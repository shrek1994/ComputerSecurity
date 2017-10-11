#include "HexConverter.hpp"
#include <sstream>
#include "debug.h"

std::string HexConverter::addSpace(std::string in) const {
    for (int i = in.size() ; i > 0 ; i -= 2)
    {
        in.insert(i, " ");
    }
    return std::move(in);
}

std::vector<CryptoPP::byte> HexConverter::convertHex(const std::string &txt) const
{
    auto hexString = addSpace(txt);
    std::vector<CryptoPP::byte> hexs;
    hexs.reserve(txt.size() / 2);
    std::stringstream hexStream(hexString);
    unsigned number = 0;
    hexStream >> std::hex >> number;
    while (hexStream.good())
    {
        hexs.push_back(number);
        hexStream >> std::hex >> number;
    }
    LOG << "size: " << hexs.size() << ", " << txt << "\n";
    return std::move(hexs);
}
