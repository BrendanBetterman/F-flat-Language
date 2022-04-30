#ifndef CODEGEN
#define CODEGEN
#include <string>
#include <vector>
using namespace std;

#include "FScan.h"

enum ConKind{EQ,NE,LT,GT,LE,GE};
struct ConRec{
    ConKind con;
};
enum OpKind {PLUS, MINUS,MULT,DIV,MOD};

struct OpRec // information about an operator
{
	OpKind op; // operator type
};
enum ExprKind { 
 LITERAL_INT, LITERAL_FAKE, LITERAL_BOOL, LITERAL_STR,
 ID_EXPR, IDF_EXPR, IDB_EXPR, IDS_EXPR,
 TEMP_EXPR,TEMPF_EXPR };

struct ExprRec // information about a constant, variable, or
               // an intermediate (temporary) result
{
   ExprKind kind;   // operand type
   string   name;   // used when kind is ID_EXPR, TEMP_EXPR, IDF_EXPR, or TEMPF_EXPR
   int      val;    // used when kind is LITERAL_EXPR
   float    valF;   // used when kind is LITERAL_FAKE


};
struct Symbol{
    ExprKind kind;
    int off;
    string label;
};
//for parse gen math equations
struct Math{
    ExprRec expr;
    OpKind op;
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

    void ProcessIf(ExprRec& expr,ConRec& con,ExprRec& expr2);
    void ProcessEndIf();
    void ProcessElse();

    void ProcessDo();
    void ProcessEndFwhile(ExprRec& Lexpr,ConRec& con,ExprRec& Rexpr);

    void ProcessWhile(ExprRec& Lexpr,ConRec& con,ExprRec& Rexpr);
    void ProcessWhileCond();
    void ProcessEndWhile();

    void InitLoopCtrl();
    void ProcessForCond(ExprRec& Lexpr,ConRec& con,ExprRec& Rexpr);
    void LoopUpdate();
    void ProcessEndFor();

    void SetOrCond();
    void SetAndCond();
    void SetNotCond();

    void ProcessRelOp();
    void SetRelCond();
    void ProcessOp(OpRec& o);

    void GenInfix(ExprRec & e1, const OpRec & op,  ExprRec & e2, ExprRec& e);

    void ProcessMulOp();
    void ProcessId(ExprRec& e);
    void ProcessLiteralInit(ExprRec& e);
    void ProcessLiteral(ExprRec& e);

    ExprKind GetSymbolTableKind(string & s);

    //void ProcessLitFake(ExprRec& e);
    //void ProcessStringLiteral(ExprRec& e);


private:
    int maxTemp;
    int intoff;
    int fakoff;
    int stroff;
    int booloff;
    //loop stacks
    int fifId;
    int whlId;

    vector<Symbol> symbolTable;
    vector<Symbol> intTable;
    vector<Symbol> boolTable;
    vector<string> fakeTable;
    vector<string> stringTable;
    //loop stacks
    vector<string> fifStack;
    vector<string> whileStack;
    
    string kindtoStr(const ExprKind& t);
    string ConvertToSam(string s);
    int StringSamDistance(int index);
    void CheckId(const string & s, ExprKind & t);
    void Enter(const string & s, ExprKind & t);
    void IntToAlpha(int val, string& str);
    void FakeToAlpha(float val, string& str);
    bool LookUp(const string & s,ExprKind & t);
    bool isFake(ExprKind& kind);
    bool isInt(ExprKind& kind);
    int getOff(string& s);

    void Condition(ExprRec& expr,ConRec& con,ExprRec& expr2);
    void Jump(ConRec& con,string& label);
    void NIJump(ConRec& con,string& label);
    void ExtractExpr(const ExprRec & e, string& s);
    string ExtractOp(const OpRec & o, ExprKind & k);
    string GetTemp();
    string GetTempF();
    void Generate(const string & s1, const string & s2, const string & s3);

};

#endif
