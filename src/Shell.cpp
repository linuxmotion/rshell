/*
 * RShell.cpp
 *
 *  Created on: Jan 6, 2015
 *      Author: john
 */

#include "Shell.h"

#include <iostream>
#include <vector>
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <sstream>
#include <fcntl.h>

#include <ctype.h>
#include <signal.h>
#include <algorithm>

#include "DirUtils.h"
#include "log.h"
#include "Tokenizer.h"
#include "Cd.h"

using std::string;
using std::vector;

// move these to a header



Shell::Shell() {
	// TODO Auto-generated constructor stub
	setupInternalCommandList();
	VERSION = "0.9-lynx";

}

bool Shell::NeedToExit(vector<string> commands) {
	if(commands[0] == "exit")
		return true;
	else
		return false;
}

void Shell::orConnector(bool& doExecution, bool& success, int execi, int size,
		std::vector<std::vector<std::string> > execCommandSet,
		bool& resetExecution) {
	log("Found a || connector")
	doExecution = !success;
	string yes = "Will execute beacuse prevois command failed";
	string no = "Not executing next command because prevois command succeeded";
	string logs = ((doExecution == true) ? yes : no);
	log(logs)
	// if it turn out that we do not execute the next command
	// we dont need to execute until a ; is found
	if (!doExecution) {
		log("Finding next vector to execute")
		for (int findNextEnd = execi; findNextEnd < size; findNextEnd++) {
			log("Checking vector " + IntToString(execi))
			vector < string > tmp = execCommandSet[execi]; //
			string connector = tmp[tmp.size() - 1];
			log(connector)
			// once we find the connector, advance execi to the vector
			// following that connector
			if (connector.compare(";") == 0) {
				execi = (findNextEnd);
				log("Found next ; at " + IntToString(execi))
				// since the executin flag is false, break out
				// of the loop and let the outer loop complete
				// execution is phrohibited
				resetExecution = true;
				// break out from the loop
				break;
			}

		}

	}
}

void sigInt(int sigint){



	//printf("\n\nCaught SIGINT\n\n");

}
void Shell::StartShell(int argc, char **argv){

	log("Starting shell")

	signal(SIGINT, sigInt);
	Run();



}

void Shell::Run(){

	std::vector<std::vector<std::string> > execCommandSet;
	bool terminate = false;
	do{


		// Print a command line every time
		PrintCommandPrompt();
		// Read in the commands from the printed prompt
		std::string commands = ReadInCommands();
		log("About to parse commands")
		// Parse the strings into by ||,&&,;
		// Clear any commands left in buffer
		execCommandSet.clear();
		execCommandSet = ParseCommands(commands);
		// set initial success status to true
		// If there was one or more commands to execute, execute them
		if(execCommandSet.size() > 0){
			terminate = ExecuteCommands(execCommandSet);
		}

	}while(terminate == false);
}
void Shell::PrintCommandPrompt(){
	log("Printing command line")
	// getcwd(mCurrentDir, sizeof(mCurrentDir));
	// clear all the tokens from the last input
	//	Tokens.clear();
	// Print the command line
	char * buff = new char[256];
	char *username = new char[256];
	string name = "unknown";
	string host = "unknown";
	cuserid(username);
	if(username != NULL){
		name = username;
	}
	if(!gethostname(buff, 256)){
		host = buff;
	}

	printf("%s@%s:%s$ ", name.c_str(), host.c_str(),  DirUtils::get_cwd().c_str() );
	//Get the line that was entered

}



std::string Shell::ReadInCommands(){
	log("Reading in commands")
	int buffSize = 256;
	char *mBuffer = new char[buffSize];

	//std::cin.ignore(10, '\n');
	//std::cin.ignore('\n');
	//std::cin.
	std::cin.clear();
	std::cin.getline(mBuffer, buffSize);
	//std::cin.clear();
	return string(mBuffer);
}

vector<vector<string> > Shell::ParseCommands(string commandStream){
	log("parsing commands")

	vector<vector<string> > commandSet;
	// Check if there are commands to parse
	if(commandStream.empty())
		return commandSet;

	// resize the array to remove anything after the #
	size_t comment = commandStream.find_first_of('#');
	if(comment != string::npos){
		if(comment <= 0){
			commandStream.resize(0);
		}else{

			commandStream.resize(comment-1);
		}


	}
	// Check to see if a comment was the only input
	if(commandStream.empty())
		return commandSet;
	// log the whole stream to parse. Kick off the parser
	log(commandStream)
	commandSet = Tokenizer::TokenizeCommandStream(commandStream);

	return commandSet;
}




// Accepts a command vector
// commands are in the form
// [command] [flag] --- [flag] [connector]
void Shell::ExecuteChildProcess(vector<string> commandVector) {

	vector<string> command = commandVector;
	log("Executing child process " + commandVector[0])
	int size = command.size();
	log(size)
	log("commands Found")
	// Create an array 1 great than size of commandVector
	char** argv = new char*[size+1];
	for(int i = 0; i < size; i++){
			// create new empty array
			argv[i] = new char;
	}

	// Null the last command
	argv[size] = NULL;

	log("Deep copy parameters")
	for(int i = 0; i < size; i++ ){
		log("Copying command " + IntToString(i))

		string scommand = command[i];
		log(scommand)
		if(isConnector(scommand)){
			argv[i] = NULL;
			log("NULL")
		}else{
			log("about to copy string")
			char *str = new char;
			strcpy(str,scommand.c_str());
			//log(str);
			argv[i] = str;
			log(argv[i]);
			//strcpy(argvi)
		}
	}

	log("About to dump commands")
    // Last command is always NULL
	dumpCommands(argv, size-1);


	log("Executing: ")
	log(argv[0]);
	log("PATH #'s:" )

	vector<string> paths = getEnvVar("PATH", ":");

	char *oprogram = new char;
	strcpy(oprogram, argv[0]);
	for(unsigned int i = 0; i < paths.size(); i++){

		string program = paths[i]+ "/" + oprogram;
		strcpy(argv[0],program.c_str());
		log("executing:" + program)
		execv(program.c_str(), argv);
	}
	string err = "All exec failed. Program ";
			err += oprogram;
			err += " does not exists";
	perror(err.c_str());

	exit(EXIT_FAILURE);
}
vector<string> Shell::getEnvVar(string var, string delim){


	char *evar = getenv(var.c_str());
	vector<string> paths;
	// if there was no delim, we cant break it
	if(delim.empty()){
		paths.push_back(evar);
		return paths;
	}

	// There will on be one set of vectors
	char* envt = new char[256];
	strcpy(envt, evar);

	char * envpath = strtok(envt, delim.c_str());
		// Put all the tokens into a vector
		do{
			if(envpath == NULL){
				break;
			}else{
				log(envpath)
			}
			// push back the program token
			paths.push_back(envpath);

			envpath = strtok(NULL, delim.c_str());
			// Only continue if there is another
		}while(envpath != NULL);
		// clear the token vector for reuse

	delete envt;

	return paths;

}


bool Shell::ExecuteParentProcess(pid_t pid,bool wait) {
	log("Parent execution after fork started");
		/**
		 * if somewhere there is a &
		 * ie. command -flags &
		 * the the parent should not wait on the child
		 *
		 */
		//wait = true;
		if(wait){
			log("Waiting on child process");
			int status;
			// wait for child to exit
			if (waitpid(pid, &status, 0) < 0) {
				log("An error occured while waiting for the child process");
				perror("An error occured while waiting for the child process");
				return false;
			}
			if (WIFEXITED(status)) {
				log("Child exit status = " << WEXITSTATUS(status))
			}
			else{
				// The child did not exit properly
				perror("The child did not exit properly");
				return false;

			}
		}
		else{
			log("Not waiting on child execution")

		}
		log("Parent execution after fork finished successfully");

		return true;
}

bool Shell::Execute(vector<string> commandVect){
	return Execute(commandVect, true);
}

bool Shell::Execute(vector<string> commandVect, bool wait){
	log("Forking process")

	// first we must fork
	pid_t PID = fork();
	if(PID == 0){
		// Child process
		log("In child process");
		ExecuteChildProcess(commandVect);
		log("this should not print");
		return true;
	}
	else if(PID > 0){
		// Parent process
		return ExecuteParentProcess(PID, wait);
	}
	else{
		perror("Something bad happened with fork");
		return false;

	}
	// After we fork we must execute the command
	// in the child process
	// the parent should wait for the child to finish
	// unless a & is encountered

	return false;
}

int Shell::isInternalCommand(const vector<string> *command){

	log("isInternalCommand: finding " + command->at(0))
	//loop though all possible internal commands
	for(unsigned int i = 0; i < this->mInternalCommands.size(); i++){
		// do we have a match
		if(command->at(0).compare(mInternalCommands.at(i)) == 0){
			// we do have a match
			log("Match Found at " + IntToString(i))
			return i;
		}
	}

	return -1;
}
bool Shell::ExecuteInternalCommand(int pos, const vector<string> *command){

	log("ExecuteInternalCommand")
	const int CD = 0;
	const int VERSION = 1;

	switch(pos){

		case CD:{
			log("Calling CD")
			return Cd::callCD(command);
		}
		case VERSION:{
			log("Printing version")
			printf("%s\n",this->VERSION.c_str());
			return true;
		}

	}

	return false;
}

void Shell::setupInternalCommandList(){
	mInternalCommands.push_back("cd");
	mInternalCommands.push_back("--version");
}
bool Shell::ExecuteCommands(vector<vector<string> > execCommandSet){


	bool resetExecution = false;
	bool success = true;
	// set initial execution status to true
	bool doExecution = true;
	int commandSetsize = execCommandSet.size();

	log("Looping through " + IntToString(commandSetsize) + " command sets" )

#ifdef DEBUG
		dumpEntireCommandVector(execCommandSet);
#endif

	// loop through the commands to execute
	for(int execi = 0; execi < commandSetsize; execi++){

		log("Checking against internal commands")
		int pos = isInternalCommand(&execCommandSet[execi]);
		if(pos != -1){
			success = ExecuteInternalCommand(pos, &execCommandSet[execi]);
			continue;
		}

		log("Finding exit status")
		if(NeedToExit(execCommandSet[execi])){
			log("Going to exit the program")
			return true; // This is okay because there is no cleanup needed
		}else{

			// Execute the command
			log("Beginning execution phase for command: " + IntToString(execi))
			// grab the commands set to execute
			vector<string> command = execCommandSet[execi];

			bool execFromRedir = HandleConnectors(commandSetsize,
												  execi,
												  execCommandSet,
												  doExecution,
												  resetExecution,
												  success);



			log("Index:" + IntToString(execi))
			if(!execFromRedir && doExecution){
				log("Will execute a command")
				success = Execute(command);
			}
			// we should mark that the next command is allowed to execute
			if(resetExecution){
				log("Resetting execution status")
				doExecution = true;
				// set this status falsg to false
				// so the executin is controlled by
				// connectors again
				resetExecution = false;
			}
			log("The execution finished: ")
			string tmp = (success == true ? "true" : "false");
			log(tmp)
		}


	}
	return false;


}
bool Shell::rightRedirectionAppend(vector<string>& leftHandSide,
						    vector<string>& rightHandSide){
	//First, we're going to open a file
		log("Redirecting to " << rightHandSide[0])
		char * st = new char[256];

		strcpy(st,DirUtils::get_cwd().c_str());
		strcat(st, "/");
		strcat(st, rightHandSide[0].c_str());
		int file = open(st, O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
		log("Opened redirection pipe to " << st)
		if(file < 0)
			return false;

		log("Setting the output")
		int save = dup(STDOUT_FILENO);
		//Now we redirect standard output to the file using dup2
		dup2(file,STDOUT_FILENO);

		//close(file,STDOUT_FILENO)


		//Now standard out has been redirected, we can write to
		// the file
		Execute(leftHandSide);
		dup2(save, STDOUT_FILENO);
		close(save);

		return true;
}

bool Shell::rightRedirection(vector<string>& leftHandSide,
						    vector<string>& rightHandSide){
	//First, we're going to open a file
		log("Redirecting to " << rightHandSide[0])
		char * st = new char[256];

		strcpy(st,DirUtils::get_cwd().c_str());
		strcat(st, "/");
		strcat(st, rightHandSide[0].c_str());
		int file = open(st, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
		log("Opened redirection pipe to " << st)
		if(file < 0)
			return false;

		log("Setting the output")
		int save = dup(STDOUT_FILENO);
		//Now we redirect standard output to the file using dup2
		dup2(file,STDOUT_FILENO);

		//close(file,STDOUT_FILENO)

		log("Redirecting " << rightHandSide[0].c_str() )
		//Now standard out has been redirected, we can write to
		// the file
		Execute(leftHandSide);
		dup2(save, STDOUT_FILENO);
		close(save);
		return true;
}

void Shell::handleLeftRedirect(vector<string>& LeftHandSide,
							   vector<string>& RightHandSide
							   ) {

	const int BUFFER_SIZE = 256;
	log("Redirecting from " << RightHandSide[0] << " into " << LeftHandSide[0])
	char* st = new char[BUFFER_SIZE];
	strcpy(st, DirUtils::get_cwd().c_str());
	strcat(st, "/");
	strcat(st, RightHandSide[0].c_str());
	log(st)
	int file = open(st, O_RDONLY);
	log("Opened redirection pipe to " << st)
	if (file < 0) {
		perror("I/O Error: Failed to get a valid file descriptor");
		return;
	}
	st = 0;
	delete st;
	log("Connecting the input stream to " << LeftHandSide[0])
	//Now we redirect standard output to the file using dup2
	int save = dup(STDIN_FILENO);
	if(save == -1){
		perror("dup failed: ");
		return;
	}
	if(dup2(file, STDIN_FILENO) == -1){
		perror("dup2 failed: ");
		return;
	}
	// If we have reached this point it mean all of the
	// file descriptors were valid and are ready
	Execute(LeftHandSide);
	log("Closing inputstream fd")
	if(close(file) == -1){
		perror("close failed: ");
	}

	if(dup2(save, STDIN_FILENO) == -1){
		perror("dup2 failed: ");
	}
	if(close(save) == -1){
		perror("close failed: ");
	}

}

bool Shell::HandleConnectors(int commandSetsize,
					  int &execi,
					  vector<vector<string> > execCommandSet,
					  bool &doExecution,
					  bool &resetExecution,
					  bool &success){
	// if commandSet had more than one command, was the last string
	// on the last command a connector
	// we should only check the connector if there is only more than one
	// set of commands to execute


	if((execi > 0) && (commandSetsize > 1)){

		 //HandleConnectors();
		 log("Searching for a logical connector")
		 // Grab a ptr to the prevois vector
		 // This is why we must have more than one vector
		 vector<string> tmp = execCommandSet[execi-1];
		 dumpCommandVector(tmp);
		 string connector = tmp[tmp.size()-1];
		 // if the connector was a ; execute the command
		 // if it was || execute only if the last one failed
		 // if it was a && execute if it succeded
		 if(connector.compare("||") == 0) {
				 orConnector(doExecution, success, execi, commandSetsize,
								 execCommandSet, resetExecution);
		 }
		 else if(connector.compare("&&") == 0){
				 log("Found a && connector")
				 doExecution = success;
				 string yes = "Will execute beacuse prevois command succeded";
				 string no = "Not executing next command because prevois command failed";
				 string logs = ((doExecution == true) ? yes : no);
				 log(logs)


		 }

    }

	// if commandSet had more than one command, was the last string
		// on the last command a connector
		// we should only check the connector if there is only more than one
		// set of commands to execute
		if(commandSetsize > 1){

			//HandleConnectors();
			log("Searching for redirection a connector")
			// Grab a ptr to the prevois vector
			// This is why we must have more than one vector
			vector<string> LeftHandSide = execCommandSet[execi];
			vector<string> RightHandSide;
			string connector = LeftHandSide[LeftHandSide.size()-1];

			log(connector)
			if(connector.compare(">>") == 0){
				log("Found a >> connector")
				// mark the next command as processed
				execi++;
				RightHandSide = execCommandSet[execi];
				// handle right redirection with append
				rightRedirectionAppend(LeftHandSide, RightHandSide);
				return true;
			}
			else if(connector.compare(">") == 0){
				log("Found a > connector")
				// mark the next command as processed
				execi++;
				RightHandSide = execCommandSet[execi];
				// handle right redirection with append
				rightRedirection(LeftHandSide, RightHandSide);
				return true;
			}
			// Hmmm does this need to be here
			// i should handle pipes in a loop
			else if(connector.compare("<") == 0){
				log("Found a < connector")
				// mark the next command as processed
				execi++;
				RightHandSide = execCommandSet[execi];
				handleLeftRedirect(LeftHandSide, RightHandSide );
				return true;
				// handle right redirection with append
			}else if(connector.compare("|") == 0){
				// Neesd to handle more than one pipe
				log("Found a | connector")
				// mark the next command as processed
				//execi++;
				//RightHandSide = execCommandSet[execi];
				int pipesExecuted = ExecutePipes(execi, execCommandSet);
				// Advance for the amount of pipes
				execi += pipesExecuted;
				return true;
				// handle right redirection with append
			}

		}



	return false;




}

int Shell::SpawnPipeProc(int in, int out, vector<string>  commandSet)
{
	log("input FD = " << in  <<" |  output fd = " << out)
	log("Spawning child process for pipe " << commandSet[0])

  pid_t pid;

  if ((pid = fork ()) == 0)
    {

	  if(in != 0){
		  dup2(in, STDIN_FILENO);
	  	  close(STDOUT_FILENO);
	  	  //close(out);
	  }

	  if(out != 0){
		  dup2(out, STDOUT_FILENO);
	  	  close(STDIN_FILENO);
	  	  //close(in);
	  }
	  log("Executing piped child process")
      ExecuteChildProcess(commandSet);
    }

  return pid;
}

#include "SimpleGLibPipe.h"

int Shell::ExecutePipes(int execIndex,
		vector<vector<string> > commandSet){

		log("execute pipes")

		// Find the number of pipes to execute
		// This mean to find &&,||,; then count from our starting pos

		// setup all the pipes to execute
		// Should be N-1 or N-2 if the first command was input redirected
		int numPipes = 0;
		bool inputredir = false;
		log("Finding number of pipes to execute")
		for(unsigned int i = execIndex; i < commandSet.size(); i++){
			vector<string> *tmp = &commandSet[i];
			string  delim = tmp->at(tmp->size()-1);
			//log(delim)
			if((delim == "&&") || (delim == "||") ||(delim == ";") ||(delim == ">") ||(delim == ">>")){

				break;
			}
			numPipes = i;
		}
		log("Found " << numPipes << " pipe to execute")

		if(commandSet[execIndex][commandSet[0].size() - 1] == "<"){
			log("Out initial command was input redirected")
			inputredir = true;
			numPipes--;
		}

	  /* The first process should get its input from the original file descriptor 0.  */
	  int in = 0;
	  /* Note the loop bound, we spawn here all, but the last stage of the pipeline.  */
	  log("Executing " << numPipes << " pipes")
	  //SimpleGlibPipe pipe;
	  int pipefd[2];
	  vector<pid_t> pids;
	  for (int i = execIndex, j = 0; i < execIndex+numPipes ; ++i, j++)
	    {
		  pipe(pipefd);
		  //pipefd.openPipe();

		  log("Pipe opened for command " << commandSet[i][0]  << " to command " << commandSet[i+1][0] )
		  log("readpipe = " << pipefd[0] << "  Write pipe = " << pipefd[1]    )
		  pid_t pid = SpawnPipeProc(in, pipefd[1], commandSet[i]);
	      /* fd[1] is the write end of the pipe, we carry `in` from the prev iteration.  */
	      pids.push_back(pid);
	      /* No need for the write end of the pipe, the child will write here.  */
	      //pipe.closeWritePipe();
	      //close(pipefd[1]);
	     // /* Keep the read end of the pipe, the next child will read from there.  */
	      in = pipefd[0];

	     // ExecuteParentProcess(pid, true);
	    }

	 log("Executing last stage in pipeline")
	  //pipe.setwritePipe(STDOUT_FILENO);
	 //dup(STDOUT_FILENO);

	 pid_t pid;

	  if ((pid = fork ()) == 0)
	    {


		  //close(STDIN_FILENO);
		  dup2(in, STDIN_FILENO);
		  //dup2(STDOUT_FILENO,pipefd[1]);
		  close(pipefd[1]);

		  log("readpipe = " << pipefd[0] << "  Write pipe = " << pipefd[1]    )
		  log("input FD = " << in  <<" |  output fd = " <<  STDOUT_FILENO)
		  log("Executing last piped child process")
	      ExecuteChildProcess(commandSet[execIndex + numPipes]);
	    }

	 close(pipefd[0]);
	 close(pipefd[1]);

	  for(unsigned int i = 0; i < pids.size(); i++){
		  ExecuteParentProcess(pids[i], true);
	  }
	  ExecuteParentProcess(pid, true);

	return (inputredir == false) ? numPipes : numPipes + 1;


}

/// Misc funstion


Shell::~Shell() {
	// TODO Auto-generated destructor stub
}

