#ifndef SCANNER
#define SCANNER
#include <string>

const int ID_STRING_LEN = 32;

enum Token  { BEGIN_SYM, END_SYM, EOF_SYM, 
				READ_SYM, WRITE_SYM,
				LPAREN, RPAREN, SEMICOLON, COMMA,
				ASSIGN_OP, PLUS_OP, MINUS_OP,
				ID, INT_LITERAL, STR_LITERAL,
				 REAL_LITERAL,BOOL_LITERAL};
std::string StrToken[17]  { "BEGIN_SYM", "END_SYM", "EOF_SYM", 
				"READ_SYM", "WRITE_SYM",
				"LPAREN","RPAREN", "SEMICOLON", "COMMA",
				"ASSIGN_OP", "PLUS_OP", "MINUS_OP",
				"ID", "INT_LITERAL", "STR_LITERAL",
				 "REAL_LITERAL","BOOL_LITERAL"};                 

class Scanner{
public:
    std::string tokenBuffer,lineBuffer;
    int lineNumber;

    Scanner();

    Token GetNextToken();

private:
    void BufferChar(char c);

    Token CheckReserved();

    void ClearBuffer();

    void LexicalError(char& c);

    char NextChar();
};
#endif
