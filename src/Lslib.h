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

using std::string;
using std::vector;

#define VERSION 1
#define NAME    "Mako"

class Lslib {

public:

	Lslib();
	// Calls ls on the current directory
	void call_ls(int argc, char* argv[]){
		printf("Calling ls\n");
		// parse the arguments
		vector<string> args;
		for(int i =1 ; i < argc; i++){
			args.push_back(argv[i]);
		}
		set_arguments(args);

		call_ls(get_cwd());

	}
	virtual ~Lslib();

private:

	string mCurrentDirectory;
	const string HOME_DIR = "~/";

	void setFlag(char c){
		printf("TODO: Set the status flasgs\n");

	}

	void set_arguments(vector<string> args){
		printf("Parsing the command line arguments\n");

		for(int i = 0; i < args.size(); i++){

			std::size_t pos =  args[i].find('-');
			if(pos > 1){
				// - was not the first character
				printf("Found a path %s \n", args[i].c_str());
			}
			else{
				printf("Found a switch %s \n", args[i].c_str());
				// strip the -

				// iterate through the string to grab the
				// switches, each char is a diff switch
				for(int z = 1; z < args[i].size(); z++){

					char c = args[i][z];
					// set the flag for this character
					setFlag(c);

				}


			}

		}

	}

	string get_cwd(){

		// This memory needs to be freed
		char * dirName = get_current_dir_name();
		if(dirName == NULL){

			switch(errno){
				case EACCES:{
					printf("Insufficient permissions\n");
					break;
				}
				case EFAULT:{
					printf("Allocation error\n");
					break;
				}
				case EINVAL:{
					printf("A space has been allocated but there is no name\n");
					break;
				}

				case ENOENT:{
					printf("The current working directory has been unlinked\n");
					break;
				}
				case ERANGE:{
					printf("Array size to small, allocate a larger array\n");
					break;
				}
			}

			// in any case just return a blank dir name
			return "";
		}
		// grab the string, let the string class
		// manage the memory now
		string sdirName = dirName;
		// free that initial memory
		// so we dont lose it
		free(dirName);
		return sdirName;
	}
	string tildeExpansion(){
		return string(getenv("HOME")) + "/";
	}





	bool LONG_FORMAT = false;
	bool RECURSIVE_LIST = false;
	bool ALL_RECORDS = false;
		// call ls on the given directory
		// can be realative or absolute
		void call_ls(string path){

			printf("calling %s \n", path.c_str());
			// hmm for some reason our cwd
			// could not be found
			if(path == ""){

				return;
			}


		}

};

#endif /* LSLIB_H_ */
