#include <iostream>
#include <fstream>
#include <string>
using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

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
	if (tokenBuffer == "READ") return FIN_SYM;
	if (tokenBuffer == "WRITE") return FOUT_SYM;
	if (tokenBuffer == "int") return INT_LITERAL;
	if (tokenBuffer == "fake") return FAKE_LITERAL;
	if (tokenBuffer == "bool") return BOOL_LITERAL;
	if (tokenBuffer == "str") return STR_LITERAL;
	if (tokenBuffer == "for") return FOR_SYM;
	if (tokenBuffer == "fif") return FIF_SYM;
	if (tokenBuffer == "felse") return FELSE_SYM;
	if (tokenBuffer == "fifend") return FENDIF_SYM;

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
// --   public   --
// -----------------

Token Scanner::GetNextToken(){
    char currentChar, c;
    ClearBuffer();
    currentChar= NextChar();
    while (!sourceFile.eof())
    {
        /* code either switch statment or if wall */
		if(isspace(currentChar)){
			currentChar = Nextchar();
		}
		switch(currentChar){
			case '(':
				return LPAREN;
			case ')':
				return RPAREN;
			case ';':
				return SEMICOLON;
			case '+':
				return ADD_OP;
			case '-':
				return SUB_OP;
			case '/':
				return DIV_OP;
			case '*':
				return MUL_OP;
			case '%':
				return MOD_OP;
			case ',':
				return COMMA;
			case '=':
				//special case
				if (sourceFile.peek() == '='){
					return CMP_OP;
				}else{
					return ASSIGN_OP;
				}
				break;
			default:
				if(isalpha(currentChar)){
					BufferChar(currentChar);
					while (isalnum(c) || c == '_' || c == ':'){
						currentChar = NextChar();
						BufferChar(currentChar);
						c= sourceFile.peek();
					}
					return CheckReserved();
				}else if(isdigit(currentChar)){

					//Fakes and Ints
					BufferChar(currentChar);
					c = sourceFile.peek();
					while (isdigit(c)){
						currentChar = NextChar();
						BufferChar(currentChar);
						c= sourceFile.peek();
					}
					if(c =='.'){
						//fake stuff
						currentChar = NextChar();
						BufferChar(currentChar);
						c= sourceFile.peek();
						while(isdigit(c)){
							currentChar = NextChar();
							BufferChar(currentChar);
							c= sourceFile.peek();
						}
						if(isdigit(currentChar)){
							//check if something is after the . example 1.0e
							c= sourceFile.peek();
							if(c=='e' || c =='E'){
								currentChar = NextChar();
								BufferChar(currentChar);
								c= sourceFile.peek();
								if(c=='-'){
									currentChar = NextChar();
									BufferChar(currentChar);
									c= sourceFile.peek();
								}
								while (isdigit(c)){
									currentChar = NextChar();
									BufferChar(currentChar);
									c= sourceFile.peek();
								}
								if(isdigit(currentChar)){
									return FAKE_LITERAL;
								}else{
									LexicalError(currentChar);
								}
							}else{
								//Non Science
							}

						}else{
							//No digit after .
							LexicalError(currentChar);
						}
					}else{
						return INT_LITERAL;
					}


				}else{
					LexicalError(currentChar);
				}

		}
		
    }
    
}