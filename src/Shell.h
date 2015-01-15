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
	std::vector<std::string> ParseCommands(std::string commandStream);
	bool NeedToExit(std::vector<std::string> commands);
	bool ExecuteCommand(std::string* commandsArray, int commandc);

	virtual ~Shell();

private:
	void needToExit();
};

#endif /* RSHELL_H_ */
