/*
 * Cd.h
 *
 *  Created on: Feb 25, 2015
 *      Author: john
 */

#ifndef CD_H_
#define CD_H_

#include <vector>
#include <string>

using std::string;
using std::vector;

class Cd {
public:
	static bool callCD(vector<string> tokens);

private:
	static string handleDotandTilde(const vector<string> tokens);
	static string tildeExpansion();
};

#endif /* CD_H_ */
