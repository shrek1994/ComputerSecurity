#pragma once

#include <cryptopp/secblock.h>
#include <vector>

class HexConverter
{
public:
    std::string addSpace(std::string in) const;
    std::vector<CryptoPP::byte> convertHex(const std::string& txt) const;
};
