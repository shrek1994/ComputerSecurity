#include "CryptionAesCbc.hpp"

#include "CryptionAesCbc.hpp"

#include <iostream>
#include <cstdio>
#include <sstream>

#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/modes.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>

#include "debug.h"


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
        std::cerr << e.what() << std::endl;
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
        CBC_Mode< AES >::Decryption decryption;
        decryption.SetKeyWithIV( key, key.size(), iv );

        FileSource fs (in,
                       true,
                       new StreamTransformationFilter(decryption,
                                                      new StringSink(recoveredFile)));

        LOG << "recovered text: " << recoveredFile << std::endl;
        out << recoveredFile;
    }
    catch( const CryptoPP::Exception& e )
    {
        std::cerr << e.what() << std::endl;
    }

    return std::move(out);
}
