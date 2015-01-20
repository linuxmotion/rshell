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
	std::string mCurrentDirectory;
	void needToExit();
	void handleChildExecution(std::vector<std::string> command);
	bool handleParentExecution(pid_t pid,bool wait);
	std::string tildeExpansion();
	std::string getHomeDirectory();
	std::string getCWD();
	bool callCD(std::string path);
};

#endif /* RSHELL_H_ */
