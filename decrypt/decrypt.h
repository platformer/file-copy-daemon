/*
 * Filename     decrypt.h
 * Date         05/09/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Header file for cs3377decrypt
 */

#ifndef DECRYPT_H
#define DECRYPT_H

#include <string>
#include <map>

enum Args{INFILE, OUTFILE, PASSWORD};
extern std::map<int, std::string> args;
void parseCmdLine(int, char**);

#endif
