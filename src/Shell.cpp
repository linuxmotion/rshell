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

vector<vector<string> > Shell::TokenizeCommandStream(string commandStream) {

	log("Tokenizing commands")
	vector<vector<string> > completeENDedCommands = TokenizeToLogicalEND(commandStream);
	vector<vector<string> > completeORedCommands = TokenizeToLogicalOR(completeENDedCommands);
	vector<vector<string> > completedANDCommands = TokenizeToLogicalAND(completeORedCommands);
	vector<vector<string> > completedConnecterizedCommands = TokenizeToConnectors(completedANDCommands);
	vector<vector<string> > completedTokenCommands = TokenizeToSpaces(completedConnecterizedCommands);

	return completedTokenCommands;

}
vector<vector<string> > splitToVector(string completeStream, string delim){

	vector<string>  commands;
	vector<vector<string> > commandSet;

	int foundAt = -1;
	int start = 0;
	int end = 0;
	char *commandString;
	do{

		// First the first occurence of the delim
		foundAt = end = completeStream.find(delim, start);
		// cut the string from the start of the token to the delim
		// or from the start of the token to the end of the string
		// if no delim is found

		log("First string")
		log(start)
		if(foundAt < 0)
			end = completeStream.size();
		log(end)

		int sSize = end-start;
		commandString = new char[256];

		completeStream.copy(commandString, sSize, start);
		log(string(commandString))
		//push back the command stream between tokens
		commands.push_back(string(commandString));
		if(foundAt < 0){
			commandSet.push_back(commands);
			break;
		}

		start = start + sSize ;

		commandString = 0;
		delete commandString;

		commandString = new char[256];

		// Next grab the delim
		log("Second string: delim")
		log(start)
		end = start + delim.size();
		log(end)

		sSize = end-start;
		completeStream.copy(commandString, sSize, start );
		// push back the deliminator
		log(string(commandString))
		commands.push_back(string(commandString));

		commandString = 0;
		delete commandString;


		// next start of token happens after the delim
		start = end + delim.size();
		// push back the tokenized vector

		commandSet.push_back(commands);
		// clear the commands vector, in prep for next loop
		commands.clear();

		// Do this while there is a deliminator
	}while(foundAt > 0);
	commandString = 0;
	delete commandString;

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

	return commandSet;
}

vector<vector<string> > Shell::TokenizeToConnectors(vector<vector<string> > completeStream){
	log("Connectorizing")


	return completeStream;

}vector<vector<string> > Shell::TokenizeToSpaces(vector<vector<string> > completeStream){
	log("Tokenizing spaces")

	vector<string> spaceVector;
	vector<string> tmpVector;
	vector<vector<string> > commandSet;
	char* buffer = new char[256];

	for(int i = 0; i < completeStream.size(); i++){
		log("Looping through vectors to tokenize")
		// loop through the connectorized vectors
		spaceVector = completeStream[i];
		for(int z = 0; z < spaceVector.size(); z++){
			log("Tokenizing a vector")
			// tokenize each string
			strcpy(buffer,spaceVector[0].c_str());
			char* prog = strtok(buffer, " ");
				// Put all the tokens into a vector

			while (prog != NULL) {
				// push back a new token
				tmpVector.push_back(prog);
				log(prog)
				prog = strtok(NULL, " ");
			}
			// Push the connector
			if(spaceVector.size() > 1)
				tmpVector.push_back(spaceVector[1]);
			// push the tokenized vector
			commandSet.push_back(tmpVector);
			// clear the token vecotr for reuse
			tmpVector.clear();

		}



	}
	buffer = 0;
	delete buffer;


	return commandSet;

}
vector<vector<string> > Shell::TokenizeToLogicalEND(string completeStream){
	log("Tokenizing ends")
	vector<vector<string> > commandSet = splitToVector(completeStream, ";");

	return commandSet;

}

vector<vector<string> > Shell::TokenizeToLogicalAND(vector<vector<string> > parseVector){
	log("Tokenizing and")
	vector<string>  commands;
	vector<vector<string> > commandSet;

	return parseVector;
}
vector<vector<string> > Shell::TokenizeToLogicalOR(vector<vector<string> > commandVector){
	log("Tokenizing or")

	int size = commandVector.size();
	for(int i = 0; i < size; i++){



	}

	return commandVector;
}
vector<vector<string> > Shell::ParseCommands(string commandStream){
	log("parsing commands")


	vector<vector<string> > commandSet;

	// Check if there are commands to parse
	if(commandStream.empty())
		return commandSet;

	// resize the array to remove anything after the #
	int comment = commandStream.find_first_of('#');
	if(comment != string::npos)
		commandStream.resize(comment-1);

	// log the whole stream to parse the kick of the parser
	log(commandStream)
	commandSet = TokenizeCommandStream(commandStream);

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
	log(size)
	log("commands Found")
	char** argv = new char*[size];

	// Null the last command
	argv[size-1] = NULL;

	char* str;
	log("Deep copy paramaters")
	for(int i = 0; i < size-1; i++ ){
		//log(command[i].c_str())
		str = new char[256];
		if(command[i] == " "){
			argv[i] = NULL;
			log("NULL")
		}
		else{
			strcpy(str,command[i].c_str());
			argv[i] = new char;
			if(isConnector(command[i])){
				argv[i] = NULL;
				log("NULL")
			}else{

				strcpy(argv[i],str);
				log(str);
			}
		}



		// Clear the string
		strcpy(str,"");

		str = 0;
		delete str;

	}
	log("About to dump commands")
    // Last command is always NULL
	dumpCommands(argv, size);

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

