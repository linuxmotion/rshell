/*
 * Lslib.h
 *
 *  Created on: Feb 2, 2015
 *      Author: john
 */

#ifndef LSLIB_H_
#define LSLIB_H_

#include <string>
#include <vector>
#include <dirent.h>
#include "DirUtils.h"


using std::string;
using std::vector;

#define VERSION 1
#define NAME    "Mako"


class Lslib {

public:

	Lslib();
	// Calls ls on the current directory
	void call_ls(int argc, char* argv[]);
	virtual ~Lslib();

	const static char RECURSIVE = 'R';
	const static char LONG = 'l';
	const static char ALL = 'a';


private:

	struct   cmpByName{
			    bool operator()(const DirUtils::DirectoryEntry a, const DirUtils::DirectoryEntry b)  const {
			        return a.mName < b.mName;
			    }
			};


	string mCurrentDirectory;


	bool LONG_FORMAT;
	bool RECURSIVE_LIST;
	bool ALL_RECORDS;

	void setFlag(char c);
	void set_arguments(vector<string> args);
	// call ls on the given directory
	bool call_ls(string path);
	// Display a user entry
	void displayEntry(DirUtils::DirectoryEntry entry);
	void displayLongEntry(DirUtils::DirectoryEntry entry);
	// Open a directory
	DIR * openDir(string path);
	// close the directory
	bool closeDir(DIR* dirp);
	// read a directory
	bool readDir(DIR* dirp);





};

#endif /* LSLIB_H_ */
