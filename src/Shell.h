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
	virtual ~Shell();
private:

	void PrintCommandPrompt();
	void Run();

	string ReadInCommands();
	vector<std::vector<std::string> > ParseCommands(std::string commandStream);
	bool NeedToExit(std::vector<std::string> commands);
	bool ExecuteCommand(std::vector<std::string> command);

	void orConnector(bool& doExecution, bool& success, int execi, int size,
			std::vector<std::vector<std::string> > execCommandSet,
			bool& resetExecution);
	void handleChildExecution(std::vector<std::string> command);
	bool handleParentExecution(pid_t pid,bool wait);


};

#endif /* RSHELL_H_ */
