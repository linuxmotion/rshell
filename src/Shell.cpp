/*
 * RShell.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: john
 */

#include "Shell.h"
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include "log.h"

using std::string;
using std::vector;

Shell::Shell() {
	// TODO Auto-generated constructor stub

}

bool Shell::NeedToExit(vector<string> commands) {
	if(commands[0] == "exit")
		return true;
	else
		return false;
}

void Shell::StartShell(int argc, char **argv){

	std::vector<std::string> execCommands;
	bool terminate = false;
	do{
		log("Starting shell")

		PrintCommandPrompt();
		std::string commands = ReadInCommands();
		log("About to parse commands")
		execCommands = ParseCommands(commands);
		if(execCommands.size() > 0){
			log("Find exit status")
			terminate = NeedToExit(execCommands);
		}

	}while(terminate == false);


}

void Shell::PrintCommandPrompt(){
	log("Printing command line")
		//getcwd(mCurrentDir, sizeof(mCurrentDir));
			// clear all the tokens from the last input
		//	Tokens.clear();
			// Print the command line
			printf("%s$ ", "Commands Line$");
			//Get the line that was entered

}

std::string Shell::ReadInCommands(){
	log("Reading in commands")
		int buffSize = 256;
		char *mBuffer = new char[buffSize];
		std::cin.getline(mBuffer, buffSize);
			// clean stdin
			//cin.clear();
	return string(mBuffer);
}

std::vector<string> Shell::ParseCommands(std::string commandsStream){
	log("parsing commands")

	std::vector<string> commandStrings;
	// Break it up by tokens
	char *comm = new char;
	if(commandsStream.empty())
		return commandStrings;

	log("copying strings")
	strcpy( comm, commandsStream.c_str());
	log(comm)
	log("Tokenzing")
	char* prog = strtok(comm, " ");
	log("First token pulled")
	log(&prog)
	// Put all the tokens into a vector

	while (prog != NULL) {
		commandStrings.push_back(prog);
		prog = strtok(NULL, " ");
	}

	comm = 0;
	delete comm;


	return commandStrings;
}


bool Shell::ExecuteCommand(std::string* commandsArray, int commandc){
	log("executing commands")
	return false;
}

Shell::~Shell() {
	// TODO Auto-generated destructor stub
}

