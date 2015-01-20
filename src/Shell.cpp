/*
 * RShell.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: john
 */

#include "Shell.h"
#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
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

	log("Starting shell")
	std::vector<std::vector<std::string> > execCommandSet;
	bool terminate = false;
	do{


		// Print a command line every time
		PrintCommandPrompt();
		// Read in the commands from the printed prompt
		std::string commands = ReadInCommands();
		log("About to parse commands")
		// Parse the strings into by ||,&&,;
		// Clear any commands left in buffer
		execCommandSet.clear();
		execCommandSet = ParseCommands(commands);
		// If there was one or more commands to execute, execute them
		bool success = true;
		if(execCommandSet.size() > 0){
			// loop through the commands to execute

			int size = execCommandSet.size();
			for(int i = 0; i < size; i++){
				log("Find exit status")
				if(NeedToExit(execCommandSet[i])){
					terminate = true;
					log("Going to exit the program")
					break;
				}else{
					// Execute the command
					if(!ExecuteCommand(execCommandSet[i])){
						success = false;

					}
					log("The execution finished: ")
					log(success)
				}
			}
		}

	}while(!terminate);


}

void Shell::PrintCommandPrompt(){
	log("Printing command line")
			//getcwd(mCurrentDir, sizeof(mCurrentDir));
			// clear all the tokens from the last input
			//	Tokens.clear();
			// Print the command line
			printf("%s$ ", "Commands Line");
	//Get the line that was entered

}



std::string Shell::ReadInCommands(){
	log("Reading in commands")
	int buffSize = 256;
	char *mBuffer = new char[buffSize];
	//std::cin.clear();
	//std::cin.ignore(10000, '\n');
	std::cin.getline(mBuffer, buffSize);
	// clean stdin
	std::cin.clear();
	return string(mBuffer);
}

vector<vector<string> > Shell::ParseCommands(string commandsStream){
	log("parsing commands")

	vector<string> commandStrings;
	vector<vector<string> > commandSet;

	// Break it up by tokens
	char *comm = new char;
	if(commandsStream.empty())
		return commandSet;

	log("copying strings")
	strcpy( comm, commandsStream.c_str());
	log(comm)
	log("Tokenzing")
	char* prog = strtok(comm, "&& || ; ");


	// Put all the tokens into a vector
	// This is where take each set of command by connectors
	/*
	 * Set of all exec commands
	 * {
	 * 	{Command set one }
	 * 	{Command set two}
	 * }
	 *Valid conectors are &&, ||, ;
	 */

	while (prog != NULL) {
		string tok = "token pulled: ";
		string pr = prog;
		tok += pr;
		log(tok)
		commandStrings.push_back(prog);
		if(pr == "&&" || pr == "||" || pr == ";"){
			// Push all the
			commandSet.push_back(commandStrings);
			commandStrings.clear();

		}
		prog = strtok(NULL, " ");


	}
	// Push back at least on set if the was a command to push
	if(commandStrings.size() > 0)
		commandSet.push_back(commandStrings);
// Debug command the command set
#ifdef DEBUG
	int size = commandSet.size();
	string message1 = "Command set size: " ;
	log(message1)
	log(size)
	for(int z = 0; z < commandSet.size(); z++){

			int sizze = commandSet[z].size();
			string message1 = "Command string size: " ;
			log(message1)
			log(sizze)
	}
#endif


	comm = 0;
	delete comm;


	return commandSet;
}


bool isConnector(string str){
	if((str.compare("&&")== 0) || (str.compare("||")== 0) || (str.compare(";")== 0)){
		log("A connector was found")
		return true;
	}
	return false;
}
void Shell::handleChildExecution(vector<string> command) {
	log("Executing child process")
	int size = command.size()+1;

	char** argv = new char*[size];

	char* str = new char;
	log("Deep copy paramaters")
	for(int i = 0; i < size-1; i++ ){
		//log(command[i].c_str())
		strcpy(str,command[i].c_str());
		argv[i] = new char;

		if(isConnector(command[i])){
			argv[i] = NULL;
			log("NULL")
		}else{

			strcpy(argv[i],str);
			log(str);
		}





		//log(argv[i])
		//if(isConnector(command[i])){
		//			str = NULL;
		//			log("Turning connector to NULL")
		//		}else{
		//			strcpy(strr, command[i].c_str());
		//		log(str)
		//			argv[i] = new char;
		//			strcpy(argv[i],strr);
		//			str = 0;
		//		}
		//log(argv[i])

	}
	log("About to dump commands")

	dumpCommands(argv, size);

	//argv[command.size()] = NULL;

	log("Executing: ")
	log(argv[0]);
	execvp(argv[0], argv);
}


bool Shell::handleParentExecution(pid_t pid,bool wait) {
	log("Parent execution after fork started");
		/**
		 * if somewhere there is a &
		 * ie. command -flags &
		 * the the parent should not wait on the child
		 *
		 */
		//wait = true;
		if(wait){
			log("Waiting on child process");
			int status;
			// wait for child to exit
			if (waitpid(pid, &status, 0) < 0) {
				log("An error occured while waiting for the child process");
				perror("An error occured while waiting for the child process");
				return false;
			}
			if (WIFEXITED(status)) {
				log("Child exit status = " << WEXITSTATUS(status))
			}
		}
		else{
			log("Not waiting on child execution")

		}
		log("Parent execution after fork finished successfully");

		return true;
}

bool Shell::ExecuteCommand(vector<string> commandVect){
	log("Forking process")

	// first we must fork
	pid_t PID = fork();
	if(PID == 0){
		// Child processprintf("In child process\n");
		handleChildExecution(commandVect);
	}
	else if(PID > 0){
		// Parent process
		return handleParentExecution(PID, true);
	}
	else{
		perror("Something bad happened");
		return false;

	}
	// After we fork we must execute the command
	// in the child process
	// the parent should wait for the child to finish
	// unless a & is encountered

	return true;
}
bool Shell::callCD(string path){

	string changeTo = "";
	// The first character was root
	if(path[0] == '/'){
			changeTo = "/";

	// The first char was a tilde
    // expand
	}else if(path[0] == '~'){
		changeTo = tildeExpansion() + path.substr(1);

	// It must be a realative path then.
	}else{
		changeTo = this->mCurrentDirectory + "/" + path;
	}

	if(chdir(changeTo.c_str())){
			chdir(this->mCurrentDirectory.c_str());
			return false;
	}

	return true;


}
string Shell::getCWD(){
	return get_current_dir_name();
}

string inline Shell::tildeExpansion(){
	return string(getenv("HOME")) + "/";
}
string inline Shell::getHomeDirectory(){
	return tildeExpansion();
}
Shell::~Shell() {
	// TODO Auto-generated destructor stub
}

