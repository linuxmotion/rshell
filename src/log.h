/*
 * log.h
 *
 *  Created on: Jan 15, 2015
 *      Author: john
 */

#ifndef LOG_H_
#define LOG_H_
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
// Comment this out to disable debuging
//#define DEBUG

using std::string;
using std::vector;
using std::cout;
using std::endl;
///////////////////////////
bool isConnector(string str);
string IntToString(int num);
void dumpCommands(char** commands, int size);
void dumpCommandVector(vector<string> commandSet);
void dumpEntireCommandVector(vector<vector<string> > commandSet);


string IntToString(int num);

bool isConnector(string str);



// NEVER leave this in a IF statement by itself with no braces!!!!!
// it may wreck the compile as all occurence get replace by a space
// meaning the if will pull the next statment as the clause
#ifdef DEBUG
#define log(String) \
		cout << __FILE__ << "::"<< __FUNCTION__ << "::"<< __LINE__ << " " << String << endl;



#else
#define log(String)
#endif



#endif /* LOG_H_ */
