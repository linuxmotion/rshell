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
	vector<vector<string> > ParseCommands(string commandStream);
	bool NeedToExit(vector<string> commands);
	bool Execute(vector<string> command);
	void ExecuteCommands(vector<vector<string> > command);

	void orConnector(bool& doExecution, bool& success, int execi, int size,
			vector<vector<string> > execCommandSet,
			bool& resetExecution);
	void handleChildExecution(vector<string> command);
	bool handleParentExecution(pid_t pid,bool wait);


};

#endif /* RSHELL_H_ */
