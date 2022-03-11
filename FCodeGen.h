#ifndef CODEGEN
#define CODEGEN
#include <string>
#include <vector>
using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "FScan.h"
#include "FScan.h"
extern Scanner scan;
enum OpKind {PLUS, MINUS,MULT,DIV,MOD};

struct OpRec // information about an operator
{
	OpKind op; // operator type
};


class CodeGen{
    public:
    CodeGen();
	void Assign(const ExprRec & target, const ExprRec & source);


    private:
    vector<string> symbolTable;

    void CheckId(const string & s);
    void Enter(const string & s);

    bool LookUp(const string & s);
};

#endif