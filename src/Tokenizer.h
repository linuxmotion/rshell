/*
 * Tokenizer.h
 *
 *  Created on: Feb 20, 2015
 *      Author: john
 */

#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include <vector>
#include <string>
#include <string.h>
#include <iostream>
using std::cout;
using std::endl;

#include "log.h"

using std::vector;
using std::string;

class Tokenizer{

public:
	static vector<vector<string> > TokenizeCommandStream(string commandStream) {

	log("Tokenizing commands")
	vector<vector<string> > VectorizedTokens;

	VectorizedTokens = TokenizeToLogicalEND(commandStream);
	TokenizeVector(&VectorizedTokens, "|");
	TokenizeToLogicalOR(&VectorizedTokens);
	TokenizeToLogicalAND(&VectorizedTokens);
	TokenizeVector(&VectorizedTokens, ">");
	TokenizeVector(&VectorizedTokens, "<");
	TokenizeToSpaces(&VectorizedTokens);
	log("There is a total of ")
	log(VectorizedTokens.size())
	log("Commands to run")
	dumpEntireCommandVector(VectorizedTokens);

	return VectorizedTokens;

}
private:

	// Tokenize a string by the end operator ;
	// All vectors are size 2
	// except the last command, or if there is a single, which can be size one
	// string in the form of [command and flags and othe connectors][connector]
	static vector<vector<string> > TokenizeToLogicalEND(string completeStream){
		log("Tokenizing ends")
		vector<vector<string> > commandSet = splitToVector(completeStream, ";");

		return commandSet;

	}

	// Tokenize a string by the end operator ';'
	// All vectors are size 2
	// except the last command, or if there is a single, which can be size one
	// string in the form of [command and flags and othe connectors][connector]
	static vector<vector<string> > splitToVector(string completeStream, string delim){

		vector<string>  commands;
		vector<vector<string> > commandSet;
		log(completeStream)

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
			log("Start pos = " << start)
			log("Delim found at " << foundAt)
			if(foundAt < 0){
				end = completeStream.size();
			}
			log("End found at " << end)

			int sSize = end-start;
			log("Copying " << sSize << " characters")
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
			log("Pushing back command string")
			completeStream.copy(commandString, sSize, start);
			log(string(commandString))
			//push back the command stream between tokens
			commands.push_back(string(commandString));
			if(foundAt < 0){
				commandSet.push_back(commands);
				break;
			}

			char c = commandString[0];
			for(int i = 0; c != '\0'; i++){
				c = commandString[i];
				log(c)

			}
			// advance to the delim
			start = foundAt;

			log("Start pos = " << start)

			//}

			commandString = 0;
			delete commandString;

			commandString = new char[256];
			for(int i = 0; i < 256; i++){
				commandString[i] = '\0';
			}

			// Next grab the delim
			log("Second string: delim")
			log("Start pos = " << start)
			log(delim.size());
			end = start + delim.size();
			log("end pos = " << end)

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
			start = end;// + delim.size();
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
	static vector<vector<string> >  TokenizeVector(vector<vector<string> > *commandVector, string tok){
		string message = "Tokenizing vector by " + tok;
		log(message)

		vector<string> *spaceVector;
		vector<vector<string> > commandSet;
		char* buffer = new char[256];

		log("will tokenize " + IntToString(commandVector->size()) + " vectors")
		// loop trough our set of commands
		for(unsigned int i = 0; i <= (commandVector->size()-1); i++){
			log("Looping through vectors to tokenize")
			// loop through the connectorized vectors
			log(commandVector->at(i).at(0))

			spaceVector = &commandVector->at(i); //[stuff][connector]
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
			for(unsigned int j = 0; j < commands.size(); j++){
				vector<string> command = commands.at(j);//[stuff][connector]
				log("Got the reference")
				for(unsigned int z = 0; z < command.size(); z++){
					// push back each string
					log("Pushing back command")
					string s = command.at(z);
					log(s)
					flattencommand.push_back(command.at(z));
				}
				// add back on the deliminator
				log("add back delim if there was one")
				if(spaceVector->size() > 1)
					flattencommand.push_back(spaceVector->at(1));
				log("Dumping the flattened commands")
				for(unsigned int z = 0; z < flattencommand.size(); z++){
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

		*commandVector = commandSet;

		return *commandVector;

	}
	// Tokenize a string by the logical OR operator ||
	// All vectors are size 2
	// except the last command, or if there is a single, which can be size one
	// string in the form of [command and flags and other connectors][connector]
	static vector<vector<string> >  TokenizeToLogicalOR(vector<vector<string> > *commandVector){
		log("Tokenizing or")

		vector<string> spaceVector;
		vector<string> tmpVector;
		vector<vector<string> > commandSet;
		char* buffer = new char[256];
		// loop through the connectorized vectors
		for(unsigned int i = 0; i < commandVector->size(); i++){
			log("Looping through vectors to ORize")

			spaceVector = commandVector->at(i);
			log("Tokenizing a OR_Vector")
			// tokenize each string
			strcpy(buffer,spaceVector[0].c_str());
			char* prog = strtok(buffer, "||");
			// Put all the tokens into a vector
			do{
				if(prog == NULL){
					break;
				}else{
					log(prog)
				}
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
				// Only continue if there is another
			}while(prog != NULL);
			// clear the token vecotr for reuse
			tmpVector.clear();
		}
		buffer = 0;
		delete buffer;
		log("found ")
		log(commandSet.size())
		log("command sets")

		*commandVector = commandSet;
		return *commandVector;



	}
	// Tokenize a string by the logical AND operator &&
	// All vectors are size 2
	// except the last command, or if there is a single, which can be size one
	// string in the form of [command and flags and othe connectors][connector]
	static vector<vector<string> >  TokenizeToLogicalAND(vector<vector<string> > *parseVector){
		log("Tokenizing and")

		vector<string> spaceVector;
		vector<string> tmpVector;
		vector<vector<string> > commandSet;
		char* buffer;

		for(unsigned int i = 0; i < parseVector->size(); i++){
			log("Looping through vectors to ANDerize")
			// loop through the connectorized vectors
			spaceVector = parseVector->at(i);
			log(spaceVector[0])
			//for(int z = 0; z < spaceVector.size(); z++){
			log("Tokenizing a spaceVector")
			// tokenize each string
			buffer = new char[256];
			strcpy(buffer,spaceVector[0].c_str());
			// grab the token string
			char* prog = strtok(buffer, "&&");
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
			}while(prog != NULL);
			// clear the token vecotr for reuse
			tmpVector.clear();
			buffer = 0;
			delete buffer;
			}

		buffer = 0;
		delete buffer;

		log("found ")
		log(commandSet.size())
		log("command stream")


		*parseVector = commandSet;

		return commandSet;

	}
	// Tokenize a string by spaces
	// All input vectors are size 2
	// except the last command, or if there is a single, which can be size one
	// strings in the form of [command and flags and othe connectors][connector]
	// after this command finishes vectors are in the form
	// [command] [flag1] [flag2] .... [connector]
	static vector<vector<string> >  TokenizeToSpaces(vector<vector<string> > *completeStream){
		log("Tokenizing spaces")

		vector<string> spaceVector;
		vector<string> tmpVector;
		vector<vector<string> > commandSet;
		char* buffer = new char[256];

		for(unsigned int i = 0; i < completeStream->size(); i++){
			log("Looping through vectors to spacerize")
			// Grab the vector to tokenize by space
			spaceVector = completeStream->at(i);
			log(spaceVector[0]);
			log("Tokenizing a vector for spaces")
			// Grab the string to spacerize
			strcpy(buffer,spaceVector[0].c_str());
			char* prog = strtok(buffer, " ");
			// Put all the tokens into a vector
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

		*completeStream = commandSet;
		return *completeStream;

	}


};






#endif /* TOKENIZER_H_ */
