/*
 * Lslib.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: john
 */

#include "Lslib.h"
#include "DirUtils.h"
#include <stdio.h>
#include <vector>
#include <string>

using std::string;
using std::vector;

Lslib::Lslib():LONG_FORMAT(false),RECURSIVE_LIST(false),
		ALL_RECORDS(false) {
	}

// Calls ls on the current directory
void  Lslib::call_ls(int argc, char* argv[]){
	//printf("Calling ls\n");
	// parse the arguments
	vector<string> args;
	for(int i =1 ; i < argc; i++){
		args.push_back(argv[i]);
	}
	set_arguments(args);

	call_ls(DirUtils::get_cwd());

}


void  Lslib::setFlag(char c){

	switch(c){
		case LONG:{
			this->LONG_FORMAT = true;
		}
		break;
		case ALL:{
			this->ALL_RECORDS= true;
		}
		break;
		case RECURSIVE:{
			this->RECURSIVE_LIST = true;
		}
		break;
		default:{
			// Do nothing not a flag
		}
		break;

	}


}

void  Lslib::set_arguments(vector<string> args){
	//printf("Parsing the command line arguments\n");

	for(unsigned int i = 0; i < args.size(); i++){

		std::size_t pos =  args[i].find('-');
		if(pos > 0){
			// - was not the first character
			printf("Syntax Error: Unrecognized flag: %s ignoring \n", args[i].c_str());
		}
		else{

			//printf("Found a switch %s \n", args[i].c_str());
			// strip the -

			// iterate through the string to grab the
			// switches, each char is a diff switch
			for(unsigned int z = 1; z < args[i].size(); z++){

				char c = args[i][z];
				// set the flag for this character
				setFlag(c);

			}


		}

	}

}

#include <algorithm>

bool startsWithDot(string name){


	if(name[0] == '.')
		return true;

	return false;

}

// call ls on the given directory
// can be realative or absolute
#include <iostream>
bool  Lslib::call_ls(string path){
	vector<DirUtils::DirectoryEntry> SavedDirs;

		if(path == ""){

			return false;
		}
		//printf("Path %s \n", path.c_str());
		// hmm for some reason our cwd
		// could not be found


		//printf("About to open dir \n");

		DIR * dirp = openDir(path);

		if(dirp == NULL)
			return false;
		//printf("Directory opened succesfully\n");
		struct dirent *dent;

		//printf("about to read dir\n");
		int h = 0;
		int size = 0;
		while((dent = readdir(dirp))){

			SavedDirs.push_back(DirUtils::extractDisplayData(dent, path));
			size += SavedDirs[h++].mFileSize;

		}



		std::sort( SavedDirs.begin(), SavedDirs.end(), cmpByName() );
		//printf("%d \n", size/1024);
		closeDir(dirp);
		// displayt the entry
		for(unsigned int i = 0; i < SavedDirs.size(); i++){
			bool isdot = (SavedDirs[i].mName[0] == '.') ? true : false ;
			//std::cout << isdot << std::endl;
			if(isdot){
				if(this->ALL_RECORDS){
					if(this->LONG_FORMAT){
						displayLongEntry(SavedDirs[i]);
					}else {
						displayEntry(SavedDirs[i]);

					}
				}

			}else{
				if(this->LONG_FORMAT){
					displayLongEntry(SavedDirs[i]);
				}else {
					displayEntry(SavedDirs[i]);

				}
			}

		}

		printf("\n");
		// if tyhe -r flag is set recurse
		if(this->RECURSIVE_LIST){
			for(unsigned int i = 0; i < SavedDirs.size(); i++){

				if((SavedDirs[i].mName == ".") || (SavedDirs[i].mName == ".."))
					continue;

				if(SavedDirs[i].mDirType == "d"){
					printf("\n\n");
					call_ls(path + "/" + SavedDirs[i].mName);
				}
			}

		}

		return true;

}

#include <time.h>

string getMonthName(int month){
	string months[] = {"Jan", "Feb", "Mar", "Apr", "May",
	                       "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	if(month < 0 || month > 11)
		return "";

	return months[month];


}
		void  Lslib::displayLongEntry(DirUtils::DirectoryEntry entry){

			printf("%s", entry.mDirType.c_str());
			printf("%3s%3s%s", entry.mUsrPerm.c_str(), entry.mGrpPerm.c_str() , entry.mOtherPerm.c_str() );
			printf("%2d ", entry.mNumLink);

			//intf(" %2d", int(stats.st_nlink));
			//printf(" %2d", int(stats.st_nlink));
			printf("%s %s " , entry.mUserName.c_str(), entry.mGroupName.c_str());//<< ;
			printf("%6d ", entry.mFileSize);

			tm *time = localtime(&entry.mLastAccesTime);

			// Unix time starts in 1900
			//printf("%s:%s", time->tm_hour, time->tm_min);
			printf(" %2s %d ", getMonthName(time->tm_mon).c_str(), time->tm_mday);
			printf("%d:%.2d ",time->tm_hour, time->tm_min);
			printf( "%-10s\n", entry.mName.c_str());



		}


		void  Lslib::displayEntry(DirUtils::DirectoryEntry entry){

			printf("%s ", entry.mName.c_str());


		}

	DIR * Lslib::openDir(string path){

		//TODO: Use error checking, dummy for now



		return opendir(path.c_str());

	}




	bool Lslib::closeDir(DIR * dirp){

		//TODO: Use error checking, dummy for now


		return closedir(dirp) == 0;

	}


Lslib::~Lslib() {
	// TODO Auto-generated destructor stub
}

