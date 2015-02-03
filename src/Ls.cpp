

//============================================================================
// Name        :LS.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Progam LS
//============================================================================

#include <iostream>
#include "Lslib.h"


using namespace std;
void print_information();

int main(int argc, char* argv[]) {

	print_information();

	Lslib ls;

	ls.call_ls(argc, argv);


	return 0;
}


void print_information(){

	cout << "Version: " <<  VERSION << endl;
	cout << "Name" << NAME << endl;
	cout << "=============================" << endl << endl;


}
