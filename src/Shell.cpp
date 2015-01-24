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
#include "log.h"

using std::string;
using std::vector;

// move this to header
vector<vector<string> > splitToVector(string completeStream, string delim);

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
					vector<string> command = execCommandSet[i];
					bool doExecution = true;
					// if i had more than one set, was the last string a connector
					if(i > 0){
						string connector = command[command.size()-1];
						// if the connector was a ; execute the command
						// if it was || execute only if the last one failed
						// if it was a && execute if it succeded

						if(connector.compare("||") == 0){

							doExecution = !success;
						}
						if(connector.compare("&&") == 0){

							doExecution = success;
						}

					}

					if(doExecution){
						log("Will execute a command")
						if(!ExecuteCommand(command))
							success = false;

					}
					log("The execution finished: ")
					string tmp = (success == true ? "true" : "false");
					log(tmp)
				}
			}
		}

	}while(!terminate);


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
	int comment = commandStream.find_first_of('#');
	if(comment != string::npos)
		commandStream.resize(comment-1);

	// log the whole stream to parse the kick of the parser
	log(commandStream)
	commandSet = TokenizeCommandStream(commandStream);

	return commandSet;
}


vector<vector<string> > Shell::TokenizeCommandStream(string commandStream) {

	log("Tokenizing commands")
	vector<vector<string> > completeENDedCommands = TokenizeToLogicalEND(commandStream);
	dumpEntireCommandVector(completeENDedCommands);
	vector<vector<string> > completeORedCommands = TokenizeToLogicalOR(completeENDedCommands);
	dumpEntireCommandVector(completeORedCommands);
	vector<vector<string> > completedANDCommands = TokenizeToLogicalAND(completeORedCommands);
	dumpEntireCommandVector(completedANDCommands);
	//vector<vector<string> > completedConnecterizedCommands = TokenizeToConnectors(completedANDCommands);
	vector<vector<string> > completedTokenCommands = TokenizeToSpaces(completedANDCommands);
	log("There is a total of ")
	log(completedTokenCommands.size())
	log("Commands to run")
	cout << std::endl << std::endl;
	dumpEntireCommandVector(completedTokenCommands);
	cout << std::endl << std::endl;
	return completedTokenCommands;

}
// Tokenize a string by the end operator ;
// All vectors are size 2
// except the last command, or if there is a single, which can be size one
// string in the form of [command and flags and othe connectors][connector]
vector<vector<string> > Shell::TokenizeToLogicalEND(string completeStream){
	log("Tokenizing ends")
	vector<vector<string> > commandSet = splitToVector(completeStream, ";");

	return commandSet;

}

// Tokenize a string by the end operator ';'
// All vectors are size 2
// except the last command, or if there is a single, which can be size one
// string in the form of [command and flags and othe connectors][connector]
vector<vector<string> > splitToVector(string completeStream, string delim){

	vector<string>  commands;
	vector<vector<string> > commandSet;

	int foundAt = -1;
	int start = 0;
	int end = 0;
	char *commandString;
	int pass = 0;
	do{

		// First the first occurence of the delim
		foundAt = end = completeStream.find(delim, start);
		// cut the string from the start of the token to the delim
		// or from the start of the token to the end of the string
		// if no delim is found

		log("First string")
		log(start)
		log(foundAt)
		if(foundAt < 0){
			end = completeStream.size();
		}
		log(end)

		int sSize = end-start;
		commandString = new char[256];

		if((pass > 0 )&& (foundAt < 0 ) && (start > end) )
			break;

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
		pass++;
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
// Tokenize a string by the logical OR operator ||
// All vectors are size 2
// except the last command, or if there is a single, which can be size one
// string in the form of [command and flags and other connectors][connector]
vector<vector<string> > Shell::TokenizeToLogicalOR(vector<vector<string> > commandVector){
	log("Tokenizing or")

		vector<string> spaceVector;
			vector<string> tmpVector;
			vector<vector<string> > commandSet;
			char* buffer = new char[256];

				for(int i = 0; i < commandVector.size(); i++){
					log("Looping through vectors to tokenize")
					// loop through the connectorized vectors
					spaceVector = commandVector[i];
				//	for(int z = 0; z < spaceVector.size(); z++){
						log("Tokenizing a OR_Vector")
						// tokenize each string


						strcpy(buffer,spaceVector[0].c_str());
						char* prog = strtok(buffer, "||");
						// Put all the tokens into a vector
						do{
							if(prog == NULL)
								break;

							if(prog != NULL){
								log(prog)

							}
							//while (prog != NULL) {

							// push back the program token
							tmpVector.push_back(prog);
							// grab the next string up to token
							prog = strtok(NULL, "||");

							// check to see if there is another token
							if(prog != NULL){
								// if there was push back anothere connector
								log("||")
								tmpVector.push_back("||");
							}
							// push this set into the larger set
							commandSet.push_back(tmpVector);
							// clear the tmp vector
							tmpVector.clear();
					//	}
						// Only continue if there is another
						}while(prog != NULL);
						//tmpVector.push_back(prog);
						//commandSet.push_back(tmpVector);
						//
						// Push the connector
						//if(spaceVector.size() > 1)
						//	tmpVector.push_back(spaceVector[1]);
						// push the tokenized vector
						//commandSet.push_back(tmpVector);
						// clear the token vecotr for reuse
						tmpVector.clear();

				//	}



				}
				buffer = 0;
				delete buffer;

				log("found ")
				log(commandSet.size())
				log("command sets")


				return commandSet;



}
// Tokenize a string by the logical AND operator &&
// All vectors are size 2
// except the last command, or if there is a single, which can be size one
// string in the form of [command and flags and othe connectors][connector]
vector<vector<string> > Shell::TokenizeToLogicalAND(vector<vector<string> > parseVector){
	log("Tokenizing and")

	vector<string> spaceVector;
	vector<string> tmpVector;
	vector<vector<string> > commandSet;
	char* buffer;

		for(int i = 0; i < parseVector.size(); i++){
			log("Looping through vectors to ANDerize")
			// loop through the connectorized vectors
			spaceVector = parseVector[i];
			log(parseVector[i][0])
			//for(int z = 0; z < spaceVector.size(); z++){
				log("Tokenizing a spaceVector")
				// tokenize each string
				buffer = new char[256];
				strcpy(buffer,spaceVector[0].c_str());
				// grab the token string
				char* prog = strtok(buffer, "&&");
				//log(buffer)

				// if the string grabed no token
				// the orignal string had no token
				// so grab that origianl string
				// and push it and go to next vector
				if(prog == NULL){
					log(buffer)
					tmpVector.push_back(buffer);
					commandSet.push_back(tmpVector);
					tmpVector.clear();
					continue;

				}

				// Put all the tokens into a vector if there was a token
				do{
					if(prog == NULL)
						break;

					if(prog != NULL){
						log(prog)
					}
					//while (prog != NULL) {

					// push back the program token
					tmpVector.push_back(prog);
					// grab the next string up to token
					prog = strtok(NULL, "&&");

					// check to see if there is another token
					if(prog != NULL){
						// if there was push back anothere connector
						log("&&")
						tmpVector.push_back("&&");
					}
					else{
						// if it was NULL at this point it means
						// that there was not another token or string
						// add back on the original connector
						// if there was one
						if(spaceVector.size() > 1){
							log(spaceVector[1])
							tmpVector.push_back(spaceVector[1]);
						}

					}
					// push this set into the larger set
					commandSet.push_back(tmpVector);
					// clear the tmp vector
					tmpVector.clear();
			//	}
				// Only continue if there is another
				}while(prog != NULL);
				// Push the connector
				//if(spaceVector.size() > 1)
				//	tmpVector.push_back(spaceVector[1]);
				// push the tokenized vector
				//commandSet.push_back(tmpVector);
				// clear the token vecotr for reuse
				tmpVector.clear();
				buffer = 0;
				delete buffer;

			//}



		}
		buffer = 0;
		delete buffer;

		log("found ")
		log(commandSet.size())
		log("command stream")


		return commandSet;

	return parseVector;
}
// Tokenize a string by spaces
// All input vectors are size 2
// except the last command, or if there is a single, which can be size one
// strings in the form of [command and flags and othe connectors][connector]
// after this command finishes vectors are in the form
// [command] [flag1] [flag2] .... [connector]
vector<vector<string> > Shell::TokenizeToSpaces(vector<vector<string> > completeStream){
	log("Tokenizing spaces")

	vector<string> spaceVector;
	vector<string> tmpVector;
	vector<vector<string> > commandSet;
	char* buffer = new char[256];

	for(int i = 0; i < completeStream.size(); i++){
		log("Looping through vectors to spacerize")
		// Grab the vector to tokenize by space
		spaceVector = completeStream[i];
		log(spaceVector[0]);

			log("Tokenizing a vector for spaces")
			// Grab the string to spacerize
			strcpy(buffer,spaceVector[0].c_str());
			char* prog = strtok(buffer, " ");
				// Put all the tokens into a vector
			//log(buffer)

			// if the string grabed had no tokens
			// the orignal string had no token
			// so grab that original string
			// and push it and go to next vector
			if(prog == NULL){
				log(buffer)
				tmpVector.push_back(buffer);
				commandSet.push_back(tmpVector);
				tmpVector.clear();
				continue;

			}
			// The data set had a token space

			while (prog != NULL) {
				// push back a the program token
				tmpVector.push_back(prog);


				log(prog)
				prog = strtok(NULL, " ");
				// clear the tmp vector
				//tmpVector.clear();

			}
			//readd the connector if it existed
			if(spaceVector.size() > 1){
				log(spaceVector[1])
				tmpVector.push_back(spaceVector[1]);
			}
			//

			// push this tokenized set into the larger set
			commandSet.push_back(tmpVector);

			tmpVector.clear();



	}
	buffer = 0;
	delete buffer;
	log("found ")
	log(commandSet.size())
	log("command stream")
	return commandSet;

}

string IntToString(int num){


	std::stringstream ss;
	ss << num;
	return ss.str();
}
void Shell::dumpEntireCommandVector(vector<vector<string> >& commandSet) {

	cout << std::endl << std::endl;
	log("Entire commandset")
	for(int i = 0; i < commandSet.size(); i++){
		string screen = "commandset :";
		log(screen)
		vector<string> commandString = commandSet[i];
		string command = IntToString(commandString.size()) + ": ";
		for(int z = 0; z < commandString.size(); z++){
			command += commandString[z] + " ";
		}
		log(command)



	}
	cout << std::endl << std::endl;
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

