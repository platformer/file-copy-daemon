/*
 * Filename     parseCmdLine.cc
 * Date         05/07/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Function for parsing command line arguments
 */

#include <string>
#include "tclap/CmdLine.h"
#include "decrypt.h"

void parseCmdLine(int argc, char *argv[]){
    TCLAP::CmdLine cmd("cs3377decrypt File Decryptor", ' ', "1.0");

    TCLAP::UnlabeledValueArg<std::string> passwordArg("password", "The password to use for decryption", true, "", "password", false);
    cmd.add(passwordArg);

    TCLAP::UnlabeledValueArg<std::string> infileArg("infile", "The existing encrypted file", true, "", "infile", false);
    cmd.add(infileArg);

    TCLAP::UnlabeledValueArg<std::string> outfileArg("outfile", "A file that is created and contains the decrypted version of <infile>", true, "", "outfile", false);
    cmd.add(outfileArg);
   
    cmd.parse(argc, argv);

    args[PASSWORD] = passwordArg.getValue();
    args[INFILE]   = infileArg.getValue();
    args[OUTFILE]  = outfileArg.getValue();
}
