#include "CryptionAesCbc.hpp"

#include "CryptionAesCbc.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <ctype.h>

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#include <cryptopp/base64.h>

#include "debug.h"

namespace  {
bool isAscii(const std::string& txt)
{
    auto numOfAscii = std::count_if(txt.cbegin(), txt.cend(), isascii);
    auto procent = (numOfAscii * 100) / txt.size();
    LOG << "numOfAscii = " << numOfAscii << "/" << txt.size() << ", procent = " << procent << "%\n";
    return procent > 75;
}
}

//TODO REPAIR
std::stringstream CryptionAesCbc::encrypt(std::istream& in)
{
    using namespace CryptoPP;

    std::stringstream out;
    std::string cipherFile;

    try
    {
        CBC_Mode<AES>::Encryption encryption;
        encryption.SetKeyWithIV(key, key.size(), iv);
        FileSource fs(in,
                      true,
                      new StreamTransformationFilter(encryption,
                                                     new StringSink(cipherFile)));

        LOG << "Encrypted CBC: \n";
        printHex(cipherFile.c_str());

        out << cipherFile;
    }
    catch(const CryptoPP::Exception& e )
    {
//        std::cerr << e.what() << std::endl;
    }

    return std::move(out);
}

std::string CryptionAesCbc::decrypt(const std::string& in)
{
    using namespace CryptoPP;
    std::string recoveredFile = "";
    char expected[] = "xxx ma kota. Kot ma, ale... to jednak ona go posiada. Jednakże gdy przeczytamy to ponownie to...\n";


    try
    {
        CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption decryption;
        decryption.SetKeyWithIV( key, key.size(), iv );

        auto stringSink = new StringSink(recoveredFile); //std::make_unique<CryptoPP::StringSink>(recoveredFile);
        auto streamFilter = new StreamTransformationFilter(decryption, stringSink); // std::make_unique<CryptoPP::StreamTransformationFilter>(decryption, stringSink.get());
        auto base64Decoder = new CryptoPP::Base64Decoder(streamFilter);//std::make_unique<CryptoPP::Base64Decoder>(streamFilter.get());

        StringSource ss (in, true, base64Decoder);

        LOG << "recovered text: " << recoveredFile << std::endl;
        if (isAscii(recoveredFile))
            return std::move(recoveredFile);
    }
    catch(const CryptoPP::Exception& e)
    {
//        std::cerr << e.what() << std::endl;
    }

    return "";
}
