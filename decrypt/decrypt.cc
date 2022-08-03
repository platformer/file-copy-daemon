/*
 * Filename     decrypt.cc
 * Date         05/09/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Program for decrypting files made by cs3377dirmond
 */

#include <string>
#include <map>
#include "cryptopp/cryptlib.h"
#include "cryptopp/files.h"
#include "cryptopp/default.h"
#include "decrypt.h"

std::map<int, std::string> args;

void DecryptFile(const char *in, const char *out, const char *passPhrase){
    using namespace CryptoPP;
    FileSource f(in, true, new DefaultDecryptorWithMAC(passPhrase, new FileSink(out)));
}

int main(int argc, char *argv[]){
    parseCmdLine(argc, argv);
    DecryptFile(args[INFILE].c_str(), args[OUTFILE].c_str(), args[PASSWORD].c_str());
    return 0;
}
