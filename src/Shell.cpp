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
#include <sstream>
#include <ctype.h>
#include <algorithm>

#include "log.h"
#include "SimpleGLibPipe.h"
#include "Tokenizer.h"

using std::string;
using std::vector;

// move these to a header

string IntToString(int num){


	std::stringstream ss;
	ss << num;
	return ss.str();
}




Shell::Shell() {
	// TODO Auto-generated constructor stub

}

bool Shell::NeedToExit(vector<string> commands) {
	if(commands[0] == "exit")
		return true;
	else
		return false;
}

void Shell::orConnector(bool& doExecution, bool& success, int execi, int size,
		std::vector<std::vector<std::string> > execCommandSet,
		bool& resetExecution) {
	log("Found a || connector")
	doExecution = !success;
	string yes = "Will execute beacuse prevois command failed";
	string no = "Not executing next command because prevois command succeeded";
	string logs = ((doExecution == true) ? yes : no);
	log(logs)
	// if it turn out that we do not execute the next command
	// we dont need to execute until a ; is found
	if (!doExecution) {
		log("Finding next vector to execute")
		for (int findNextEnd = execi; findNextEnd < size; findNextEnd++) {
			log("Checking vector " + IntToString(execi))
			vector < string > tmp = execCommandSet[execi]; //
			string connector = tmp[tmp.size() - 1];
			log(connector)
			// once we find the connector, advance execi to the vector
			// following that connector
			if (connector.compare(";") == 0) {
				execi = (findNextEnd);
				log("Found next ; at " + IntToString(execi))
				// since the executin flag is false, break out
				// of the loop and let the outer loop complete
				// execution is phrohibited
				resetExecution = true;
				// break out from the loop
				break;
			}

		}

	}
}

void Shell::Run(){

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
		// set initial success status to true
		bool success = true;
		// set initial execution status to true
		bool doExecution = true;

		bool resetExecution = false;
		// If there was one or more commands to execute, execute them
		if(execCommandSet.size() > 0){

			resetExecution = false;
			int size = execCommandSet.size();

			log("Looping through " + IntToString(size) + " command sets" )
			// loop through the commands to execute
			for(int execi = 0; execi < size; execi++){

				log("Finding exit status")
				if(NeedToExit(execCommandSet[execi])){
					terminate = true;
					log("Going to exit the program")
					//exit(0);
					return; // This is okay because there is no cleanup needed
				}else{
					// Execute the command
					log("Beginning execution phase for command " + IntToString(execi))
					// grab the commands set to execute
					vector<string> command = execCommandSet[execi];

					// if commandSet had more than one command, was the last string
					// on the last command a connector
					// we should only check the connector if there is only more than one
					// set of commands to execute
					if((execi > 0) && (size > 1)){
						log("Searching for a connector")
						// Grab a ptr to the prevois vector
						// This is why we must have more than one vector
						vector<string> tmp = execCommandSet[execi-1]; // I really hate having to do this line
						string connector = tmp[tmp.size()-1];
						// if the connector was a ; execute the command
						// if it was || execute only if the last one failed
						// if it was a && execute if it succeded
						if(connector.compare("||") == 0) {
							orConnector(doExecution, success, execi, size,
									execCommandSet, resetExecution);
						}
						else if(connector.compare("&&") == 0){
							log("Found a && connector")
							doExecution = success;
							string yes = "Will execute beacuse prevois command succeded";
							string no = "Not executing next command because prevois command failed";
							string logs = ((doExecution == true) ? yes : no);
							log(logs)


						}
						else if(connector.compare(">>") == 0){
							log("Found a >> connector")
									// handle right redirection with append

						}
						else if(connector.compare(">") == 0){
							log("Found a > connector")
									// handle right redirection without append

						}
						else if(connector.compare("<") == 0){
							log("Found a < connector")
									// handle input redirection

						}

					}


					if(doExecution){
						log("Will execute a command")
						if(!ExecuteCommand(command))
							success = false;

					}
					// we should mark that the next command is allowed to execute
					if(resetExecution){
						log("Resetting execution status")
						doExecution = true;
						// set this status falsg to false
						// so the executin is controlled by
						// connectors again
						resetExecution = false;
					}
					log("The execution finished: ")
					string tmp = (success == true ? "true" : "false");
					log(tmp)
				}


			}
		}

	}while(!terminate);
}
void Shell::StartShell(int argc, char **argv){

	log("Starting shell")
	Run();



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

void Shell::PrintCommandPrompt(){
	log("Printing command line")
	// getcwd(mCurrentDir, sizeof(mCurrentDir));
	// clear all the tokens from the last input
	//	Tokens.clear();
	// Print the command line
	char * buff = new char[256];
	char *username = new char[256];
	string name = "unknown";
	string host = "unknown";
	cuserid(username);
	if(username != NULL){
		name = username;
	}
	if(!gethostname(buff, 256)){
		host = buff;
	}
	printf("%s@%s$ ", name.c_str(), host.c_str() );
	//Get the line that was entered

}



vector<vector<string> > Shell::ParseCommands(string commandStream){
	log("parsing commands")


	vector<vector<string> > commandSet;

	// Check if there are commands to parse
	if(commandStream.empty())
		return commandSet;

	// resize the array to remove anything after the #
	size_t comment = commandStream.find_first_of('#');
	if(comment != string::npos){
		if(comment <= 0){
			commandStream.resize(0);
		}else{

			commandStream.resize(comment-1);
		}


	}
	// log the whole stream to parse the kick of the parser
	log(commandStream)
	commandSet = Tokenizer::TokenizeCommandStream(commandStream);

	return commandSet;
}




bool isConnector(string str){
	if((str.compare("&&")== 0) || (str.compare("||")== 0) || (str.compare(";")== 0)){
		log("A connector was found")
		return true;
	}
	return false;
}

// Accepts a command vector
// commands are in the form
// [command] [flag] --- [flag] [connector]
void Shell::handleChildExecution(vector<string> commandVector) {

	vector<string> command = commandVector;
	log("Executing child process")
	int size = command.size();
	log(size)
	log("commands Found")
	// Create an array 1 great than size of commandVector
	char** argv = new char*[size+1];
	for(int i = 0; i < size; i++){
			// create new empty array
			argv[i] = new char;
	}

	// Null the last command
	argv[size] = NULL;

	log("Deep copy parameters")
	for(int i = 0; i < size; i++ ){
		log("Copying command " + IntToString(i))

		string scommand = command[i];
		log(scommand)
		if(isConnector(scommand)){
			argv[i] = NULL;
			log("NULL")
		}else{
			log("about to copy string")
			char *str = new char;
			strcpy(str,scommand.c_str());
			//log(str);
			argv[i] = str;
			log(argv[i]);
			//strcpy(argvi)
		}
	}

	log("About to dump commands")
    // Last command is always NULL
	dumpCommands(argv, size-1);

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
			else{
				// The child did not exit properly
				perror("The child did not exit properly");
				return false;

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
		return true;
	}
	else if(PID > 0){
		// Parent process
		return handleParentExecution(PID, true);
	}
	else{
		perror("Something bad happened with fork");
		return false;

	}
	// After we fork we must execute the command
	// in the child process
	// the parent should wait for the child to finish
	// unless a & is encountered

	return false;
}
Shell::~Shell() {
	// TODO Auto-generated destructor stub
}

