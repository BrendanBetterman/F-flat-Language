#ifndef CODEGEN
#define CODEGEN
#include <string>
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

};

#endif