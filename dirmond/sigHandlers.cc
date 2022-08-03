/*
 * Filename     sigHandlers.cc
 * Date         05/07/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Functions for handling signals
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include "dirmond.h"

void end(int signum){
    if (isVerbose){
        logfile << "Exiting..." << std::endl;
    }

    system("rm -f cs3377dirmond.pid");
    logfile.close();
    exit(EXIT_SUCCESS);
}

void endFail(){
    if (isVerbose){
        logfile << "Exiting..." << std::endl;
    }

    system("rm -f cs3377dirmond.pid");
    logfile.close();
    exit(EXIT_FAILURE);   
}

void rereadConf(int signum){
    if (isVerbose){
        logfile << "Rereading config file..." << std::endl;
    }

    int prevNumVer = atoi(configs[NUMVER].c_str());

    reparseConfigFile();
    isVerbose = configs[VERBOSE] == "T";

    if (isVerbose){
        logConfigs();
    }

    if (logFilename != configs[LOGFILE]){
        if (isVerbose){
            logfile << "Switching to '" << configs[LOGFILE] << "' for logging" << std::endl;
        }

        logFilename = configs[LOGFILE];
        logfile.close();
        logfile.open(logFilename.c_str(), std::ios::out);
        if (!logfile.is_open()){
            endFail();
        }
    }

    if (prevNumVer > atoi(configs[NUMVER].c_str())){
        if (isVerbose){
            logfile << "'NumVersions' has decreased; removing oldest copies" << std::endl;
        }
        removeOldFiles();
    }
}

void removeOldFiles(){
    fileVers.clear();

    std::string findcmd = "ls -la " + versionsLoc + " | gawk 'match($9, /((.*)\\.[0-9]{4}\\.[0-9]{2}\\.[0-9]{2}-[0-9]{2}:[0-9]{2}:[0-9]{2})/, arr) {print arr[2] \" \" arr[1]}'";
    FILE *cmdout = popen(findcmd.c_str(), "r");
    if (!cmdout){
        if (isVerbose){
            logfile << "Error: Could not get files in .versions" << std::endl;
        }
        return;
    }

    char tmpbuf[1024];
    char* line = fgets(tmpbuf, 1024, cmdout);

    while (line){
        std::stringstream words(line);
        std::string filename;
        words >> filename;

        if (!fileVers.count(filename)){
            fileVers[filename] = std::queue<std::string>();
        }

        std::string nextfile;
        words >> nextfile;
        fileVers[filename].push(nextfile);

        if (fileVers[filename].size() > (unsigned) atoi(configs[NUMVER].c_str())){
            if (isVerbose){
                logfile << "Removing oldest version of '" << filename << "' from '.versions/' and '.secure/'" << std::endl;
            }

            std::string rmfile = fileVers[filename].front();
            std::string rmcmd = "rm -f " + versionsLoc + "/" + rmfile;
            system(rmcmd.c_str());
            rmcmd = "rm -f " + secureLoc + "/" + rmfile;
            system(rmcmd.c_str());
            fileVers[filename].pop();
        }

        line = fgets(tmpbuf, 1024, cmdout);
    }

    pclose(cmdout);
}
