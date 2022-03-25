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
        tokenBuffer += c;
    }
}

Token Scanner::CheckReserved(){
	cout << tokenBuffer<<"\n";
    if(tokenBuffer == "BOF") return BEGIN_SYM;
	if (tokenBuffer == "EOF") return END_SYM;
	if (tokenBuffer == "fin") return FIN_SYM;
	if (tokenBuffer == "fout") return FOUT_SYM;
	if (tokenBuffer == "foutln") return FOUTLN_SYM;
	if (tokenBuffer == "int") return INT_SYM;
	if (tokenBuffer == "fake") return FAKE_SYM;
	if (tokenBuffer == "bool") return BOOL_SYM;
	if (tokenBuffer == "str") return STR_SYM;
	if (tokenBuffer == "for") return FOR_SYM;
	if (tokenBuffer == "fif") return FIF_SYM;
	if (tokenBuffer == "felse") return FELSE_SYM;
	if (tokenBuffer == "fendif") return FENDIF_SYM;
	if (tokenBuffer == "nay") return BOOL_LITERAL;
	if (tokenBuffer == "yay") return BOOL_LITERAL;
	//missing reserves
	if(tokenBuffer == "do") return DO_SYM;
	if(tokenBuffer == "endfor") return ENDFOR_SYM;
	if(tokenBuffer == "fwhile") return FWHILE_SYM;
	if(tokenBuffer == "endWhile") return ENDWHILE_SYM;
	if(tokenBuffer == "while") return WHILE_SYM;
	
	if(tokenBuffer == "f!") return NOT_SYM;
	if(tokenBuffer == "f&") return AND_SYM;
	if(tokenBuffer == "f|") return OR_SYM;
	/*
	f!
	f&
	f|
	*/
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
bool Scanner::AcceptedChar(char c){
	
	char NotAccepted[] ={'(',')','[',']','{','}','+','=','-','*','&','|'};
	for (int i = 0; i < 11; i++){
		if(c==NotAccepted[i]) return false;
	}
	return true;
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
				//cout << sourceFile.peek();
				
				if(sourceFile.peek()=='/'){
					//comment
					
					currentChar=NextChar();
					do{
						currentChar = NextChar();
					}while (currentChar != '\n');
					//needs to return no op
					break;
				}else if(sourceFile.peek()=='*'){
					//multiLine
					currentChar=NextChar();
					do{
						currentChar = NextChar();
						cout << currentChar;
					}while (currentChar != '*' && sourceFile.peek() != '/');
					currentChar = NextChar();//ignores *
					currentChar = NextChar();//ignores / 
					//needs to return no op
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
				c = sourceFile.peek();
				if ( c == '='){
					currentChar = NextChar();
					return GE_OP;
				}	
				else			
					return GT_OP;
			case '<':
			    c = sourceFile.peek();
				if ( c == '=' ){
					currentChar = NextChar();
					return LE_OP;
				}
				else
					return LT_OP;
			case '=':
				//special case
				if (sourceFile.peek() == '='){
					currentChar=NextChar();
					return EQ_OP;
				}else{
					return ASSIGN_OP;
				}
				break;
			case '!':
				c = sourceFile.peek();
				if ( c == '='){
					currentChar = NextChar();
					return NE_OP;
				}
				
			case '[':
				return LSTAPLE;
		    case ']':
				return RSTAPLE;
			default:
			break;
		}
				if(isalpha(currentChar)){
					BufferChar(currentChar);
					c= sourceFile.peek();
					while ((isalnum(c) && AcceptedChar(c))){
						if (c=='\n' || c == ' ' ){
							NextChar();
							if(c =='\n')lineNumber++;
							break;}
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
					
					while (!(c == '"') || (last == '\\')){
						if (c == '\n'){
							lineNumber++;
							//currentChar = NextChar();
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
						if(c == '.'){
							LexicalError(currentChar);
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
								return FAKE_LITERAL;
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
	return EOF_SYM;
}