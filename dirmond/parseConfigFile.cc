/*
 * Filename     parseConfigFile.cc
 * Date         05/07/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Functions for parsing the config file
 */

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "rude/config.h"
#include "dirmond.h"

void parseConfigFile(){
    const char *confFilename = configs[CONFFILE].c_str();
    rude::Config config;

    if (!config.load(confFilename)){
        fprintf(stderr, "Error: config file '%s' could not be opened\n", confFilename);
        exit(EXIT_FAILURE);
    }

    config.setCommentCharacter('#');
    
    if (!config.setSection("Parameters", false)){
        fprintf(stderr, "Error: [Parameters] section not found in '%s'\n", confFilename);
        exit(EXIT_FAILURE);
    }

    if (config.exists("Verbose")){
        configs[VERBOSE] = config.getBoolValue("Verbose")? "T" : "F";
    }
    else {
        fprintf(stderr, "Error: 'Verbose' not defined in '%s'\n", confFilename);
        exit(EXIT_FAILURE);
    }
    
    if (config.exists("LogFile")){
        configs[LOGFILE] = config.getStringValue("LogFile");
    }
    else {
         fprintf(stderr, "Error: 'LogFile' not defined in '%s'\n", confFilename);
        exit(EXIT_FAILURE);
    }

    if (config.exists("WatchDir")){
        configs[WATCHDIR] = config.getStringValue("WatchDir");
        if (configs[WATCHDIR].at(configs[WATCHDIR].length() - 1) == '/'){
            configs[WATCHDIR] = configs[WATCHDIR].substr(0, configs[WATCHDIR].length() - 1);
        }
    }
    else {
        fprintf(stderr, "Error: 'WatchDir' not defined in '%s'\n", confFilename);
        exit(EXIT_FAILURE);
    }

    if (config.exists("Password")){
        configs[PASSWORD] = config.getStringValue("Password");
    }
    else {
        fprintf(stderr, "Error: 'Password' not defined in '%s'\n", confFilename);
        exit(EXIT_FAILURE);
    }

    if (config.exists("NumVersions")){
        int numver = config.getIntValue("NumVersions");
        char str[12];
        sprintf(str, "%d", numver);
        configs[NUMVER] = str;

        if (configs[NUMVER] <= "0"){
            fprintf(stderr, "Error: 'NumVersions' must be positive\n");
            exit(EXIT_FAILURE);
        }
    }
    else {
        fprintf(stderr, "Error: 'NumVersions' not defined in '%s'\n", confFilename);
        exit(EXIT_FAILURE);
    }
}

void reparseConfigFile(){
    const char *confFilename = configs[CONFFILE].c_str();
    rude::Config config;

    if (!config.load(confFilename)){
        logfile << "Error: config file '" << confFilename << "' could not be opened" << std::endl;
        endFail();
    }

    config.setCommentCharacter('#');
    
    if (!config.setSection("Parameters", false)){
        logfile << "Error: [Parameters] section not found in '" << confFilename << "'" << std::endl;
        endFail();
    }

    if (config.exists("Verbose")){
        configs[VERBOSE] = config.getBoolValue("Verbose")? "T" : "F";
    }
    else {
        logfile << "Error: 'Verbose' not defined in '" << confFilename << "'" << std::endl;
        endFail();
    }
    
    if (config.exists("LogFile")){
        configs[LOGFILE] = config.getStringValue("LogFile");
    }
    else {
        logfile << "Error: 'LogFile' not defined in '" << confFilename << "'" << std::endl;
        endFail();
    }

    if (!config.exists("WatchDir") || configs[WATCHDIR] != config.getStringValue("WatchDir")){
        logfile << "Error: Cannot change 'WatchDir' during execution; altered value ignored" << std::endl;
    }

    if (!config.exists("Password") || configs[PASSWORD] != config.getStringValue("Password")){
        logfile << "Error: Cannot change 'Password' during execution; altered value ignored" << std::endl;
    }

    if (config.exists("NumVersions")){
        int numver = config.getIntValue("NumVersions");
        char str[12];
        sprintf(str, "%d", numver);
        configs[NUMVER] = str;

        if (configs[NUMVER] <= "0"){
            logfile << "Error: 'NumVersions' must be positive" << std::endl;
            endFail();
        }
    }
    else {
        logfile << "Error: 'NumVersions' not defined in '" << confFilename << "'" << std::endl;
        endFail();
    }
}
