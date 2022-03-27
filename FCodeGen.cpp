#include <iostream>
#include <fstream>

using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "FScan.h"
#include "FCodeGen.h"


extern Scanner scan; //Global scanner

CodeGen::CodeGen(){
    maxTemp =0;
}
//-------Private-Methods-------
void CodeGen::CheckId(const string & s)
{
	if (!LookUp(s))  // variable not declared yet
		Enter(s);
}
void CodeGen::Enter(const string & s)
{
	//fakes, bools and ints
	symbolTable.push_back(s);
}
bool CodeGen::LookUp(const string & s)
{
	//loop through all symbol tables int bool string fake
	for (unsigned i =0; i < intTable.size(); i++)
		if (intTable[i].label == s)
			return true;
   // for (unsigned j = 0; j < fakeTable.size(); j++)
    //    if (fakeTable[j].label = s)
    //        return true;


	

	return false;
}
void CodeGen::IntToAlpha(int val, string& str)
{
	int k;
	char temp;

	str = "";
	if (val == 0) str = "0";
	while (val > 0)
	{
		str.append(1, (char)(val % 10 + (int)'0'));
		val /= 10;
	}
	k = int(str.length());
	for (int i = 0; i < k/2; i++)
	{
		temp = str[i];
		str[i] = str[k-i-1];
		str[k-i-1] = temp;
	}
}
void CodeGen::FakeToAlpha(float val, string& str)
{

    str = "";

    cout << "\nFakeToAlpha\n";

    string fakeStr(std::to_string(val));

    str = fakeStr;
    cout << str;



}
void CodeGen::ExtractExpr(const ExprRec & e, string& s){
	string t;
	int k, n;

	switch (e.kind)
	{
	case ID_EXPR:
	case TEMP_EXPR:  // operand form: +k(R15)
		s = e.name;
		n = 0;
		
		while (symbolTable[n] != s) n++;
		k = 2 * n;  // offset: 2 bytes per variable
		IntToAlpha(k, t);
		s = "+" + t + "(R15)";
		break;
	case LITERAL_INT:
		cout <<"lit int\n";
		IntToAlpha(e.val, t);
		s = "#" + t;
	case LITERAL_STR:
		break;
		//s = "+" + to_string(StringSamDistance(stringTable.size()-1))+ "(R14)";
		//s ="+" + to_string(StringSamDistance(stringTable.size()-1)) +"(R14)";
	//WIP case for bool and fakes
    case BOOL_LITERAL:
        break;
    case FAKE_LITERAL:
        cout << "---literal fake\n";
        FakeToAlpha(e.valF, t);
        s = "#" + t;
        break;
		
	}
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
	Generate("LDA		", "R15", "INTS");
	Generate("LDA		", "R14", "FAKES");
	//WIP
	//Generate load address for STRS BOOLS fakes
	//R14 , R13, R12
}
void CodeGen::Finish()
{
	string s;
	listFile.width(6);
	listFile << ++scan.lineNumber << " " << scan.lineBuffer << endl;
	Generate("HALT		", "", "");
	//repeat this chunk for fakes, bools, strings
	//keep in mind the differnt sizes of registers
	Generate("LABEL	", "INTS","");
	IntToAlpha(int(2*(intTable.size()+1)),s);
	Generate("SKIP	", s, "");
	//WIP Need tables for str bool and fake
    //bools 2nd

    Generate("LABEL	", "FAKES", "");
    IntToAlpha(int(4*(fakeTable.size()+1)),s);
    Generate("SKIP	", s, "");



	outFile.close();


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
	//WIP .kind of lit str bool and fake
	if(outExpr.kind == LITERAL_STR){
		//wrtie string
	}
	if(outExpr.kind == LITERAL_INT){
		string s;
		
		ExtractExpr(outExpr, s);
		Generate("WRI		", s, "");
	}
    if (outExpr.kind == LITERAL_FAKE){
        string f;
        cout << "/nWriteExpr .kind = LITERAL_FAKE\n";
        ExtractExpr(outExpr, f);
        Generate("WRF       ", f, "");
    }
}
void CodeGen::NewLine()
{
	Generate("WRNL		", "", "");
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
string CodeGen::GetTemp(){
	string s;
	static string t;
	t = "Temp&";
	IntToAlpha(++maxTemp, s);
	t += s;
	CheckId(t);
	return t;
}
string ExtractOp(const OpRec& o){
	switch(o.op){
		case PLUS:
			return "IA		";
		case MINUS:
			return "IS		";
		//WIP
		//find sam instructions for mul div mod
	}	

}
void CodeGen::GenInfix(const ExprRec & e1, const OpRec & op, const ExprRec & e2, ExprRec& e)
{
	string s;
	if(e1.kind == ID_EXPR && e2.kind == ID_EXPR){
		e.kind = ID_EXPR;
		switch(op.op){
			case PLUS:
				e.val = e1.val + e2.val;
				break;
			case MINUS:
				e.val = e1.val - e2.val;
			case MULT:
				e.val = e1.val * e2.val;
			case DIV:
				e.val = e1.val / e2.val;
			case MOD:
				e.val = e1.val % e2.val;
		}
	}else{
		e.kind = TEMP_EXPR;
		e.name = GetTemp();
		ExtractExpr(e1,s);
		Generate("LD		","R0",s);
		ExtractExpr(e2, s);
		Generate(ExtractOp(op), "R0", s);
		ExtractExpr(e, s);
		Generate("STO       ", "R0", s);

	}
}

void CodeGen::ProcessMulOp()
{

}
void CodeGen::ProcessId(ExprRec& e)
{
	CheckId(scan.tokenBuffer);
	//e.kind = ID_EXPR;
	cout << scan.tokenBuffer;
	e.name = scan.tokenBuffer;
}
void CodeGen::ProcessLiteralInit(ExprRec& e)
{

}


void CodeGen::ProcessLiteral(ExprRec& e)
{
	switch(e.kind){
		case LITERAL_INT:
			//cout << scan.tokenBuffer.data();
			e.val = atoi(scan.tokenBuffer.data());
			//cout << e.val;
			break;
		case LITERAL_STR:
			//push to string table.
			break;
		case LITERAL_BOOL:
			break;
		case LITERAL_FAKE:
            cout << scan.tokenBuffer.data();
            e.valF = atoi(scan.tokenBuffer.data());
            cout << e.valF;
			break;
		case ID_EXPR:
			cout << "expr";
			break;
		case TEMP_EXPR:
			cout << "temp expr";
			break;
		default:
			cout <<"default";
			break;
	}
	
}
/*
void CodeGen::ProcessStringLiteral(ExprRec& e)
{

}
*/
