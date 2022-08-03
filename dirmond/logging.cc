/*
 * Filename     logging.cc
 * Date         05/09/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Logging functions
 */

#include <stdio.h>
#include <fstream>
#include "dirmond.h"

void printConfigs(){
    printf("Daemon:      %s\n", configs[DAEMON].c_str());
    printf("Config:      %s\n", configs[CONFFILE].c_str());
    printf("WatchDir:    %s\n", configs[WATCHDIR].c_str());
    printf("LogFile:     %s\n", configs[LOGFILE].c_str());
    printf("Verbose:     %s\n", configs[VERBOSE].c_str());
    printf("Password:    %s\n", configs[PASSWORD].c_str());
    printf("NumVersions: %s\n", configs[NUMVER].c_str());
    printf("\n");
}

void logConfigs(){
    logfile << "LogFile:     " << configs[LOGFILE] << std::endl;
    logfile << "Verbose:     " << configs[VERBOSE] << std::endl;
    logfile << "NumVersions: " << configs[NUMVER] << std::endl;
}
