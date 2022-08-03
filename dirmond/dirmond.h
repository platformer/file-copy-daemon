/*
 * Filename     dirmond.h
 * Date         05/09/2021
 * Author       Andrew Sen
 * Email        ats190003@utdallas.edu
 * Course       CS 3377.0W6 Spring 2021
 * Version      1.0
 * Copyright    2021, All Rights Reserved
 *
 * Description
 * 	
 *      Header file for assignment 6
 *      Contains global variables and functions
 *          used in other source files
 */

#ifndef DIRMOND_H
#define DIRMOND_H

#include <sys/inotify.h>
#include <map>
#include <queue>
#include <fstream>
#include <string>

enum Args {DAEMON, CONFFILE, VERBOSE, LOGFILE, PASSWORD, NUMVER, WATCHDIR};
extern std::map<int, std::string> configs;
extern std::ofstream logfile;
extern std::string logFilename;
extern std::string versionsLoc;
extern std::string secureLoc;
extern std::map< std::string, std::queue<std::string> > fileVers;
extern bool isVerbose;

void parseCmdLine(int, char**);
void parseConfigFile();
void reparseConfigFile();
void printConfigs();
void logConfigs();
void end(int);
void endFail();
void rereadConf(int);
void copyFile(struct inotify_event*);
void removeOldFiles();

#endif
