/*
 * Lslib.h
 *
 *  Created on: Feb 2, 2015
 *      Author: john
 */

#ifndef LSLIB_H_
#define LSLIB_H_

#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <list>
#include <sys/stat.h>
#include <sys/types.h>

using std::string;
using std::vector;

#define VERSION 1
#define NAME    "Mako"

#define printusrperm(stats) \
	printf("%c%c%c",  ((stats.st_mode & S_IRUSR)?'r':'-'),\
					  ((stats.st_mode & S_IWUSR)?'w':'-'),\
					  ((stats.st_mode & S_IXUSR)?'x':'-'));


#define printgrpperm(stats) \
		printf("%c%c%c",  ((stats.st_mode & S_IRGRP)?'r':'-'),\
						  ((stats.st_mode & S_IWGRP)?'w':'-'),\
						  ((stats.st_mode & S_IXGRP)?'x':'-'));

#define printothperm(stats) \
		printf("%c%c%c",  ((stats.st_mode & S_IROTH)?'r':'-'),\
						  ((stats.st_mode & S_IWOTH)?'w':'-'),\
						  ((stats.st_mode & S_IXOTH)?'x':'-'));

class Lslib {

public:

	Lslib();
	// Calls ls on the current directory
	void call_ls(int argc, char* argv[]);
	virtual ~Lslib();

private:

	string mCurrentDirectory;
	const string HOME_DIR = "~/";
	bool LONG_FORMAT = false;
	bool RECURSIVE_LIST = false;
	bool ALL_RECORDS = false;

	void setFlag(char c);
	void set_arguments(vector<string> args);
	string get_cwd();
	string tildeExpansion();
	// call ls on the given directory
	bool call_ls(string path);
	// Display a user entry
	void displayEntry(struct dirent* entry, string root);
	// Open a directory
	DIR * openDir(string path);
	// close the directory
	bool closeDir(DIR* dirp);
	// read a directory
	bool readDir(DIR* dirp);





};

#endif /* LSLIB_H_ */
