#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
using namespace std;

                     // GLOBAL FILE STREAMS
ifstream sourceFile; // source program
ofstream outFile,    // object program
         listFile;   // compiler listing

#include "Fscan.h"   // scanner component definition 
#include "Fparse.h"	 // parser component definition
#include "FcodeGen.h"   // code generator component definition

                     // GLOBAL COMPONENT OBJECTS
Scanner scan;        // scanner
Parser parse;        // parser
CodeGen code;        // code generator

string Date();
// Returns the current date.

string Time();
// Returns the current time.

int main(int argc, char* argv[])
{
	string sourceName, outName, listName;

	cout 
		<< "\n" 
		<< " F F L A T   C O M P I L E R  2022 \n"
		<< " ___________________________________\n" 
		<< endl;
        if (argc == 2){
          sourceName = argv[1];
        }
        else {
	  cout << " Source file (.flat extension is assumed): ";
	  getline(cin, sourceName);
	}

   // Add appropriate extensions to file names.
	outName = sourceName + ".asm";
	listName = sourceName + ".lst";
	sourceName += ".flat";

   // Open and initialize all files.
	sourceFile.open(sourceName.data());
	if (!sourceFile.is_open())
	{
		cout << "\n File not found. Compilation aborted!\n\n";
		//cin.get();
		exit(1);
	}
	outFile.open(outName.data());
	listFile.open(listName.data());

	listFile
		<< "\n\n F F L A T   C O M P I L E R   L I S T I N G\n\n"
		<< "               Version 2022\n\n"
		<< " Generated code is SAM assembly language for\n"
		<< " the MACC2 virtual computer.\n"
		<< " ___________________________________________\n\n";
	listFile << ' ' + Date() << " at " << Time() << endl;
	listFile << " Source file: " << sourceName << endl << endl;
	listFile << " LINE #" << endl;

	parse.SystemGoal();

	cout << endl
		<< "\n Successful Compilation\n"
		<< "\n Object code --> " << outName << endl
		<< "\n Listing file --> " << listName << endl << endl; 

//	cin.get();
	return 0;
}

string Date()
{
	const time_t current_time = time (NULL);
	char the_date[20];

	strftime (the_date, 20, "%B %d, %Y", localtime (&current_time));

	return the_date;
}

string Time()
{
	const time_t current_time = time (NULL);
	char the_time[10];


	strftime (the_time, 10, "%I:%M %p", localtime (&current_time));
	
	return the_time;
}