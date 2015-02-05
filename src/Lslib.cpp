/*
 * Lslib.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: john
 */

#include "Lslib.h"

Lslib::Lslib() {
	// TODO Auto-generated constructor stub
}

// Calls ls on the current directory
void  Lslib::call_ls(int argc, char* argv[]){
	printf("Calling ls\n");
	// parse the arguments
	vector<string> args;
	for(int i =1 ; i < argc; i++){
		args.push_back(argv[i]);
	}
	set_arguments(args);

	call_ls(get_cwd());

}


void  Lslib::setFlag(char c){
	printf("TODO: Set the status flasgs\n");

}

void  Lslib::set_arguments(vector<string> args){
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

string  Lslib::get_cwd(){

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
string  Lslib::tildeExpansion(){
	return string(getenv("HOME")) + "/";
}

// call ls on the given directory
// can be realative or absolute
bool  Lslib::call_ls(string path){
	std::list<string> SavedDirs;

		if(path == ""){

			path = ".";
		}
		//printf("Path %s \n", path.c_str());
		// hmm for some reason our cwd
		// could not be found


		//printf("About to open dir \n");

		DIR * dirp = openDir(path);

		if(dirp == NULL)
			return false;
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
		// need to free dirp
		// it should be open at this time

		}
		closeDir(dirp);
		return true;

}


		void  Lslib::displayEntry(struct dirent* entry, string root){

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

