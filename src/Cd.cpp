/*
 * Cd.cpp
 *
 *  Created on: Feb 25, 2015
 *      Author: john
 */

#include "Cd.h"
//#include "log.h"

#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <stdio.h>

using std::string;
using std::vector;



/**
 * Exits if a syntax error occured
 * Returns "" if a no expansion was needed
 * or the expanded path and the command
 *
 */
string Cd::handleDotandTilde(const vector<string> tokens){

	const string commandToken = tokens[0];
	const string cwd = tokens[1];
	// Check to see if it is a dotr case
	if (commandToken[0] == '.') {
		//Is the second character a dot
		if (commandToken[1] == '.') {
			// Do we have a slash
			if (commandToken[2] == '/') {
				// A command was invoked in
				// the manner of
				// ../PATH_TO_COMMAND
				// Get the the current working
				// directory
				string tmp = string(cwd);
				tmp += "/" + commandToken;
				return tmp;

			}else{
				// Improperly formed command call
				exit(EXIT_FAILURE);
			}
			// is the second char a /
		} else  {
			if(commandToken[1] == '/'){
				// A command was invoked in
				// the manner of
				// ./COMMAND
				// Get the the current working
				// directory
				string pwd = string(cwd);
				// strip the . away
				string sub = commandToken.substr(1);
				// returns the CWD/COMMAND, ie,
				// the full path, not nessicarly absolute
				return pwd + sub;
			}else{
				// Improperly formed command call
				exit(EXIT_FAILURE);
			}
		}
	} else if (commandToken[0] == '~') {

		if (commandToken[1] == '/') {
			// Strip the ~ off the command
			string sub = commandToken.substr(2);
			return tildeExpansion() + sub;
		} else {
			exit(EXIT_FAILURE);
		}
	}
	return "";
}
string Cd::tildeExpansion(){
	return string(getenv("HOME")) + "/";
}

/**
 * Takes a vector of strings
 * This first vecotor is the current directory
 * The second vector is the path to change to
 */
bool Cd::callCD(const vector<string> *tokens){
	//log("callCD");
	string newDir = tokens->at(1);
	if(tokens->size() > 1){
		// it was change to root
		if(tokens->at(1)[0] == '/')
			newDir = tokens->at(1);
		// it was a tilde expansion
		else if(tokens->at(1)[0] == '~'){
			newDir = tildeExpansion() + tokens->at(1).substr(1);
		}
		else
			//
			newDir += "/" + tokens->at(1);
	}else{
		// no change to directory was supplied, return to HOME
		newDir = getenv("HOME");
	}
	//log("Changing the current dir: " + tokens[0] + " to " + newDir);
	// chdir return 0 on success
	if(chdir(newDir.c_str())){
		perror("chdir failed:");
		return false;
		//chdir(tokens[0].c_str());
	}

	return true;

}

