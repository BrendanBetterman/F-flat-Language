#include <iostream>

using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "FScan.h"
#include "FCodeGen.h"

extern Scanner scan; //Global scanner

CodeGen::CodeGen(){
    
}
//Privates
void CodeGen::CheckId(const string & s)
{
	if (!LookUp(s))  // variable not declared yet
		Enter(s);
}
void CodeGen::Enter(const string & s)
{
	symbolTable.push_back(s);
}
bool CodeGen::LookUp(const string & s)
{
	for (unsigned i = 0; i < symbolTable.size(); i++)
	if (symbolTable[i] == s)
		return true;

	return false;
}
//publics
void Assign(const ExprRec & target, const ExprRec & source){
//needs to check if its an int fake boolean or string

}
