/*
 * log.h
 *
 *  Created on: Jan 15, 2015
 *      Author: john
 */

#ifndef LOG_H_
#define LOG_H_
#include <string>
#include <vector>
#include <iostream>
// Comment this out to disable debuging
#define DEBUG

#ifdef DEBUG

using std::cout;
using std::endl;
using std::cerr;
// NEVER leave this in a IF statement by itself with no braces!!!!!
// it may wreck the compile as all occurence get replace by a space
// meaning the if will pull the next statment as the clause
#define log(String) \
		cout << __FILE__ << "::"<< __FUNCTION__ << "::"<< __LINE__ << " " << String << endl;



#else
#define log(String)
#endif

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

#endif /* LOG_H_ */
