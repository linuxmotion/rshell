/*
 * SimpleGLibPipe.h
 *
 *  Created on: Feb 1, 2013
 *      Author: john
 */

#ifndef SIMPLEGLIBPIPE_H_
#define SIMPLEGLIBPIPE_H_

#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <stdio.h>


class SimpleGlibPipe{

private:
	int mPipes[2];
	int mSavedPipes[2];
	int mOpenPipes[2];

public:
	SimpleGlibPipe() throw(int){
		if(openPipe() == -1){
			throw -1;
		}
	}

	bool openPipe() {

		// Open a pipe
		int status = pipe(mPipes);
		//
		if(status == -1){
			perror("pipe failed: ");
			return false;
		}
		// Set a status flag so that
		// we dont double close
		mOpenPipes[0] = 1;
		mOpenPipes[1] = 1;
		return true;

	}



	/**
	 * Return the handle to the write pipe.
	 * Returns -1 on error.
	 */
	int getWritePipe(){

		if(mOpenPipes[1] != 1){
			fprintf(stderr, "Failed to get write pipe");
			return -1;
		}

		return mPipes[1];
	}
	/**
	 * Return the handle to the read pipe.
	 * Returns -1 on error.
	 */
	int getReadPipe(){
		if(mOpenPipes[0] != 1){
			fprintf(stderr,"Failed to get read pipe");
			return -1;
		}
		return mPipes[0];
	}

	/**
	 * Set the handle to the write pipe.
	 * Returns false on error, true on success
	 */
	bool setwritePipe(int fd){
		if(mOpenPipes[1] < 0){
			fprintf(stderr, "Unable to get write pipe");
			return false;
		}

		dup2(getWritePipe(), fd);


		return true;

	}

	/**
	 * Set the handle to the read pipe.
	 * Returns false on error, true on success
	 */
	bool setReadPipe(int fd){
		if(mOpenPipes[0] < 0){
			fprintf(stderr, "Unable to get read pipe");
			return false;
		}
		dup2(getReadPipe(), fd);


		return true;
	}

	void resetPipes(){

		dup2(mSavedPipes[0], STDIN_FILENO);
		dup2(mSavedPipes[1], STDOUT_FILENO);
	}

	/**
	 * Reopens the pipes to use.
	 * It will first close both pipes if necessary.
	 * Return true on success, false otherwise
	 */
	bool reopenPipe(){
		closeWritePipe();
		closeReadPipe();
		return openPipe();

	}

	void closePipe(){
			closeWritePipe();
			closeReadPipe();
		}


	void closeWritePipe(){

		if(mOpenPipes[1]){
			close(mPipes[1]);
			mOpenPipes[1] = 0;
		}
	}

	void closeReadPipe(){
		if(mOpenPipes[0]){
			close(mPipes[0]);
			mOpenPipes[0] = 0;
		}
	}

	~SimpleGlibPipe(){
		closeWritePipe();
		closeReadPipe();

	}


};





#endif /* SIMPLEGLIBPIPE_H_ */
