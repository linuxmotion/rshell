/*
 * DirUtils.h
 *
 *  Created on: Feb 4, 2015
 *      Author: john
 */

#ifndef DIRUTILS_H_
#define DIRUTILS_H_

#include <string>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <error.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using std::string;

#include <errno.h>




class DirUtils{
public:

static string  get_cwd(){

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
static string  tildeExpansion(){
	return string(getenv("HOME")) + "/";
}


struct DirectoryEntry{

	string mName;
	string mUserName;
	string mGroupName;
	string mDirType;
	string mUsrPerm;
	string mGrpPerm;
	string mOtherPerm;
	int mNumLink;
	__time_t mLastAccesTime;
	int mFileSize;

};

// Extracts the nessecarry information need to display the ls entry
// return with a blank entry on failure, or a struct with all information
static DirectoryEntry  extractDisplayData(struct dirent* entry, string root){

			DirectoryEntry Entry;
			//printf("Displaying entry");
			if((string(entry->d_name) == ".")|| (string(entry->d_name) == "..")){
				//	return;
			}
			struct stat stats;
			string path = root  + "/"+ entry->d_name;
			if(lstat(path.c_str(), &stats) == -1){
				// lstat failed, call perror
				perror("lstat " + errno);
				return Entry;

			}

			struct passwd *user = getpwuid(stats.st_uid);
			struct group * grp  = getgrgid(stats.st_gid);
			Entry.mDirType = ((entry->d_type == DT_DIR) ? 'd' : ((entry->d_type == DT_LNK) ? 'l' : '-'));
			Entry.mUsrPerm = ((stats.st_mode & S_IRUSR)?'r':'-');
			Entry.mUsrPerm += ((stats.st_mode & S_IWUSR)?'w':'-');
			Entry.mUsrPerm += ((stats.st_mode & S_IXUSR)?'x':'-');
			Entry.mGrpPerm = ((stats.st_mode & S_IRGRP)?'r':'-') ;
			Entry.mGrpPerm +=((stats.st_mode & S_IWGRP)?'w':'-');
			Entry.mGrpPerm += ((stats.st_mode & S_IXGRP)?'x':'-');
			Entry.mOtherPerm = ((stats.st_mode & S_IROTH)?'r':'-');
			Entry.mOtherPerm += ((stats.st_mode & S_IWOTH)?'w':'-');
			Entry.mOtherPerm += ((stats.st_mode & S_IXOTH)?'x':'-');
			Entry.mNumLink =  int(stats.st_nlink);
			Entry.mUserName = user->pw_name;
			Entry.mGroupName = grp->gr_name;
			Entry.mFileSize = int(stats.st_size);
			Entry.mLastAccesTime = stats.st_atim.tv_sec;
			Entry.mName =  entry->d_name;


			return Entry;
		}

};


#endif /* DIRUTILS_H_ */
