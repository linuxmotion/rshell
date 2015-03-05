
#include "log.h"

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
// Comment this out to disable debuging

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;

void dumpCommands(char** commands, int size) {
	log("Duimping commands to execute")
	for(int i = 0; i < size; i++ ){

		std::string str = "command:";
		//log(str)
		if(commands[i] != NULL){
			log(commands[i]);
		}else{
			log("NULL")
		}
	}


}


void dumpCommandVector(vector<string>  commandSet) {

#ifdef DEBUG
	vector<vector<string> > tmp;
	tmp.push_back(commandSet);
	dumpEntireCommandVector(tmp);
#endif
}

void dumpEntireCommandVector(vector<vector<string> > commandSet) {

#ifdef DEBUG
	std::cout << std::endl << "Start dump"<< std::endl ;
	log("Entire commandset: " + IntToString(commandSet.size()))
	for(unsigned int i = 0; i < commandSet.size(); i++){
		string screen = "commandset :";
		log(screen)
		vector<string> commandString = commandSet[i];
		string command = IntToString(commandString.size()) + ": ";
		for(unsigned int z = 0; z < commandString.size(); z++){
			command += commandString[z] + " ";
		}
		log(command)



	}
	std::cout << "End dump" << std::endl << std::endl;
#endif
}


string IntToString(int num){

	std::stringstream ss;
	ss << num;
	return ss.str();
}


bool isConnector(string str){
	if((str.compare("&&")== 0)
			|| (str.compare("||")== 0)
			|| (str.compare(";")== 0)
			|| (str.compare(">") == 0)
			|| (str.compare(">>") == 0)
			|| (str.compare("<") == 0)
			|| (str.compare("|") == 0)){
		log("A connector was found")
		return true;
	}
	return false;
}
