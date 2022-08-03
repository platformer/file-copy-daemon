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
#include "dirmond.h"

void parseCmdLine(int argc, char *argv[]){
    TCLAP::CmdLine cmd("cs3377dirmond Directory Monitor Daemon", ' ', "1.0");

    TCLAP::UnlabeledValueArg<std::string> conffileArg("conffile", "The name of the configuration file. Defaults to cs3377dirmond.conf", false, "cs3377dirmond.conf", "config filename", false);
    cmd.add(conffileArg);

    TCLAP::SwitchArg daemonFlag("d", "daemon", "Run in daemon mode (forks to run as a daemon).", cmd, false);
   
    cmd.parse(argc, argv);

    configs[DAEMON]   = daemonFlag.getValue()? "T" : "F";
    configs[CONFFILE] = conffileArg.getValue();
}
