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
		std::list<string> SavedDirs;

			if(path == ""){

				return;
			}
			//printf("Path %s \n", path.c_str());
			// hmm for some reason our cwd
			// could not be found


			//printf("About to open dir \n");

			DIR * dirp = openDir(path);

			if(dirp == NULL)
				return;
			//rintf("Directory opened succesfully\n");
			struct dirent *dent;

			//printf("about to read dir\n");
			while((dent = readdir(dirp))){

				//printf("About to Display entry\n");
				displayEntry(dent, path); // display the entry


				if(dent->d_type == DT_DIR){
					if((string(dent->d_name) == ".") || (string(dent->d_name) == "..")){
						continue;
					}
					SavedDirs.push_back(dent->d_name);



			}

		}


	}


			void displayEntry(struct dirent* entry, string root){

				//printf("Displaying entry");
				if((string(entry->d_name) == ".")|| (string(entry->d_name) == "..")){
					//	return;
				}

				struct stat stats;
				string tmp = root  + "/"+ entry->d_name;
				lstat(tmp.c_str(), &stats);
				struct passwd *user = getpwuid(stats.st_uid);
				struct group * grp  = getgrgid(stats.st_gid);
				printf("%c", ((entry->d_type == DT_DIR) ? 'd' : ((entry->d_type == DT_LNK) ? 'l' : '-')));
				// Defined by Macro's above to save function athstetics
				printusrperm(stats);
				printgrpperm(stats);
				printothperm(stats);
				printf(" %2d", int(stats.st_nlink));
				printf(" %s %s " , user->pw_name, grp->gr_name);//<< ;
				printf(" %6d", int(stats.st_size));
				tm *time = localtime(&stats.st_atim.tv_sec);
				// Unix time starts in 1900
				printf(" %d-%.2d-%.2d", time->tm_year+1900, time->tm_mon + 1, time->tm_mday);
				printf(" %d:%.2d",time->tm_hour, time->tm_min);
				printf(" %-10s\n", entry->d_name);


			}

		DIR * openDir(string path){

			//TODO: Use error checking, dummy for now

			return opendir(path.c_str());

		}







};

#endif /* LSLIB_H_ */
