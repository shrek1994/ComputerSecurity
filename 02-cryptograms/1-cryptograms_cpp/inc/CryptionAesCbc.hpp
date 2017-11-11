#pragma once

#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>

class CryptionAesCbc
{
public:
    CryptionAesCbc(CryptoPP::SecByteBlock& key, CryptoPP::SecByteBlock& iv)
        : key(std::move(key)),
          iv(std::move(iv))
    {}

    std::stringstream encrypt(std::istream& in);
    std::string decrypt(const std::string &in);

private:
    CryptoPP::SecByteBlock key;
    CryptoPP::SecByteBlock iv;
};
