/*
 * log.h
 *
 *  Created on: Jan 15, 2015
 *      Author: john
 */

#ifndef LOG_H_
#define LOG_H_

// Comment this out to disable debuging
#define DEBUG

#ifdef DEBUG
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#define log(String) \
		cout << __FILE__ << "::"<< __FUNCTION__ << "::"<< __LINE__ << " " << String << endl;
#else
#define log(String)
#endif


#endif /* LOG_H_ */
