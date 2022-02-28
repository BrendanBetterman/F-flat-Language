#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "FScan.h"

// -----------------
// -- Constructor --
// -----------------

Scanner::Scanner(){
    tokenBuffer = "";
    lineBuffer =  "";
    lineNumber =   0;
}

// -----------------
// --   Private   --
// -----------------

void Scanner::BufferChar(char c){
    if(tokenBuffer.length() < ID_STRING_LEN){
        tokenBuffer += toupper(c);
    }
}

Token Scanner::CheckReserved(){
    if(tokenBuffer == "BOF") return BEGIN_SYM;
	if (tokenBuffer == "EOF") return END_SYM;
	if (tokenBuffer == "READ") return READ_SYM;
	if (tokenBuffer == "WRITE") return WRITE_SYM;
	if (tokenBuffer == "int") return INT_LITERAL;
	if (tokenBuffer == "fake") return REAL_LITERAL;
	if (tokenBuffer == "bool") return BOOL_LITERAL;
	if (tokenBuffer == "str") return STR_LITERAL;
	if (tokenBuffer == "for") return FOR_SYM;
	if (tokenBuffer == "fif") return IF_SYM;
	if (tokenBuffer == "felse") return ELSE_SYM;
	if (tokenBuffer == "fifend") return END_IF_SYM;

    return ID;
}
void Scanner::ClearBuffer(){
    tokenBuffer = "";
}
void Scanner::LexicalError(char& c){
    cout << " *** Lex Error: '" << c
		<< "' ignored at position " << int(lineBuffer.size())
		<< " on line #" << lineNumber+1 << '.' << endl;
	listFile << " *** Lex Error: '" << c
		<< "' ignored at position " << int(lineBuffer.size())
		<< " on line #" << lineNumber+1 << '.' << endl;
	c = NextChar();
}
char Scanner::NextChar(){
    char c;
	sourceFile.get(c);
	if (c == '\n')
	{
		listFile.width(6);
		listFile << ++lineNumber << "  " << lineBuffer << endl;
		lineBuffer = "";
	}
	else
		lineBuffer += c;
	return c;
}

// -----------------
// --   Private   --
// -----------------

