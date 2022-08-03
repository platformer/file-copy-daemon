/*
 * Filename     dirmond.cc
 * Date         05/09/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Daemon program that tracks a folder for changes
 *      and maintains backups of the folder's contents
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <sys/types.h>
#include <map>
#include <queue>
#include <string>
#include <fstream>
#include "dirmond.h"

std::map<int, std::string> configs;
std::map< std::string, std::queue<std::string> > fileVers;
std::ofstream logfile;
std::string logFilename;
std::string versionsLoc;
std::string secureLoc;
bool isVerbose = false;

int main(int argc, char *argv[]){
    struct stat buff;
    if (!stat("cs3377dirmond.pid", &buff)){
        fprintf(stderr, "Error: Another process already exists, see 'cs3377dirmond.pid' for process ID\n");
        exit(EXIT_FAILURE);
    }

    parseCmdLine(argc, argv);
    parseConfigFile();

    isVerbose = configs[VERBOSE] == "T";
    logFilename = configs[LOGFILE];

    printConfigs();
    printf("Moving to '%s' for logging\n", logFilename.c_str());

    if (configs[DAEMON] == "T"){
        pid_t pid = fork();

        if (pid > 0){
            printf("Daemon started\n");
            exit(EXIT_SUCCESS);
        }
        else if (pid == 0){
            if (close(0) || close(1) || close(2)){
                endFail();
            }
            setsid();
        }
        else if (pid < 0) {
            perror("Error");
            exit(EXIT_FAILURE);
        }
    }

    logfile.open(logFilename.c_str(), std::ios::out);
    if (!logfile.is_open()){
        exit(EXIT_FAILURE);
    }

    std::ofstream pidfile;
    pidfile.open("cs3377dirmond.pid", std::ios::out);
    if (!pidfile.is_open()){
        logfile << "Error: " << strerror(errno) << std::endl;
        endFail();
    }

    pidfile << getpid() << std::endl;
    pidfile.close();

    int fd = inotify_init();
    if (fd < 0){
        logfile << "Error: " << strerror(errno) << std::endl;
        endFail();
    }

    int wd = inotify_add_watch(fd, configs[WATCHDIR].c_str(), IN_CLOSE_WRITE);
    if (wd < 0){
        logfile << "Error: " << strerror(errno) << std::endl;
        endFail();
    }

    size_t bufsiz = sizeof(struct inotify_event) + PATH_MAX + 1;
    struct inotify_event *event = (struct inotify_event *)malloc(bufsiz);

    versionsLoc = configs[WATCHDIR] + "/.versions";
    secureLoc = configs[WATCHDIR] + "/.secure";
    system(std::string("mkdir -p ").append(versionsLoc).c_str());
    system(std::string("mkdir -p ").append(secureLoc).c_str());

    if (isVerbose){
        logfile << "Searching for extra file copies from previous execution..." << std::endl;
    }
    removeOldFiles();

    signal(SIGINT, end);
    signal(SIGTERM, end);
    signal(SIGHUP, rereadConf);

    ssize_t ret;
    while ((ret = read(fd, event, bufsiz)) != 0){
        if (ret == -1){
            if (errno == EINTR){
                continue;
            }
            endFail();
        }
        logfile << "Detected modification of '" << event->name << "'" << std::endl;
        copyFile(event);
    }

    return 0;
}
