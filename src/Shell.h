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

	void Run();
	void PrintCommandPrompt();
	string ReadInCommands();
	vector<vector<string> > ParseCommands(string commandStream);

	vector<string> mInternalCommands;
	void setupInternalCommandList();
	int  isInternalCommand(const vector<string> *command);
	bool ExecuteInternalCommand(int pos, const vector<string> *command);
	bool NeedToExit(vector<string> commands);


	bool Execute(vector<string> command);
	bool Execute(vector<string> command, bool wait);
	bool ExecuteCommands(vector<vector<string> > command);

	virtual bool HandleConnectors(int size,
						  int &execi,
						  vector<vector<string> > execCommandSet,
						  bool &doExecution,
						  bool &resetEecution,
						  bool &success);
	bool rightRedirectionAppend(vector<string>& leftHandSide,
							    	   vector<string>& rightHandSide);
	bool rightRedirection(vector<string>& leftHandSide,
								 vector<string>& rightHandSide);
	bool handlePipe(vector<string>& leftHandSide,
					vector<string>& rightHandSide);
	void orConnector(bool& doExecution,
					 bool& success,
					 int execi,
					 int size,
					 vector<vector<string> > execCommandSet,
					 bool& resetExecution);
	void handleChildExecution(vector<string> command);
	bool handleParentExecution(pid_t pid,bool wait);
	void handleLeftRedirect(vector<string>& RightHandSide,
			vector<string>& LeftHandSide);


	vector<string> getEnvVar(string var, string delim);

};

#endif /* RSHELL_H_ */
