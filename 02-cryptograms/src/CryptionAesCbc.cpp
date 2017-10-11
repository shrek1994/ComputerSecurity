#include "CryptionAesCbc.hpp"

#include "CryptionAesCbc.hpp"
#include <iostream>
#include <cstdio>
#include <sstream>
#include <ctype.h>

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
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

std::stringstream CryptionAesCbc::decrypt(std::istream &in)
{
    using namespace CryptoPP;
    std::stringstream out;
    std::string recoveredFile;

    try
    {
        CBC_Mode<AES>::Decryption decryption;
        decryption.SetKeyWithIV( key, key.size(), iv );

        FileSource fs (in,
                       true,
                       new CryptoPP::Base64Decoder( new StreamTransformationFilter(decryption,
                                                                                   new StringSink(recoveredFile))));
//                       new StreamTransformationFilter(decryption,
//                                                      new StringSink(recoveredFile)));


        LOG << "recovered text: " << recoveredFile << std::endl;
        if (isAscii(recoveredFile))
            out << recoveredFile;
    }
    catch( const CryptoPP::Exception& e )
    {
//        std::cerr << e.what() << std::endl;
    }

    return std::move(out);
}
