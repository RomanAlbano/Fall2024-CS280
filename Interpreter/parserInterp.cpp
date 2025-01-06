/* Implementation of Recursive-Descent Parser
 * for Mini C-Like Language
 * Programming Assignment 2
 * Fall 2024
 */

#include "parserInterp.h"
using namespace std;
map<string, bool> defVar;
map<string, Token> SymTable;
map<string, Value> TempsResults; // Container of temporary locations of Value objects for results of expressions, variables values and constants
queue<Value> *ValQue;			 // declare a pointer variable to a queue of Value objects

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

// Program is: Prog = PROGRAM IDENT CompStmt
bool Prog(istream &in, int &line)
{
	bool sl = false;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == PROGRAM)
	{
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT)
		{
			sl = CompStmt(in, line);
			if (!sl)
			{
				ParseError(line, "Invalid Program");
				return false;
			}
			cout << "(DONE)" << endl;
			return true;
		}
		else
		{
			ParseError(line, "Missing Program name");
			return false;
		}
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	else
	{
		ParseError(line, "Missing Program keyword");
		return false;
	}
} // End of Prog

// StmtList ::= Stmt { Stmt }
bool StmtList(istream &in, int &line)
{
	bool status;
	LexItem tok;

	status = Stmt(in, line);
	tok = Parser::GetNextToken(in, line);
	while (status && tok != RBRACE)
	{
		Parser::PushBackToken(tok);

		status = Stmt(in, line);
		tok = Parser::GetNextToken(in, line);
	}
	// if(!status)
	// {
	// 	ParseError(line, "Syntactic error in statement list.");

	// 	return false;
	// }
	Parser::PushBackToken(tok);
	return true;
} // End of StmtList

// DeclStmt ::= ( INT | FLOAT | BOOL | CHAR | STRING) VarList
bool DeclStmt(istream &in, int &line)
{
	bool status = false;
	LexItem tok;

	LexItem t = Parser::GetNextToken(in, line);

	if (t == INT || t == FLOAT || t == CHAR || t == BOOL || t == STRING)
	{

		status = VarList(in, line, t);

		if (!status)
		{
			ParseError(line, "Incorrect variable in Declaration Statement.");
			return status;
		}
		return true;
	}

	return true;
} // End of Decl

// Stmt ::= DeclStmt ; | ControlStmt ; | CompStmt
bool Stmt(istream &in, int &line)
{
	bool status = true;

	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken())
	{

	case PRINT:
	case IF:
	case IDENT:
		Parser::PushBackToken(t);
		status = ControlStmt(in, line);

		if (!status)
		{
			ParseError(line, "Invalid control statement.");
			return false;
		}

		break;
	case INT:
	case FLOAT:
	case CHAR:
	case BOOL:
	case STRING:
		Parser::PushBackToken(t);
		status = DeclStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid declaration statement.");
			return false;
		}
		t = Parser::GetNextToken(in, line);
		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}
		break;
	case LBRACE:
		Parser::PushBackToken(t);
		status = CompStmt(in, line);
		if (!status)
		{
			ParseError(line, "Invalid compound statement.");
			return false;
		}

		break;
	default:
		Parser::PushBackToken(t);

		return false;
	}

	return status;
} // End of Stmt

bool CompStmt(istream &in, int &line)
{
	bool status = false;

	LexItem t = Parser::GetNextToken(in, line);
	if (t == LBRACE)
	{
		status = StmtList(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect statement list");
			return false;
		}
		LexItem t = Parser::GetNextToken(in, line);
		if (t == RBRACE)
		{
			return true;
		}
		else if (t == DONE)
		{
			ParseError(line, "Missing end of program right brace.");
			return false;
		}
		else
		{
			ParseError(line, "Missing right brace.");
			return false;
		}
	}
	return status;
} // End of CompStmt

bool ControlStmt(istream &in, int &line)
{
	bool status;

	LexItem t = Parser::GetNextToken(in, line);

	switch (t.GetToken())
	{

	case PRINT:
		status = PrintStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect Print Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);

		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		break;

	case IDENT:
		Parser::PushBackToken(t);
		status = AssignStmt(in, line);
		if (!status)
		{
			ParseError(line, "Incorrect Assignment Statement");
			return false;
		}
		t = Parser::GetNextToken(in, line);

		if (t == SEMICOL)
		{
			return true;
		}
		else
		{
			ParseError(line, "Missing semicolon at end of Statement.");
			return false;
		}

		break;
	case IF:
		status = IfStmt(in, line);

		if (!status)
		{
			ParseError(line, "Incorrect IF Statement");
			return false;
		}

		break;

	default:
		Parser::PushBackToken(t);
		return true;
	}

	return status;
} // End of ControlStmt

// VarList ::= Var [= Expr] {, Var [= Expr]}
bool VarList(istream &in, int &line, LexItem &idtok)
{
	bool status = false, exprstatus = false;
	string identstr;

	LexItem tok = Parser::GetNextToken(in, line);
	if (tok == IDENT)
	{

		identstr = tok.GetLexeme();
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
			SymTable.insert({tok.GetLexeme(), idtok.GetToken()});
			// cout<<tok.GetLexeme() << " Token: " << idtok.GetToken();
		}
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
	}
	else
	{

		ParseError(line, "Missing Variable Name");
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok == ASSOP)
	{
		Value tempVal;
		exprstatus = Expr(in, line, tempVal);
		if (!exprstatus)
		{
			ParseError(line, "Incorrect initialization for a variable.");
			return false;
		}
		TempsResults.insert({identstr, tempVal});
		// cout<< "Initialization of the variable " << identstr <<" in the declaration statement at line " << line << endl;
		tok = Parser::GetNextToken(in, line);

		if (tok == COMMA)
		{

			status = VarList(in, line, idtok);
		}
		else
		{
			Parser::PushBackToken(tok);
			return true;
		}
	}
	else if (tok == COMMA)
	{

		status = VarList(in, line, idtok);
	}
	else if (tok == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");

		return false;
	}
	else
	{

		Parser::PushBackToken(tok);
		return true;
	}

	return status;

} // End of VarList

// PrintStmt:= PRINT (ExpreList)
// PrintStmt:= PRINT (ExpreList)
bool PrintStmt(istream &in, int &line)
{
	LexItem t;
	ValQue = new queue<Value>;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if (!ex)
	{
		ParseError(line, "Missing expression list after Print");
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		while (!(*ValQue).empty())
		{
			ValQue->pop();
		}
		delete ValQue;
		return false;
	}
	// Evaluate: print out the list of expressions' values
	while (!(*ValQue).empty())
	{
		Value nextVal = (*ValQue).front();
		cout << nextVal;
		ValQue->pop();
	}
	delete ValQue;
	cout << endl;
	return true;
} // End of PrintStmt

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
	bool status = false;
	Value retVal;

	status = Expr(in, line, retVal);
	if (!status)
	{
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(retVal);
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA)
	{

		status = ExprList(in, line);
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

// IfStmt ::= IF (Expr) Stmt [ ELSE Stmt ]
bool IfStmt(istream &in, int &line)
{
	bool ex = false, status;
	static int nestlevel = 0;
	LexItem t;

	t = Parser::GetNextToken(in, line);
	if (t != LPAREN)
	{

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}
	Value tempVal;
	ex = Expr(in, line, tempVal);
	if (!ex)
	{
		ParseError(line, "Missing if statement condition");
		return false;
	}
	if (tempVal.GetType() != VBOOL)
	{
		ParseError(line, "Invalid Type for If statement condition");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if (t != RPAREN)
	{

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	nestlevel += 1;
	// cout << "in IfStmt then-clause at nesting level: " << nestlevel << endl;
	//HEREEEE
	// if (tempVal.GetBool())
	// {
		status = Stmt(in, line);

		if (!status)
		{
			ParseError(line, "If-Stmt Syntax Error");
			return false;
		}
	// }

	t = Parser::GetNextToken(in, line);

	if (t == ELSE)
	{
		// if(!tempVal.GetBool()){
		// cout << "in IfStmt else-clause at nesting level: " << nestlevel << endl;
		status = Stmt(in, line);
		if (!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}

		nestlevel--;

		return true;
		// }
	}

	nestlevel--;
	Parser::PushBackToken(t);
	return true;
} // End of IfStmt function

// Var:= ident
bool Var(istream &in, int &line, LexItem &idtok)
{
	string identstr;
	// identifier token
	LexItem tok = Parser::GetNextToken(in, line);
	idtok = tok;
	if (tok == IDENT)
	{
		identstr = tok.GetLexeme();

		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		return true;
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
} // End of Var

// void TokenToVal(LexItem t, Value& tempVal){
// 	ValType result;
// 	switch(t.GetToken()){
// 		case STRING:
// 			result= VSTRING;
// 			tempVal.SetString(t.GetLexeme());
// 			break;
// 		case FLOAT:
// 			result= VREAL;
// 			tempVal.SetReal(stod(t.GetLexeme()));
// 			break;
// 		case BOOL:
// 			result= VBOOL;
// 			tempVal.SetBool(t.GetLexeme() == "true");
// 			break;
// 		case INT:
// 			result= VINT;
// 			tempVal.SetInt(stoi(t.GetLexeme()));
// 			break;
// 		case CHAR:
// 			result= VCHAR;
// 			tempVal.SetChar(t.GetLexeme()[0]);
// 			break;
// 		default:
// 			result= VERR;
// 	}
// 	tempVal.SetType(result);
// }
ValType TokenToVal(Token t)
{
	ValType result;
	switch (t)
	{
	case STRING:
		result = VSTRING;
		break;
	case FLOAT:
		result = VREAL;
		break;
	case BOOL:
		result = VBOOL;
		break;
	case INT:
		result = VINT;
		break;
	case CHAR:
		result = VCHAR;
		break;
	default:
		result = VERR;
	}
	return result;
}
// AssgnStmt ::= Var ( = | += | -= | *= | /= | %= ) Expr
bool AssignStmt(istream &in, int &line)
{

	bool varstatus = false, status = false;
	LexItem t;
	// Gets the Identifier's token
	LexItem previousToken;
	varstatus = Var(in, line, previousToken); // CHECK
	if (varstatus)
	{
		t = Parser::GetNextToken(in, line);

		if (t == ASSOP || t == ADDASSOP || t == SUBASSOP || t == MULASSOP || t == DIVASSOP || t == REMASSOP)
		{
			// TempVal is the variable
			Value tempVal;

			status = Expr(in, line, tempVal);
			if (t == ASSOP)
			{
				// MIGHT NOT WORK FOR REAL = INT ASSIGNMENTS
				if (TokenToVal(SymTable[previousToken.GetLexeme()]) != tempVal.GetType())
				{
					ParseError(line, "AssignStmt Error");
					return false;
				}
			}
			if (tempVal.GetType() == VERR)
			{
				ParseError(line, "AssignStmt Error");
				return false;
			}
			if (t == ADDASSOP || t == SUBASSOP || t == MULASSOP || t == DIVASSOP || t == REMASSOP)
			{
				if (TempsResults.find(previousToken.GetLexeme()) == TempsResults.end())
				{
					ParseError(line, "Using Unassigned Variable");
					return false;
				}
				if (t == ADDASSOP)
				{
					TempsResults[previousToken.GetLexeme()] = TempsResults[previousToken.GetLexeme()].operator+(tempVal);
				}
				if (t == SUBASSOP)
				{
					TempsResults[previousToken.GetLexeme()] = TempsResults[previousToken.GetLexeme()].operator-(tempVal);
				}
				if (t == MULASSOP)
				{
					TempsResults[previousToken.GetLexeme()] = TempsResults[previousToken.GetLexeme()].operator*(tempVal);
				}
				if (t == DIVASSOP)
				{
					TempsResults[previousToken.GetLexeme()] = TempsResults[previousToken.GetLexeme()].operator/(tempVal);
				}
				if (t == REMASSOP)
				{
					TempsResults[previousToken.GetLexeme()] = TempsResults[previousToken.GetLexeme()].operator%(tempVal);
					if (TempsResults[previousToken.GetLexeme()].GetType() == VERR)
					{
						// Value newVal;
						// newVal.SetType(VINT);
						// newVal.SetInt(3);
						// TempsResults[previousToken.GetLexeme()] = newVal;
						ParseError(line, "HERE");
					}
				}
			}

			// Adds the resultant assignment expression and identifier to TempsResult for use in UnaryExpr
			if (tempVal.GetType() == VINT && SymTable[previousToken.GetLexeme()] == FLOAT)
			{
				Value newVal;
				newVal.SetType(VREAL);
				newVal.SetReal(tempVal.GetInt());
				TempsResults.insert({previousToken.GetLexeme(), newVal});
			}
			else
			{
				TempsResults.insert({previousToken.GetLexeme(), tempVal});
			}
			if (!status)
			{
				ParseError(line, "Missing Expression in Assignment Statement");
				return status;
			}
		}
		else if (t.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << t.GetLexeme() << ")" << endl;
			return false;
		}
		else
		{
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else
	{
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
} // End of AssignStmt

// Expr ::= LogANDExpr { || LogANDRxpr }
bool Expr(istream &in, int &line, Value &retVal)
{

	LexItem tok;
	bool t1 = LogANDExpr(in, line, retVal);
	Value previousRetVal = retVal;
	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == OR)
	{
		t1 = LogANDExpr(in, line, retVal);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == OR)
		{
			retVal = previousRetVal.operator||(retVal);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "Expr Error");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
} // End of Expr/LogORExpr

// LogANDExpr ::= EqualExpr { && EqualExpr }
bool LogANDExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	bool t1 = EqualExpr(in, line, retVal);
	Value previousRetVal = retVal;
	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == AND)
	{
		t1 = EqualExpr(in, line, retVal);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == AND)
		{
			retVal = previousRetVal.operator&&(retVal);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "LogANDExpr Error");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}

	Parser::PushBackToken(tok);
	return true;
} // End of LogANDExpr

// EqualExpr ::= RelExpr [ (== | !=) RelExpr ]
bool EqualExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	bool t1 = RelExpr(in, line, retVal);
	Value previousRetVal = retVal;
	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == EQ || tok == NEQ)
	{
		t1 = RelExpr(in, line, retVal);

		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == EQ)
		{
			retVal = previousRetVal.operator==(retVal);
		}
		else if (tok == NEQ)
		{
			retVal = previousRetVal.operator!=(retVal);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "EqualExpr Error");
			return false;
		}
		tok = Parser::GetNextToken(in, line);
		if (tok == EQ || tok == NEQ)
		{
			ParseError(line, "Illegal Equality Expression.");
			return false;
		}
		else if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);

	return true;
} // End of EqualExpr

// RelExpr ::= AddExpr [ ( < | > ) AddExpr ]
bool RelExpr(istream &in, int &line, Value &retVal)
{
	LexItem tok;
	bool t1 = AddExpr(in, line, retVal);
	Value previousRetVal = retVal;
	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if (tok == LTHAN || tok == GTHAN)
	{
		t1 = AddExpr(in, line, retVal);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}

		if (tok == LTHAN)
		{
			retVal = previousRetVal.operator<(retVal);
		}
		else if (tok == GTHAN)
		{
			retVal = previousRetVal.operator>(retVal);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "RelExpr Error");
			return false;
		}

		tok = Parser::GetNextToken(in, line);

		if (tok == LTHAN || tok == GTHAN)
		{
			ParseError(line, "Illegal Relational Expression.");
			return false;
		}
		else if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
} // End of RelExpr

// AddExpr :: MultExpr { ( + | - ) MultExpr }
bool AddExpr(istream &in, int &line, Value &retVal)
{

	bool t1 = MultExpr(in, line, retVal);
	LexItem tok;
	Value previousRetVal = retVal;

	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == PLUS || tok == MINUS)
	{
		t1 = MultExpr(in, line, retVal);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == PLUS)
		{
			retVal = previousRetVal.operator+(retVal);
		}
		else if (tok == MINUS)
		{
			retVal = previousRetVal.operator-(retVal);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "AddExpr Error");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
} // End of AddExpr

// MultExpr ::= UnaryExpr { ( * | / | %) UnaryExpr }
bool MultExpr(istream &in, int &line, Value &retVal)
{

	bool t1 = UnaryExpr(in, line, retVal);
	LexItem tok;
	Value previousRetVal = retVal;
	if (!t1)
	{
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while (tok == MULT || tok == DIV || tok == REM)
	{
		t1 = UnaryExpr(in, line, retVal);
		if (!t1)
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
		if (tok == MULT)
		{
			retVal = previousRetVal.operator*(retVal);
		}
		else if (tok == DIV)
		{
			retVal = previousRetVal.operator/(retVal);
		}
		else if (tok == REM)
		{
			retVal = previousRetVal.operator%(retVal);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "MultExpr Error");
			return false;
		}

		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == ERR)
		{
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
} // End of MultExpr

// UnaryExpr ::= ( - | + | ! ) PrimaryExpr | PrimaryExpr
bool UnaryExpr(istream &in, int &line, Value &retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if (t == MINUS)
	{
		sign = -1;
	}
	else if (t == PLUS)
	{
		sign = 1;
	}
	else if (t == NOT)
	{
		sign = 2;
	}
	else
		Parser::PushBackToken(t);

	status = PrimaryExpr(in, line, sign, retVal);
	if (sign == 2)
	{
		retVal = retVal.operator!();
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "Illegal Operand Type for NOT Operator");
			return false;
		}
	}
	else if (sign == -1)
	{
		if (retVal.GetType() != VINT || retVal.GetType() != VREAL)
		{
			retVal.SetType(VERR);
		}
		if (retVal.GetType() == VERR)
		{
			ParseError(line, "Illegal Operand Type for NOT Operator");
			return false;
		}
	}

	return status;
} // End of UnaryExpr

// void TokenToVal2(LexItem t, Value& tempVal){
// 	ValType result;
// 	if (TempsResults.find(t.GetLexeme()) == TempsResults.end()) {
//     return;
// }
// 	Value current = TempsResults[t.GetLexeme()];
//     if(current.GetType() == VERR){
//         return;
//     }
// 	//t represents the identifier
// 	if (SymTable.find(t.GetLexeme()) == SymTable.end()) {
//     return;
// }
// 	switch(SymTable[t.GetLexeme()]){
// 		case STRING:
// 			result= VSTRING;
// 			tempVal.SetString(current.GetString());
// 			break;
// 		case FLOAT:
// 			result= VREAL;
// 			tempVal.SetReal(current.GetReal());
// 			break;
// 		case BOOL:
// 			result= VBOOL;
// 			tempVal.SetBool(current.GetBool());
// 			break;
// 		case INT:
// 			result= VINT;
// 			tempVal.SetInt(current.GetInt());
// 			break;
// 		case CHAR:
// 			result= VCHAR;
// 			tempVal.SetChar(current.GetChar());
// 			break;
// 		default:
// 			result= VERR;
// 	}
// 	tempVal.SetType(result);
// }

void TokenToVal2(LexItem t, Value &tempVal)
{
	ValType result;

	// Check if lexeme exists in TempsResults
	if (TempsResults.find(t.GetLexeme()) == TempsResults.end())
	{
		cout << "Lexeme not found: " << t.GetLexeme() << endl;
		return; // Handle error or return appropriately
	}
	Value current = TempsResults[t.GetLexeme()];

	// If the value is of type VERR (uninitialized or error), return
	if (current.GetType() == VERR)
	{
		return;
	}

	// Check if lexeme exists in SymTable
	if (SymTable.find(t.GetLexeme()) == SymTable.end())
	{
		cout << "Lexeme not found in SymTable: " << t.GetLexeme() << endl;
		return; // Handle error or return appropriately
	}

	// Handle lexeme type based on SymTable
	switch (SymTable[t.GetLexeme()])
	{
	case STRING:
		if (!current.IsString())
		{
			// cout << "Type mismatch for lexeme: " << t.GetLexeme() << endl;
			return;
		}
		result = VSTRING;
		tempVal.SetString(current.GetString());
		break;
	case FLOAT:
		if (!current.IsReal() && !current.IsInt())
		{
			// cout << "Type mismatch for lexeme: " << t.GetLexeme() << endl;
			return;
		}
		result = VREAL;
		if (current.IsInt())
		{
			tempVal.SetReal(current.GetInt());
		}
		else
		{
			tempVal.SetReal(current.GetReal());
		}
		break;
	case BOOL:
		if (!current.IsBool())
		{
			// cout << "Type mismatch for lexeme: " << t.GetLexeme() << endl;
			return;
		}
		result = VBOOL;
		tempVal.SetBool(current.GetBool());
		break;
	case INT:
		if (!current.IsInt())
		{
			// cout << "Type mismatch for lexeme: " << t.GetLexeme() << endl;
			return;
		}
		result = VINT;
		tempVal.SetInt(current.GetInt());
		break;
	case CHAR:
		if (!current.IsChar())
		{
			// cout << "Type mismatch for lexeme: " << t.GetLexeme() << endl;
			return;
		}
		result = VCHAR;
		tempVal.SetChar(current.GetChar());
		break;
	default:
		// cout << "Error: Unrecognized type for lexeme: " << t.GetLexeme() << endl;
		result = VERR;
		tempVal.SetType(result);
		return;
	}

	// Set the final type
	tempVal.SetType(result);
}

// PrimaryExpr ::= IDENT | ICONST | RCONST | SCONST | BCONST | CCONST | ( Expr )
bool PrimaryExpr(istream &in, int &line, int sign, Value &retVal)
{

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == IDENT)
	{

		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undeclared Variable");
			return false;
		}
		// retVal.SetType(TokenToVal(SymTable[tok.GetLexeme()]));
		if (TempsResults.find(lexeme) == TempsResults.end())
		{
			ParseError(line, "Using UnInitialized Variable");
			return false;
		}
		TokenToVal2(tok, retVal);
		// if(retVal.GetType() == VERR){
		// 	cout << "IDK HERE" << endl;
		// }
		return true;
	}
	else if (tok == ICONST)
	{
		retVal.SetType(VINT);
		retVal.SetInt(stoi(tok.GetLexeme()));
		return true;
	}
	else if (tok == SCONST)
	{
		retVal.SetType(VSTRING);
		retVal.SetString(tok.GetLexeme());
		return true;
	}
	else if (tok == RCONST)
	{
		retVal.SetType(VREAL);
		retVal.SetReal(stod(tok.GetLexeme()));
		return true;
	}
	else if (tok == BCONST)
	{
		retVal.SetType(VBOOL);
		retVal.SetBool(tok.GetLexeme() == "true" || tok.GetLexeme() == "True");
		return true;
	}
	else if (tok == CCONST)
	{
		retVal.SetType(VCHAR);
		retVal.SetChar(tok.GetLexeme()[0]);
		return true;
	}
	else if (tok == LPAREN)
	{
		bool ex = Expr(in, line, retVal);
		if (!ex)
		{
			ParseError(line, "Missing expression after Left Parenthesis");
			return false;
		}
		if (Parser::GetNextToken(in, line) == RPAREN)
			return ex;
		else
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing right Parenthesis after expression");
			return false;
		}
	}
	else if (tok.GetToken() == ERR)
	{
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
} // End of PrimaryExpr

// How do i get the value of t, t.GetLexeme() only returns the name of the identifier (i.e. name = 3)
