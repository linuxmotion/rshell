/*
 * RShell.h
 *
 *  Created on: Jan 6, 2015
 *      Author: john
 */

#ifndef RSHELL_H_
#define RSHELL_H_
#include <string>
#include <vector>

using std::vector;
using std::string;

class Shell {
public:
	Shell();
	void StartShell(int argc, char **argv);
	void PrintCommandPrompt();
	std::string ReadInCommands();
	std::vector<std::vector<std::string> > ParseCommands(std::string commandStream);
	bool NeedToExit(std::vector<std::string> commands);
	bool ExecuteCommand(std::vector<std::string> command);

	virtual ~Shell();

private:
	string mCurrentDirectory;
	void dumpEntireCommandVector(vector<vector<string> >& commandSet);vector<vector<string> > TokenizeToConnectors(vector<vector<string> > completeStream);
	vector<vector<string> > TokenizeToSpaces(vector<vector<string> > completeStream);
	vector<vector<string> > TokenizeToLogicalEND(string completeCommands);
	vector<vector<string> > TokenizeToLogicalOR(vector<vector<string> > parseVector);
	vector<vector<string> > TokenizeToLogicalAND(vector<vector<string> > parseVector);
    vector<vector<string> > TokenizeCommandStream(string commandStream);
	void handleChildExecution(std::vector<std::string> command);
	bool handleParentExecution(pid_t pid,bool wait);
	string tildeExpansion();
	string getHomeDirectory();
	string getCWD();
	bool callCD(std::string path);
};

#endif /* RSHELL_H_ */
