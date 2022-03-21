#ifndef SCANNER
#define SCANNER
#include <string>

const int ID_STRING_LEN = 32;

enum Token  { BEGIN_SYM, END_SYM, EOF_SYM, 
				LPAREN, RPAREN, SEMICOLON, COMMA,CMP_OP,
				ASSIGN_OP, ADD_OP, SUB_OP,MUL_OP,DIV_OP,MOD_OP,
				ID, INT_LITERAL, STR_LITERAL,
				 FAKE_LITERAL,BOOL_LITERAL,
				 FOR_SYM,FIF_SYM,FELSE_SYM,FENDIF_SYM,LSTAPLE,RSTAPLE,
				AND_SYM,NOT_SYM,OR_SYM,
				INT_SYM,STR_SYM,FAKE_SYM,BOOL_SYM,
				LT_OP,LE_OP,GT_OP,GE_OP,EQ_OP,NE_OP,
				FOUTLN_SYM,FOUT_SYM,FIN_SYM,
				WHILE_SYM,DO_SYM,ENDFOR_SYM,ENDWHILE_SYM,
				FWHILE_SYM};
/*
std::string StrToken[17]  { "BEGIN_SYM", "END_SYM", "EOF_SYM", 
				"LPAREN","RPAREN", "SEMICOLON", "COMMA","CMP_OP",
				"ASSIGN_OP", "ADD_OP", "SUB_OP", "MUL_OP","DIV_OP","MOD_OP",
				"ID", "INT_LITERAL", "STR_LITERAL",
				"FAKE_LITERAL","BOOL_LITERAL",
				"FOR_SYM","IF_SYM","ELSE_SYM","END_IF_SYM","Left_Staple","Right_Staple",
				"AND_SYM","NOT_SYM","OR_SYM",
				"INT_SYM","STR_SYM","FAKE_SYM","BOOL_SYM",
				"LT_OP","LE_OP","GT_OP","GE_OP","EQ_OP","NE_OP",
				"FOUTLN_SYM","FOUT_SYM","FIN_SYM",
				"WHILE_SYM","DO_SYM","ENDFOR_SYM","ENDWHILE_SYM",
				"FWHILE_SYM"};                 
*/
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
