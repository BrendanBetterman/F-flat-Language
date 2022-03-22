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
    void Start();
    void Finish();
	void Assign(const ExprRec & target, const ExprRec & source);
    void ReadValue(const ExprRec & InVal);
    void ProcessVariable();
    void WriteExpr(const ExprRec & outExpr);
    void NewLine();

    void ProcessIf();
    void ProcessEndIf();
    void ProcessElse();

    void ProcessDo();
    void ProcessEndFwhile();

    void ProcessWhile();
    void ProcessWhileCond();
    void ProcessEndWhile();

    void InitLoopCtrl();
    void ProcessForCond();
    void LoopUpdate();
    void ProcessEndFor();

    void SetOrCond();
    void SetAndCond();
    void SetNotCond();

    void ProcessRelOp();
    void SetRelCond();
    void ProcessOp(OpRec& o);

    void GenInfix(const ExprRec & e1, const OpRec & op, const ExprRec & e2, ExprRec& e);

    void ProcessMulOp();
    void ProcessId(ExprRec& e);
    void ProcessLiteralInit(ExprRec& e);

    //void ProcessLitFake(ExprRec& e);
    //void ProcessStringLiteral(ExprRec& e);








    private:
    vector<string> symbolTable;

    void CheckId(const string & s);
    void Enter(const string & s);

    bool LookUp(const string & s);


};

#endif
