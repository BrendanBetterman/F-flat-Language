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
enum ExprKind { ID_EXPR, LITERAL_INT, TEMP_EXPR, LITERAL_STR, LITERAL_FAKE, LITERAL_BOOL };

struct ExprRec // information about a constant, variable, or
               // an intermediate (temporary) result
{
   ExprKind kind;   // operand type
   string   name;   // used when kind is ID_EXPR or TEMP_EXPR
   int      val;    // used when kind is LITERAL_EXPR
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
    void ProcessId(ExprRec& e);

};

#endif