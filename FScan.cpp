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
	cout << tokenBuffer<<"\n";
    if(tokenBuffer == "BOF") return BEGIN_SYM;
	if (tokenBuffer == "EOF") return END_SYM;
	if (tokenBuffer == "FIN") return FIN_SYM;
	if (tokenBuffer == "FOUT") return FOUT_SYM;
	if (tokenBuffer == "FOUTLN") return FOUTLN_SYM;
	if (tokenBuffer == "INT") return INT_SYM;
	if (tokenBuffer == "FAKE") return FAKE_LITERAL;
	if (tokenBuffer == "BOOL") return BOOL_LITERAL;
	if (tokenBuffer == "STR") return STR_LITERAL;
	if (tokenBuffer == "FOR") return FOR_SYM;
	if (tokenBuffer == "FIF") return FIF_SYM;
	if (tokenBuffer == "FELSE") return FELSE_SYM;
	if (tokenBuffer == "FENDIF") return FENDIF_SYM;
	cout << "not reserved"<<"\n";
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
			currentChar = NextChar();
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
				
				if(sourceFile.peek()=='/'){
					//comment
					
					currentChar=NextChar();
					do{
						currentChar = NextChar();
						cout << currentChar;
					}while (currentChar != '\n');
					break;
				}else if(sourceFile.peek()=='*'){
					//multiLine
					currentChar=NextChar();
					do
						currentChar = NextChar();
					while (currentChar == '*' && sourceFile.peek() == '/');
					break;
				}else{
					return DIV_OP;
				}
				
			case '*':
				return MUL_OP;
			case '%':
				return MOD_OP;
			case ',':
				return COMMA;
			case '>':
				return GT_OP;
			case '=':
				//special case
				cout<<"here\n";
				if (sourceFile.peek() == '='){
					currentChar=NextChar();
					return EQ_OP;
				}else{
					return ASSIGN_OP;
				}
				break;
			default:
			break;
		}
				if(isalpha(currentChar)){
					BufferChar(currentChar);
					
					c= sourceFile.peek();
					while (isalnum(c) || c == '_' || c == ':'){
						currentChar = NextChar();
						BufferChar(currentChar);
						c= sourceFile.peek();

					}
					
					return CheckReserved();
				}else if (currentChar == '"'){
					//input:  "hello \"world\""
					//output: hello "world"
					//Reasoning: SAM has a different escape char.
					c = sourceFile.peek();
					char last = '"';
					cout << "in \"";
					while (!(c == '"') || (last == '\\')){
						if (c == '\n'){
							//here we could add multiline strings
							//formating reasons.
							break;//remove break for multi line
						}else{
							currentChar = NextChar();
							if(c == '\\' && last != '\\'){
								//escapes the escape char
							}else{
								BufferChar(currentChar);
							}
							last = c;
							c= sourceFile.peek();
						}
					}
					currentChar = NextChar();
					cout << tokenBuffer;
					return STR_LITERAL;
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