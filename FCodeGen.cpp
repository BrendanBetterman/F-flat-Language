#include <iostream>
#include <fstream>

using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "FScan.h"
#include "FCodeGen.h"


extern Scanner scan; //Global scanner

CodeGen::CodeGen(){
    
}
//-------Private-Methods-------
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
void CodeGen::ExtractExpr(const ExprRec & e, string& s){

}
void CodeGen::Generate(const string & s1, const string & s2, const string & s3){
	
	listFile.width(20);
	listFile << ' ' << s1;
	outFile << s1;
	if (s2.length() > 0)
	{
		listFile << s2;
		outFile << s2;
		if (s3.length() > 0)
		{
			listFile << ',' << s3;
			outFile << ',' << s3;
		}
	}
	listFile << endl;
	outFile << endl;
}
//--------Public-Methods--------
void CodeGen::Start()
{

}
void CodeGen::Finish()
{

}
void CodeGen::Assign(const ExprRec & target, const ExprRec & source)
{
//needs to check if its an int fake boolean or string
	string s;
	ExtractExpr(source, s);
	Generate("LD		", "R0", s);
	ExtractExpr(target, s);
	Generate("STO		", "R0", s);
}
void CodeGen::ReadValue(const ExprRec & InVal)
{

}
void CodeGen::ProcessVariable()
{

}
void CodeGen::WriteExpr(const ExprRec & outExpr)
{
	if(outExpr.kind == LITERAL_STR){
		//wrtie string
	}
	//if(outExpr.kind == LITERAL_INT){
		string s;
		ExtractExpr(outExpr, s);
		Generate("WRI		", s, "");
	//}
}
void CodeGen::NewLine()
{

}
//---fif---
void CodeGen::ProcessIf()
{

}
void CodeGen::ProcessEndIf()
{

}
void CodeGen::ProcessElse()
{

}
//---do-while---
void CodeGen::ProcessDo()
{

}
void CodeGen::ProcessEndFwhile()
{

}
//---while---
void CodeGen::ProcessWhile()
{

}
void CodeGen::ProcessWhileCond()
{

}
void CodeGen::ProcessEndWhile()
{

}
//---for-loop---
void CodeGen::InitLoopCtrl()
{

}
void CodeGen::ProcessForCond()
{

}
void CodeGen::LoopUpdate()
{

}
void CodeGen::ProcessEndFor()
{

}
//---Conditions---
void CodeGen::SetOrCond()
{

}
void CodeGen::SetAndCond()
{

}
void CodeGen::SetNotCond()
{

}
//---Ops---
void CodeGen::ProcessRelOp()
{

}
void CodeGen::SetRelCond()
{

}
void CodeGen::ProcessOp(OpRec& o)
{

}

void CodeGen::GenInfix(const ExprRec & e1, const OpRec & op, const ExprRec & e2, ExprRec& e)
{

}

void CodeGen::ProcessMulOp()
{

}
void CodeGen::ProcessId(ExprRec& e)
{
	CheckId(scan.tokenBuffer);
	e.kind = ID_EXPR;
	e.name = scan.tokenBuffer;
}
void CodeGen::ProcessLiteralInit(ExprRec& e)
{

}

/*
void CodeGen::ProcessLiteral(ExprRec& e)
{

}
void CodeGen::ProcessStringLiteral(ExprRec& e)
{

}
*/
