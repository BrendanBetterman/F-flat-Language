#ifndef CODEGEN
#define CODEGEN
#include <string>
#include <vector>
using namespace std;

#include "FScan.h"


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