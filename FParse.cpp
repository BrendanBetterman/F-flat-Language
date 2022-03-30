// f_flat Grammar 2022
// 
// Parser Class Implementation File

// SHELL GENERATED BY PARSERGEN VERSION 5.0.4

#include <iostream>
#include <string>
using namespace std;

#include "FScan.h"
#include "FParse.h"
#include "FCodeGen.h"

extern Scanner scan;
extern CodeGen code;

Parser::Parser()
{
	tokenAvailable = false;
}

void Parser::SyntaxError(Token t, string msg)
{
	cout << "line #" <<scan.lineNumber+1;
	cout << "position "<<scan.lineBuffer.size();
	
	cout << "Char" << scan.tokenBuffer;
	cout << "Syntax Error: " + msg << endl;
	exit(1); // abort on any syntax error
}

Token Parser::NextToken()
{
	if (!tokenAvailable)
	{
		
		savedToken = scan.GetNextToken();
		
		tokenAvailable = true;
	}
	return savedToken;
}

void Parser::Match(Token t)
{
	

	if (t != NextToken()){
	cerr << "no token\n";
		SyntaxError(t, "");
	}
	else{
		tokenAvailable = false;
		//cerr<< "token got\n";
	}
	
		
}


void Parser::VarDec()
{
	Match(ID);
	VarDecTail();
}

void Parser::VarDecList()
{
	VarDec();
	VarDecListTail();
}

void Parser::VarDecListTail()
{
	switch (NextToken())
	{
	case COMMA:
		Match(COMMA);
		VarDec();
		VarDecListTail();
		break;
	case SEMICOLON:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::VarDecTail()
{
	switch (NextToken())
	{
	case LSTAPLE:
		Match(LSTAPLE);
		Match(INT_LITERAL);
		Match(RSTAPLE);
		break;
	case SEMICOLON:
	case COMMA:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::DecTail()
{
	ExprRec expr;
	switch (NextToken())
	{
	case ASSIGN_OP:
		Match(ASSIGN_OP);
		Literal();
		code.ProcessLiteral(expr);
		cout << "Assign OP\n";
		cout << scan.tokenBuffer;
		break;
	case LSTAPLE:
	case SEMICOLON:
	case COMMA:
		VarDecTail();
		VarDecListTail();
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Type(ExprRec& expr)
{
	switch (NextToken())
	{
	case INT_SYM:
		Match(INT_SYM);
		expr.kind=LITERAL_INT;
		break;
	case BOOL_SYM:
		Match(BOOL_SYM);
		expr.kind = LITERAL_BOOL;
		break;
	case FAKE_SYM:
		Match(FAKE_SYM);
		expr.kind = LITERAL_FAKE;
		break;
	case STR_SYM:
		Match(STR_SYM);
		expr.kind = LITERAL_STR;
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Literal()
{
	switch (NextToken())
	{
	case INT_LITERAL:
		Match(INT_LITERAL);
		break;
	case BOOL_LITERAL:
		Match(BOOL_LITERAL);
		break;
	case FAKE_LITERAL:
		Match(FAKE_LITERAL);
		break;
	case STR_LITERAL:
		Match(STR_LITERAL);
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::MultOp()
{
	switch (NextToken())
	{
	case MUL_OP:
		Match(MUL_OP);
		break;
	case DIV_OP:
		Match(DIV_OP);
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::FactorTail()
{
	ExprRec expr;
	switch (NextToken())
	{
	case MUL_OP:
	case DIV_OP: //Real Div
		MultOp();
		Primary(expr);
		// code.GenInfix();
		cout << "Mul or div\n";
		FactorTail();
		break;
	case AND_SYM:
	case NOT_SYM:
	case RSTAPLE:
	case RPAREN:
	case SEMICOLON:
	case COMMA:
	case ADD_OP:
	case SUB_OP:
	case LT_OP:
	case LE_OP:
	case GT_OP:
	case GE_OP:
	case EQ_OP:
	case NE_OP:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Primary(ExprRec& result)
{
	
	
	switch (NextToken())
	{
	case INT_LITERAL:
		result.kind = LITERAL_INT;
		Literal();
		code.ProcessLiteral(result);
		cout << "Process Literal INT\n";
		break;
	case FAKE_LITERAL:
		result.kind = LITERAL_FAKE;
		Literal();
		code.ProcessLiteral(result);
		cout << "Process Literal Fake\n";
		break;
	case STR_LITERAL:
		result.kind = LITERAL_STR;
		Literal();
		code.ProcessLiteral(result);
		cout << "Process Literal STR\n";
		break;
	case BOOL_LITERAL:
		result.kind = LITERAL_BOOL;
		Literal();
		code.ProcessLiteral(result);
		cout << "Process Literal Bool\n";
		break;
	case ID:
		Variable(result);
		break;
	case LPAREN:
		Match(LPAREN);
		Condition(result);
		Match(RPAREN);
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::AddOp(OpRec& op)
{
	switch (NextToken())
	{
	case ADD_OP:
		Match(ADD_OP);
		code.ProcessOp(op);
		break;
	case SUB_OP:
		Match(SUB_OP);
		code.ProcessOp(op);
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::ExprTail()
{
	OpRec op;
	switch (NextToken())
	{
	case ADD_OP:
	case SUB_OP:
		AddOp(op);
		Factor();
		ExprTail();
		break;
	case AND_SYM:
	case NOT_SYM:
	case RSTAPLE:
	case RPAREN:
	case SEMICOLON:
	case COMMA:
	case LT_OP:
	case LE_OP:
	case GT_OP:
	case GE_OP:
	case EQ_OP:
	case NE_OP:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Factor()
{
	ExprRec expr;
	Primary(expr);
	//code.GenInfix(expr);
	cout << "Get infix\n";
	FactorTail();
}

void Parser::RelOp()
{
	switch (NextToken())
	{
	case LT_OP:
		Match(LT_OP);
		break;
	case LE_OP:
		Match(LE_OP);
		break;
	case GT_OP:
		Match(GT_OP);
		break;
	case GE_OP:
		Match(GE_OP);
		break;
	case EQ_OP:
		Match(EQ_OP);
		break;
	case NE_OP:
		Match(NE_OP);
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::RelTail()
{
	ExprRec expr;
	switch (NextToken())
	{
	case LT_OP:
	case LE_OP:
	case GT_OP:
	case GE_OP:
	case EQ_OP:
	case NE_OP:
		RelOp();
		Expression(expr);
		break;
	case AND_SYM:
	case NOT_SYM:
	case RPAREN:
	case SEMICOLON:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Relational()
{
	ExprRec expr;
	Expression(expr);
	RelTail();
}

void Parser::AndTail()
{
	switch (NextToken())
	{
	case AND_SYM:
		Match(AND_SYM);
		Negation();
		AndTail();
		break;
	case NOT_SYM:
	case RPAREN:
	case SEMICOLON:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Negation()
{
	switch (NextToken())
	{
	case NOT_SYM:
		Match(NOT_SYM);
		Relational();
		break;
	case AND_SYM:

	case RPAREN:
	case SEMICOLON:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::CondTail()
{
	switch (NextToken())
	{
	case NOT_SYM:
		Match(NOT_SYM);
		AndCond();
		break;
	case RPAREN:
	case SEMICOLON:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::AndCond()
{
	Negation();
	AndTail();
}

void Parser::VarInit()
{
	ExprRec expr;
	switch (NextToken())
	{
	case INT_SYM:
		Match(INT_SYM);
		Match(ID);
		break;
	case ID:
		Variable(expr);
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::FelseClause()
{
	ExprRec expr;
	switch (NextToken())
	{
	case FELSE_SYM:
		Match(FELSE_SYM);
		StmtList(expr);
		break;
	case FENDIF_SYM:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Condition(ExprRec& expr)
{
	cout << "Condition";
	AndCond();
	CondTail();
}

void Parser::ForStmt()
{
	ExprRec expr;
	Match(FOR_SYM);
	Match(LPAREN);
	VarInit();
	Match(ASSIGN_OP);
	Expression(expr);
	Match(SEMICOLON);
	Condition(expr);
	Match(SEMICOLON);
	Variable(expr);
	Match(ASSIGN_OP);
	Expression(expr);
	Match(RPAREN);
	StmtList(expr);
	Match(ENDFOR_SYM);
}

void Parser::DoFwhileStmt()
{
	ExprRec expr;
	Match(DO_SYM);
	StmtList(expr);
	Match(FWHILE_SYM);
	Match(LPAREN);
	Condition(expr);
	Match(RPAREN);
}

void Parser::WhileStmt()
{
	ExprRec expr;
	Match(WHILE_SYM);
	Match(LPAREN);
	Condition(expr);
	Match(RPAREN);
	StmtList(expr);
	Match(ENDWHILE_SYM);
}

void Parser::FifStmt()
{
	ExprRec expr;
	Match(FIF_SYM);
	Match(LPAREN);
	Condition(expr);
	Match(RPAREN);
	StmtList(expr);
	FelseClause();
	Match(FENDIF_SYM);
}

void Parser::ItemListTail()
{
	ExprRec expr;
	switch (NextToken())
	{
	case COMMA:
		Match(COMMA);
		Expression(expr);
		code.WriteExpr(expr);
		
		ItemListTail();
		break;
	case RPAREN:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::ItemList()
{
	ExprRec expr;
	Expression(expr);

	code.WriteExpr(expr);
	ItemListTail();
}

void Parser::VariableTail()
{
	ExprRec expr;
	switch (NextToken())
	{
	case LSTAPLE:
		Match(LSTAPLE);
		Expression(expr);
		Match(RSTAPLE);
		break;
	case AND_SYM:
	case NOT_SYM:
	case RSTAPLE:
	case RPAREN:
	case SEMICOLON:
	case COMMA:
	case ASSIGN_OP:
	case ADD_OP:
	case SUB_OP:
	case MUL_OP:
	case DIV_OP:
	case LT_OP:
	case LE_OP:
	case GT_OP:
	case GE_OP:
	case EQ_OP:
	case NE_OP:
		break;
	default:
		
		SyntaxError(NextToken(), "");
	}
}

void Parser::VarListTail()
{
	ExprRec expr;
	switch (NextToken())
	{
	case COMMA:
		Match(COMMA);
		Variable(expr);
		// code.ReadValue();
		cout << "Read value\n";
		VarListTail();
		break;
	case RPAREN:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::VarList()
{
	ExprRec expr;
	Variable(expr);
	code.ReadValue(expr);
	cout << "Read Value\n";
	VarListTail();
}

void Parser::Expression(ExprRec& result)//
{
	ExprRec leftOperand, rightOperand;
	OpRec op;

	Primary(result);
	for (;;){
		if(NextToken() == ADD_OP || NextToken()== SUB_OP){
			leftOperand.kind = result.kind;
			leftOperand.val = result.val;
			leftOperand.name = result.name;
			AddOp(op);
			Primary(rightOperand);
			code.GenInfix(leftOperand, op, rightOperand, result);
		}else{
			return;
		}
	}
	Factor();
	ExprTail();
}

void Parser::Variable(ExprRec& expr)
{
	Match(ID);
	VariableTail();
}

void Parser::FoutlnStmt()
{
	Match(FOUTLN_SYM);
	Match(LPAREN);
	code.NewLine();
	//cout << "fout new line\n";
	ItemList();
	Match(RPAREN);
	Match(SEMICOLON);
}

void Parser::FoutStmt()
{
	Match(FOUT_SYM);
	Match(LPAREN);
	ItemList();
	Match(RPAREN);
	Match(SEMICOLON);
}

void Parser::FinStmt()
{
	Match(FIN_SYM);
	Match(LPAREN);
	VarList();
	Match(RPAREN);
	Match(SEMICOLON);
}

void Parser::AssignStmt(ExprRec& expr)
{
	ExprRec identifier;
	Variable(expr);
	Match(ASSIGN_OP);
	Expression(expr);
	code.Assign(identifier, expr);
	
	cout << "Assignment op\n";
	Match(SEMICOLON);
}

void Parser::Declaration(ExprRec& expr)
{
	Type(expr);
	Match(ID);
	DecTail();
	code.ProcessId(expr);
	cout << "Processed ID\n";
	Match(SEMICOLON);
}

void Parser::StructStmt()
{
	switch (NextToken())
	{
	case FIF_SYM:
		FifStmt();
		break;
	case WHILE_SYM:
		WhileStmt();
		break;
	case DO_SYM:
		DoFwhileStmt();
		break;
	case FOR_SYM:
		ForStmt();
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::SimpleStmt(ExprRec& expr)
{
	switch (NextToken())
	{
	case ID:
		AssignStmt(expr);
		break;
	case FIN_SYM:
		FinStmt();
		break;
	case FOUT_SYM:
		FoutStmt();
		break;
	case FOUTLN_SYM:
		FoutlnStmt();
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::StmtTail(ExprRec& expr)
{
	switch (NextToken())
	{
	case BOOL_SYM:
	case DO_SYM:
	case FAKE_SYM:
	case FIF_SYM:
	case FIN_SYM:
	case FOR_SYM:
	case FOUT_SYM:
	case FOUTLN_SYM:
	case INT_SYM:
	case STR_SYM:
	case WHILE_SYM:
	case ID:
		Statement(expr);
		StmtTail(expr);
		break;
	case ENDFOR_SYM:
	case ENDWHILE_SYM:
	case END_SYM:
	case FELSE_SYM:
	case FENDIF_SYM:
	case FWHILE_SYM:
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::Statement(ExprRec& expr)
{
	cout << "statement\n";
	switch (NextToken())
	{
	case FIN_SYM:
	case FOUT_SYM:
	case FOUTLN_SYM:
	case ID:
		SimpleStmt(expr);
		break;
	case DO_SYM:
	case FIF_SYM:
	case FOR_SYM:
	case WHILE_SYM:
		StructStmt();
		break;
	case BOOL_SYM:
	case FAKE_SYM:
	case INT_SYM:
	case STR_SYM:
		Declaration(expr);//needs expr
		break;
	default:
		SyntaxError(NextToken(), "");
	}
}

void Parser::StmtList(ExprRec& expr)
{
	Statement(expr);
	StmtTail(expr);
}

void Parser::Program()
{
	
	code.Start();
	cout << "Started\n";
	Match(BEGIN_SYM);
	ExprRec expr;
	StmtList(expr);
	Match(END_SYM);
	
}

void Parser::SystemGoal()
{
	Program();
	Match(EOF_SYM);
	code.Finish();
}
