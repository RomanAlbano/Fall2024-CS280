/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
 * 
 * Roman Albano
 * CS 280-011
 * rba
 */

#include "parser.h"

// Represents the name of the variable, and whether or not its been initialized
map<string, bool> defVar;

//
map<string, Token> SymTable;
int thenNestingLevel = 0;
int elseNestingLevel = 0;
namespace Parser
{
	bool pushed_back = false;
	LexItem pushed_token;

	static LexItem GetNextToken(istream &in, int &line)
	{
		if (pushed_back)
		{
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem &t)
	{
		if (pushed_back)
		{
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
	return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

bool IdentList(istream &in, int &line);

// Prog ::= PROGRAM IDENT CompStmt
bool Prog(istream &in, int &line)
{
	LexItem t = Parser::GetNextToken(in, line);
	if (t != PROGRAM)
	{
		ParseError(line, "Missing PROGRAM Keyword"); 
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if (t != IDENT)
	{
		ParseError(line, "Missing Program Name"); 
		return false;
	}
	if (!CompStmt(in, line))
	{
		ParseError(line, "Invalid Program");
		return false;
	}
	cout << "(DONE)" << endl;
	return true;
}

// StmtList Incorrect
// StmtList ::= Stmt { Stmt }
bool StmtList(istream &in, int &line)
{
	if (!Stmt(in, line))
	{
		return false;
	}
	// Loops through each statement within LBRACE and RBRACE
	while (Stmt(in, line))
	{
		LexItem t = Parser::GetNextToken(in, line);
		Parser::PushBackToken(t);
		if (t == RBRACE)
		{
			return true;
		}
	}
	//ParseError(line, "Invalid statement list");
	return true;
}
// Stmt ::= DeclStmt | ControlStmt | CompStmt
bool Stmt(istream &in, int &line)
{
	LexItem t = Parser::GetNextToken(in, line);
	Parser::PushBackToken(t);
	//  If the next token is a If, Print, or Identification Token, go to ControlStmt

	if (t == IF || t == PRINT || t == IDENT)
	{
		if (ControlStmt(in, line)){
			return true;
		}
	}
	// If the next token is a variable type, go to Declaration Statement
	if (t == CHAR || t == STRING || t == INT || t == BOOL || t == FLOAT)
	{
		if(DeclStmt(in, line)){
			return true;
		}
	}
	// If the next token is a Left Brace, go to Computation Statement
	if (t == LBRACE)
	{
		if (CompStmt(in, line)){
			return true;
		}
	}
	if (t == RBRACE)
	{ // COULD ERROR OUT
		return true;
	}
	ParseError(line, "Syntactic error in statement list.");
	return false;
}
// DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING ) VarList ;
bool DeclStmt(istream &in, int &line)
{
	// Gets variable type
	LexItem t = Parser::GetNextToken(in, line);

	// I MIGHT NEED TO SET THE TYPE OF THE VARIABLE HERRRRRRRRRRRRRRRRRRRRRRREEEEEEEE
	//  Refer To Line 107 HERE
	if (!VarList(in, line))
	{
		ParseError(line, "Incorrect variable in Declaration Statement."); // MIGHT BE WRONG SPOT
		ParseError(line, "Invalid declaration statement.");				  // MIGHT BE WRONG SPOT
		return false;
	}
	t = Parser::GetNextToken(in, line);
	// If the line doesn't end in a Semicolon, an error is displayed
	if (t != SEMICOL)
	{
		ParseError(line, "Missing Semicolon at the end of Declaration Statement");
		return false;
	}
	return true;
}

bool VarList(istream &in, int &line)
{
	LexItem variableName = Parser::GetNextToken(in, line);
	Parser::PushBackToken(variableName);
	if (defVar.find(variableName.GetLexeme()) != defVar.end())
	{
		ParseError(line, "Variable Redefinition");
		return false;
	}
	defVar[(variableName.GetLexeme())] = false;
	if (!Var(in, line))
	{

		return false;
	}

	// gets the token AFTER the variable name
	LexItem t = Parser::GetNextToken(in, line);
	if (t == ASSOP)
	{

		if (defVar.find(variableName.GetLexeme()) == defVar.end())
		{
			ParseError(line, "Undeclared Variable: ");
			return false;
		}

		defVar[variableName.GetLexeme()] = true;
		cout << "Initialization of the variable " << variableName.GetLexeme() << " in the declaration statement at line " << line << endl;
		if (!Expr(in, line))
		{
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
	}

	while ((t = Parser::GetNextToken(in, line)) == COMMA)
	{
		variableName = Parser::GetNextToken(in, line);
		Parser::PushBackToken(variableName);
		if (defVar.find(variableName.GetLexeme()) != defVar.end())
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
		defVar[(variableName.GetLexeme())] = false;
		if (!Var(in, line))
		{
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if (t == ASSOP)
		{
			// Repetitive?
			if (defVar.find(variableName.GetLexeme()) == defVar.end())
			{
				ParseError(line, "Undeclared Variable: ");
				return false;
			}

			defVar[variableName.GetLexeme()] = true;
			cout << "Initialization of the variable " << variableName.GetLexeme() << " in the declaration statement at line " << line << endl;
			if (!Expr(in, line))
			{
				return false;
			}
		}
		else
		{
			Parser::PushBackToken(t);
		}
	}
	// returns the previous token that failed the "while" statement
	Parser::PushBackToken(t);
	return true;
}

// ControlStmt ::= AssgnStmt ; | IfStmt | PrintStmt ;
bool ControlStmt(istream &in, int &line)
{
	LexItem t = Parser::GetNextToken(in, line);
	Parser::PushBackToken(t);
	if (t == IDENT)
	{
		if (AssignStmt(in, line))
		{
			// cout<<"AssignStmt " << t.GetLexeme()<<endl;
			// t = Parser::GetNextToken(in, line);
			// if (t != SEMICOL)
			// {
			// 	ParseError(line, "Missing Semi Colon in Assignment");
			// 	return false;
			// }
			return true;
		} 
	}
	if (t == IF)
	{
		if(IfStmt(in, line)){
			return true;
		}
	}
	if (t == PRINT)
	{
		if(PrintStmt(in, line)){
			return true;
		}
	}
	ParseError(line, "Invalid control statement.");
	return false;
}

// CompStmt ::= '{' StmtList '}'
bool CompStmt(istream &in, int &line)
{
	LexItem t = Parser::GetNextToken(in, line);
	if (t != LBRACE)
	{

		return false;
	}

	if (StmtList(in, line))
	{
		t = Parser::GetNextToken(in, line);
		if (t != RBRACE)
		{
			ParseError(line, "Incorrect Statement list");
			return false;
		}
	}
	else
	{
		ParseError(line, "Incorrect statement list");
		return false;
	}

	return true;
}
// IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
bool IfStmt(istream &in, int &line)
{
	// Gets the IF token
	LexItem t = Parser::GetNextToken(in, line);

	// Gets Token after IF
	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{
		ParseError(line, "Missing Left Parenthesis in If Statement");
		return false;
	}
	if (!Expr(in, line))
	{
		ParseError(line, "Missing if statement condition");
		ParseError(line, "Incorrect IF Statement");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{
		cout << t.GetLexeme() << endl;
		ParseError(line, "Missing Right Parenthesis in If Statement");
		return false;
	}
	thenNestingLevel++;
	cout << "in IfStmt then-clause at nesting level: " << thenNestingLevel << endl;
	if (!Stmt(in, line))
	{
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t == ELSE)
	{
		cout << "in IfStmt else-clause at nesting level: " << thenNestingLevel << endl;
		if (!Stmt(in, line))
		{
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
	}
	thenNestingLevel--;
	return true;
}
// AssignStmt := Var = Expr;

// EXAMPLE:
// bool nice;
// nice = true;
bool AssignStmt(istream &in, int &line)
{
	int currentLine = line;
	LexItem variableName = Parser::GetNextToken(in, line);
	Parser::PushBackToken(variableName);
	if (!Var(in, line))
	{
		ParseError(currentLine, "Invalid Variable in Assignment Statement");
		return false;
	}

	if (defVar.find(variableName.GetLexeme()) == defVar.end())
	{
		ParseError(currentLine, "Undeclared Variable");
		ParseError(currentLine, "Missing Left-Hand Side Variable in Assignment statement");
		ParseError(currentLine, "Incorrect Assignment Statement");
		return false;
	}

	defVar[variableName.GetLexeme()] = true;
	// cout << "Initialization of the variable " << variableName.GetLexeme() << " in the declaration statement at line " << line << endl;

	LexItem t = Parser::GetNextToken(in, line);
	if (t != ASSOP)
	{
		ParseError(line, "Missing Equal Sign in Assignment Statement");
		ParseError(currentLine, "Incorrect Assignment Statement");
		return false;
	}
	if (!Expr(in, line))
	{
		ParseError(line, "Missing Expression in Assignment Statement");
		
			ParseError(line, "Incorrect Assignment Statement");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if (t != SEMICOL)
	{
		// Parser::PushBackToken(t); // HERHEHREE
		line--;
		ParseError(line, "Missing semicolon at end of Statement.");
		return false;
	}
	return true;
}

// Var := IDENT
bool Var(istream &in, int &line)
{
	LexItem t = Parser::GetNextToken(in, line);
	if (t != IDENT)
	{
		ParseError(line, "Expected Identifier");
		return false;
	}

	return true;
}

// Expr := LogANDExpr {|| LogANDExpr}
bool Expr(istream &in, int &line)
{

	if (!LogANDExpr(in, line))
	{
		// DOES THERE NEED TO BE A PARSEERROR DISPLAY HERE
		return false;
	}

	LexItem t = Parser::GetNextToken(in, line);

	while (t == OR)
	{
		if (!LogANDExpr(in, line))
		{
			ParseError(line, "Invalid Logical AND Expression in Expression");
			return false;
		}
		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);
	return true;
}

// LogANDExpr := EqualExpr {&& EqualExpr}
bool LogANDExpr(istream &in, int &line)
{
	if (!EqualExpr(in, line))
	{
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	while (t == AND)
	{
		if (!EqualExpr(in, line))
		{
			ParseError(line, "Invalid Equality Expression in Logical AND Expression");
			return false;
		}
		t = Parser::GetNextToken(in, line);
	}

	Parser::PushBackToken(t);
	return true;
}

// EqualExpr := RelExpr {== RelExpr}
bool EqualExpr(istream &in, int &line)
{
	if (!RelExpr(in, line))
	{
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	while (t == EQ || t == NEQ)
	{
		if (!RelExpr(in, line))
		{
			// ParseError(line, "Invalid Equality Expression in Equality Expression");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if(t==EQ || t==NEQ){
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
	}
	Parser::PushBackToken(t);
	return true;
}

// RelExpr := AddExpr {<|> AddExpr}
//>= and <= Operator Omitted Since there arent any tokens for specified cases
bool RelExpr(istream &in, int &line)
{
	if (!AddExpr(in, line))
	{
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	while (t == LTHAN || t == GTHAN)
	{
		if (!AddExpr(in, line))
		{
			// ParseError(line, "Invalid Addition Expression in Relational Expression");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if(t==LTHAN || t==GTHAN){
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
	}
	Parser::PushBackToken(t);
	return true;
}

// AddExpr := MultExpr {+|- MultExpr}
bool AddExpr(istream &in, int &line)
{
	if (!MultExpr(in, line))
	{
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	while (t == PLUS || t == MINUS)
	{
		if (!MultExpr(in, line))
		{
			// ParseError(line, "Invalid Multiplication Expression in Addition Expression");
			return false;
		}
		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);
	return true;
}
// MultExpr := UnaryExpr {*|/ UnaryExpr}
bool MultExpr(istream &in, int &line)
{
	if (!UnaryExpr(in, line))
	{
		return false;
	}
	LexItem t = Parser::GetNextToken(in, line);
	while (t == MULT || t == DIV)
	{
		if (!UnaryExpr(in, line))
		{
			ParseError(line, "Missing Operand after Operator");
			return false;
		}
		t = Parser::GetNextToken(in, line);
	}
	Parser::PushBackToken(t);
	return true;
}

// UnaryExpr := - PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream &in, int &line)
{
	int isNegative = 1;
	LexItem t = Parser::GetNextToken(in, line);
	if (t == MINUS)
	{
		isNegative = -1;
	}
	else
	{
		// if the next token is NOT negative, we put it back into the stream for it be run as a PrimaryExpr
		Parser::PushBackToken(t);
	}
	return PrimaryExpr(in, line, isNegative);
}

// PrimaryExpr := IDENT | ICONST | RCONST | SCONST | BCONST | ( Expr )
bool PrimaryExpr(istream &in, int &line, int sign)
{
	LexItem t = Parser::GetNextToken(in, line);
	if (t == ICONST || t == RCONST || t == SCONST || t == BCONST)
	{
		return true;
	}
	if (t == IDENT)
	{
		if (defVar.find(t.GetLexeme()) == defVar.end())
		{
			ParseError(line, "Undeclared Variable: ");
			return false;
		}

		// Check if the variable is initialized
		// if (!defVar[t.GetLexeme()])
		// {
		// 	// ParseError(line, "Uninitialized Variable: ");
		// 	return false;
		// }
		return true;
	}
	if (t == LPAREN)
	{
		if (!Expr(in, line))
		{
			ParseError(line, "Invalid Expression in Primary Expression");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if (t != RPAREN)
		{
			ParseError(line, "Missing Right Parenthesis");
			return false;
		}
		return true;
	}

	// ParseError(line, "Unrecognized Primary Expression");
	return false;
}

// PrintStmt:= PRINT (ExpreList)
bool PrintStmt(istream &in, int &line)
{
	LexItem t;
	// cout << "in PrintStmt" << endl;
	// Gets the Print token
	t = Parser::GetNextToken(in, line);
	// Gets the token after Print
	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{

		ParseError(line, "Missing Left Parenthesis");
		ParseError(line, "Incorrect Print Statement");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex)
	{
		ParseError(line, "Missing expression list after Print");
		ParseError(line, "Incorrect Print Statement");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		ParseError(line, "Incorrect Print Statement");
		return false;
	}
	// Evaluate: print out the list of expressions values
	t = Parser::GetNextToken(in, line);
	// If the line doesn't end in a Semicolon, an error is displayed
	if (t != SEMICOL)
	{
		ParseError(line, "Missing Semicolon at the end of Print Statement");
		ParseError(line, "Incorrect Print Statement");
		return false;
	}

	return true;
} // End of PrintStmt

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
	bool status = false;
	// cout << "in ExprList and before calling Expr" << endl;
	status = Expr(in, line);
	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA)
	{
		// cout << "before calling ExprList" << endl;
		status = ExprList(in, line);
		// cout << "after calling ExprList" << endl;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
} // End of ExprList
