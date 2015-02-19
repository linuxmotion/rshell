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

						}
						else if(connector.compare(">") == 0){
							log("Found a > connector")

						}
						else if(connector.compare("<") == 0){
							log("Found a < connector")

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
	commandSet = TokenizeCommandStream(commandStream);

	return commandSet;
}


vector<vector<string> > Shell::TokenizeCommandStream(string commandStream) {

	log("Tokenizing commands")
	vector<vector<string> > completeENDedCommands = TokenizeToLogicalEND(commandStream);
	dumpEntireCommandVector(completeENDedCommands);
	vector<vector<string> > completeORedCommands = TokenizeToLogicalOR(completeENDedCommands);
	completeENDedCommands.clear();
	dumpEntireCommandVector(completeORedCommands);
	vector<vector<string> > completedANDCommands = TokenizeToLogicalAND(completeORedCommands);
	dumpEntireCommandVector(completedANDCommands);
	vector<vector<string> > completedRightRedirCommands = TokenizeVector(completedANDCommands, ">");
	vector<vector<string> > completedLeftRedirCommands = TokenizeVector(completedRightRedirCommands, "<");

	vector<vector<string> > completedTokenCommands = TokenizeToSpaces(completedLeftRedirCommands);
	log("There is a total of ")
	log(completedTokenCommands.size())
	log("Commands to run")
	dumpEntireCommandVector(completedTokenCommands);

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
vector<vector<string> > Shell::splitToVector(string completeStream, string delim){

	vector<string>  commands;
	vector<vector<string> > commandSet;
	log(completeStream)

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
		commandString = new char[sSize+1];
		for(int i = 0; i < (sSize+1); i++){
			commandString[i] = '\0';
		}

		if(foundAt < 0 ){
			if(start > end)
				break;
			log("Pushing back final string")
			completeStream.copy(commandString, sSize, start);
			log(string(commandString))
			//push back the command stream between tokens
			commands.push_back(string(commandString));
			commandSet.push_back(commands);
			break;
		}

		completeStream.copy(commandString, sSize, start);
		log(string(commandString))
		//push back the command stream between tokens
		commands.push_back(string(commandString));
		if(foundAt < 0){
			commandSet.push_back(commands);
			break;
		}

		start = foundAt;

		log(start)
		char c = commandString[0];
		for(int i = 0; c != '\0'; i++){
			c = commandString[i];
			log(c)

		}
		//}

		commandString = 0;
		delete commandString;

		commandString = new char[256];
		for(int i = 0; i < 256; i++){
			commandString[i] = '\0';
		}

		// Next grab the delim
		log("Second string: delim")
		log(start)
		log(delim.size());
		end = start + delim.size();
		log(end)

		sSize = delim.size();// end-start;
		if((delim.size() == 1 )&& (completeStream[start] ==  delim[0]) && (completeStream[start+1] ==  delim[0])){
			log("double delim")
			sSize++;
		}


		completeStream.copy(commandString, sSize, start );
		c = commandString[0];
		int i = 0;
		while( c != '\0'){
				log(commandString[i])
				c = commandString[i];
				i++;
		}
		// push back the deliminator
		log("pushing back deliminator")

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
		//pass++;
	}while(foundAt > 0);
	commandString = 0;
	delete commandString;

	dumpEntireCommandVector(commandSet);

	return commandSet;
}

// Tokenize a string by the logical OR operator ||
// All vectors are size 2
// except the last command, or if there is a single, which can be size one
// string in the form of [command and flags and other connectors][connector]
vector<vector<string> > Shell::TokenizeVector(vector<vector<string> > commandVector, string tok){
	string message = "Tokenizing vector by " + tok;
	log(message)

			vector<string> *spaceVector;
			vector<vector<string> > commandSet;
			//if(commandVector.size() <= 1 ){
			//	commandSet = commandVector;
			//	return commandSet;
			//}

			char* buffer = new char[256];

			log("will tokenize " + IntToString(commandVector.size()) + " vectors")
			// loop trough our set of commands
			for(unsigned int i = 0; i <= commandVector.size()-1; i++){
				log("Looping through vectors to tokenize")
				// loop through the connectorized vectors
				log(commandVector.at(i).at(0))

				spaceVector = &commandVector.at(i); //[stuff][connector]
				log("Tokenizing " + IntToString(i) + " vector")
				vector<string> flattencommand;


				// tokenize each string
				log(spaceVector->at(0))

				// split this vector by delim
				vector<vector<string> > commands = splitToVector(spaceVector->at(0), tok); // [stuff]
				//dumpEntireCommandVector(commands);
				log("getting commands")
				// loop through the return vectors
				// get the vector reference

				for(int j = 0; j < commands.size(); j++){
					vector<string> *command = &commands.at(j);//[stuff][connector]
					log("Got the reference")
					for(signed int z = 0; z < command->size(); z++){
						// push back each string
						log("Pushing back command")
						string s = command->at(z);
						log(s)
						flattencommand.push_back(command->at(z));

					}
					// add back on the deliminator
					log("add back delim if there was one")
					if(spaceVector->size() > 1)
						flattencommand.push_back(spaceVector->at(1));

					log("Dumping the flattened commands")
					for(signed int z = 0; z < flattencommand.size(); z++){
						// push back each string
						log(flattencommand[z]);
					}
					commandSet.push_back(flattencommand);
					flattencommand.clear();
					//log(IntToString(i))

				}



			}
			buffer = 0;
			delete buffer;

			dumpEntireCommandVector(commandSet);
			log("found ")
			log(commandSet.size())
			log("command sets")


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

				for(unsigned int i = 0; i < commandVector.size(); i++){
					log("Looping through vectors to ORize")
					// loop through the connectorized vectors
					spaceVector = commandVector[i];
				//	for(int z = 0; z < spaceVector.size(); z++){
						log("Tokenizing a OR_Vector")
						// tokenize each string


						strcpy(buffer,spaceVector[0].c_str());
						char* prog = strtok(buffer, "||");
						// Put all the tokens into a vector
						// if the string grabed no token
						// the orignal string had no token
						// so grab that original string
						// and push it and go to next vector

		//				if(prog == NULL){
		//					log(buffer)
		//					tmpVector.push_back(buffer);
		//					commandSet.push_back(tmpVector);
		//					tmpVector.clear();
		//					continue;
		//				}

						do{
							if(prog == NULL){

								break;
							}

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
								// if there was push back another connector
								log("||")
								tmpVector.push_back("||");
							}else{
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

		for(unsigned int i = 0; i < parseVector.size(); i++){
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

	for(unsigned int i = 0; i < completeStream.size(); i++){
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


void Shell::dumpEntireCommandVector(vector<string> & commandSet) {

#ifdef DEBUG
	vector<vector<string> > tmp;
	tmp.push_back(commandSet);
	dumpEntireCommandVector(tmp);
#endif
}
void Shell::dumpEntireCommandVector(vector<vector<string> >& commandSet) {

#ifdef DEBUG
	std::cout << std::endl << "Start dump"<< std::endl ;
	log("Entire commandset " + IntToString(commandSet.size()))
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
	std::cout << "End dump" << std::endl << std::endl;
#endif
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

	return false;
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

