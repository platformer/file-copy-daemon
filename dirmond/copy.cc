/*
 * Filename     copy.cc
 * Date         05/09/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Functions for copying files to .versions
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/inotify.h>
#include <string>
#include <map>
#include <queue>
#include <fstream>
#include "cryptopp/cryptlib.h"
#include "cryptopp/files.h"
#include "cryptopp/default.h"
#include "dirmond.h"

void EncryptFile(const char *in, const char *out, const char *passPhrase){
    using namespace CryptoPP;
    FileSource f(in, true, new DefaultEncryptorWithMAC(passPhrase, new FileSink(out)));
}

void copyFile(struct inotify_event *event){
    std::string filename = event->name;
    std::string fileLoc = configs[WATCHDIR] + "/" + filename;

    if (isVerbose){
        logfile << "Fetching modification date..." << std::endl;
    }

    std::string datecmd = "date --reference=" + fileLoc + " +%Y.%m.%d-%H:%M:%S";
    FILE *cmdout = popen(datecmd.c_str(), "r");
    if (!cmdout){
        logfile << "Error: Could not get date stamp" << std::endl;
        return;
    }

    char tmpbuf[50];
    char *result = fgets(tmpbuf, 50, cmdout);
    pclose(cmdout);
    if (!result){
        logfile << "Error: '" << filename << "' probably does not exist anymore" << std::endl;
        return;
    }
    std::string date = std::string(tmpbuf);
    date = date.substr(0, date.length() - 1);
    std::string datedFilename = filename + "." + date;

    if (isVerbose){
        logfile << "Copying '" << filename << "' to '.versions/" << datedFilename << "'" << std::endl;
    }

    std::string newLoc = versionsLoc + "/" + datedFilename;
    std::string syscmd = "cp " + fileLoc + " " + newLoc;
    int ret = system(syscmd.c_str());

    if (!ret){
        if (isVerbose){
            logfile << "Encrypting and copying '" << filename << "' to '.secure/" << datedFilename << "'" << std::endl;
        }

        std::string newSecLoc = secureLoc + "/" + datedFilename;
        EncryptFile(newLoc.c_str(), newSecLoc.c_str(), configs[PASSWORD].c_str());

        if (fileVers.count(filename)){
            fileVers[filename].push(datedFilename);
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
        }
        else {
            fileVers[filename] = std::queue<std::string>();
            fileVers[filename].push(datedFilename);
        }
    }
    else {
        logfile << "Error: '" << filename << "' probably does not exist anymore" << std::endl;
    }
}

