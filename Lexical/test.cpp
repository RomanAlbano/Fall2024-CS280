#include "lex.h"

LexItem getNextToken (istream& in, int& linenumber){
    enum TokState {
        START, INID, ININT,
        

    }
    lexstate = START;
    string lexeme;
    char ch;
    while(in.get(ch)) {
        switch(lexstate){
            case START:
            break;
            case INID:
            break;
            case ININT:
            break;
            

        }
    }
}
//Roman Albano
//rba
//CS 241-011
#include "lex.h"

LexItem id_or_kw(const string &lexeme, int linenum)
{
    map<string, Token> StrToTok = {
        {"PROGRAM", PROGRAM},
        {"PRINT", PRINT},
        {"IF", IF},
        {"ELSE", ELSE},
        {"IDENT", IDENT},
        {"BOOL", BOOL},
        {"STRING", STRING},
        {"INT", INT},
        {"FLOAT", FLOAT},
        {"CHAR", CHAR},
        {"ICONST", ICONST},
        {"RCONST", RCONST},
        {"SCONST", SCONST},
        {"BCONST", BCONST},
        {"CCONST", CCONST},

        {"PLUS", PLUS},
        {"MINUS", MINUS},
        {"MULT", MULT},
        {"DIV", DIV},
        {"ASSOP", ASSOP},
        {"EQ", EQ},
        {"NEQ", NEQ},
        {"GTHAN", GTHAN},
        {"LTHAN", LTHAN},
        {"AND", AND},
        {"OR", OR},
        {"NOT", NOT},
        {"COMMA", COMMA},
        {"SEMICOL", SEMICOL},
        {"LPAREN", LPAREN},
        {"RPAREN", RPAREN},
        {"LBRACE", LBRACE},
        {"RBRACE", RBRACE},
        {"REM", REM},
        {"ADDASSOP", ADDASSOP},
        {"SUBASSOP", SUBASSOP},
        {"MULASSOP", MULASSOP},
        {"DIVASSOP", DIVASSOP},
        {"REMASSOP", REMASSOP},
        {"DOT", DOT},
        {"ERR", ERR},

        {"DONE", DONE},
    };
    LexItem object;
    if (lexeme == "TRUE" || lexeme == "FALSE")
    {
        object = LexItem(BCONST, lexeme, linenum);
    }
    else if (StrToTok.find(lexeme) != StrToTok.end())
    {
        object = LexItem(StrToTok[lexeme], lexeme, linenum);
    }
    else
    {
        object = LexItem(IDENT, lexeme, linenum);
    }
    
    return object;
}

ostream& operator<<(ostream& out, const LexItem& tok){
    switch (tok.GetToken()){
        case IDENT: 
            cout << "IDENT: <" << tok.GetLexeme() << ">";
            break;
        case ICONST:
            cout << "ICONST: (" << tok.GetLexeme() << ")";
            break;
        case RCONST:
            cout << "RCONST: (" << tok.GetLexeme() << ")";
            break;
        case BCONST: 
            cout << "BCONST: (" << tok.GetLexeme() << ")";
            break;
        case PROGRAM:
            out << "PROGRAM: KEYWORD";
            break;
        case PRINT:
            out << "PRINT: KEYWORD";
            break;
        case IF:
            out << "IF: KEYWORD";
            break;
        case ELSE:
            out << "ELSE: KEYWORD";
            break;
        case BOOL:
            out << "BOOL: KEYWORD";
            break;
        case STRING:
            out << "STRING: KEYWORD";
            break;
        case INT:
            out << "INT: KEYWORD";
            break;
        case FLOAT:
            out << "FLOAT: KEYWORD";
            break;
        case CHAR:
            out << "CHAR: KEYWORD";
            break;
        case SCONST:
            out << "SCONST: \"" << tok.GetLexeme() << "\"";
            break;
        case CCONST:
            out << "CCONST: '" << tok.GetLexeme() << "'";
            break;
        case PLUS:
            out << "PLUS: [" << tok.GetLexeme() << "]";
            break;
        case MINUS:
            out << "MINUS: [" << tok.GetLexeme() << "]";
            break;
        case MULT:
            out << "MULT: [" << tok.GetLexeme() << "]";
            break;
        case DIV:
            out << "DIV: [" << tok.GetLexeme() << "]";
            break;
        case ASSOP:
            out << "ASSOP: [" << tok.GetLexeme() << "]";
            break;
        case EQ:
            out << "EQ: [" << tok.GetLexeme() << "]";
            break;
        case NEQ:
            out << "NEQ: [" << tok.GetLexeme() << "]";
            break;
        case GTHAN:
            out << "GTHAN: [" << tok.GetLexeme() << "]";
            break;
        case LTHAN:
            out << "LTHAN: [" << tok.GetLexeme() << "]";
            break;
        case AND:
            out << "AND: [" << tok.GetLexeme() << "]";
            break;
        case OR:
            out << "OR: [" << tok.GetLexeme() << "]";
            break;
        case NOT:
            out << "NOT: [" << tok.GetLexeme() << "]";
            break;
        case ADDASSOP:
            out << "ADDASSOP: [" << tok.GetLexeme() << "]";
            break;
        case SUBASSOP:
            out << "SUBASSOP: [" << tok.GetLexeme() << "]";
            break;
        case MULASSOP:
            out << "MULASSOP: [" << tok.GetLexeme() << "]";
            break;
        case DIVASSOP:
            out << "DIVASSOP: [" << tok.GetLexeme() << "]";
            break;
        case REMASSOP:
            out << "REMASSOP: [" << tok.GetLexeme() << "]";
            break;
        case COMMA:
        out << "COMMA: [" << tok.GetLexeme() << "]";
            break;
        case SEMICOL:
        out << "SEMICOL: [" << tok.GetLexeme() << "]";
            break;
        case LPAREN:
        out << "LPAREN: [" << tok.GetLexeme() << "]";
            break;
        case RPAREN:
        out << "RPAREN: [" << tok.GetLexeme() << "]";
            break;
        case LBRACE:
        out << "LBRACE: [" << tok.GetLexeme() << "]";
            break;
        case RBRACE:
        out << "RBRACE: [" << tok.GetLexeme() << "]";
            break;
        case DOT:
        out << "DOT: [" << tok.GetLexeme() << "]";
            break;
        case REM:
            out << "REM: [" << tok.GetLexeme() << "]";
            break;
        case ERR:
            out << "ERR: {" << tok.GetLexeme() << "} error at line no. " << tok.GetLinenum();
            break;
        case DONE:
            out << "DONE: " << tok.GetLexeme();
            break;
        default:
            out << "UNKNOWN TOKEN: " << tok.GetLexeme();
            break;
    }
    return out;
}
     
