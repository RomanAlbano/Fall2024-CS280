// Roman Albano
// rba
// CS 280-011

#include <cctype>
#include "lex.h"
#include <iostream>
#include <regex>

LexItem getNextToken(istream &in, int &linenumber)
{
    enum TokState
    {
        START_STATE,
        IDENT_STATE,
        COMMENT_STATE_INCOMPLETE /* For the second slash in "//"*/,
        COMMENT_STATE_INCOMPLETE2, // For /* */
        COMMENT_STATE_INCOMPLETE3,
        COMMENT_STATE,
        SCONST_STATE,
        ICONST_STATE,
        RCONST_STATE,
        CCONST_STATE1,
        CCONST_STATE2,
        RESERVED_WORDS_STATE,
        ERROR_STATE,
        DONE_STATE,
        DONE_WITH_FILE_STATE
    } lexstate = START_STATE;
    string lexeme;
    char ch;
    Token type;
    // bool newLineTriggered = false;
    while (in.get(ch))
    {
        // if (in.peek() == ' ' && ch == '.' && lexstate == ICONST_STATE){
        //     in.putback(ch);
        //     lexstate = DONE_STATE;
        //     type = ICONST;
        //     break;
        // }
        if (ch == '\n')
        {
            if(lexstate==CCONST_STATE1 || lexstate == CCONST_STATE2){
                lexstate = ERROR_STATE;
                type = ERR;
                lexeme = "New line is an invalid character constant.";
                break;
            }
            linenumber++;
        }
        if(ch == ' ' && lexstate == START_STATE){
            continue;
        }
        if (lexeme == " ")
        {
            lexeme = "";
            lexstate = START_STATE;
        }
        
        if ((isspace(ch))  && lexeme != "" && lexstate != CCONST_STATE1 && lexstate!= CCONST_STATE2 && lexstate != COMMENT_STATE && lexstate != SCONST_STATE && lexstate != START_STATE && lexstate != COMMENT_STATE_INCOMPLETE2 && lexstate != COMMENT_STATE_INCOMPLETE3)
        {
            if (!(lexstate == ICONST_STATE && ch == '.'))
            {
                break;
            }
        }
        if (ch == '\n')
        {
            if (lexstate == COMMENT_STATE_INCOMPLETE)
            {
                lexstate = ERROR_STATE;
                type = ERR;
            }
            else if (lexstate == CCONST_STATE1 || lexstate == CCONST_STATE2){
                lexstate = ERROR_STATE;
                type = ERR;
                lexeme = "New line is an invalid character constant.";
            }
            else if (lexeme == "" || lexstate == START_STATE ||lexstate == COMMENT_STATE_INCOMPLETE2 || lexstate == COMMENT_STATE_INCOMPLETE3 || lexstate == COMMENT_STATE || lexstate == SCONST_STATE)
            {
                if (lexstate == COMMENT_STATE)
                {
                    lexeme = "";
                    lexstate = START_STATE;
                }
                else if (lexstate == COMMENT_STATE_INCOMPLETE3)
                {
                    lexstate = COMMENT_STATE_INCOMPLETE2;
                }

                // continue;
            }
            else
            {
                break;
            }
        }
        // lexeme += ch;
        // if (ch == ERR)
        // {
        //     type = ERR;
        //     break;
        // }
        if (ch == '?'){
            lexstate = ERROR_STATE;
            type = ERR;
        }
        lexeme += ch;
        if (ch == '\n' && lexstate!=SCONST_STATE && lexstate != CCONST_STATE1 && lexstate != CCONST_STATE2)
        {
            lexeme = "";
        }
        switch (lexstate)
        {
        case START_STATE:
            if (isalpha(ch) || ch == '_')
            {
                lexstate = IDENT_STATE;
                type = IDENT;
            }
            else if (isdigit(ch))
            {
                lexstate = ICONST_STATE;
                type = ICONST;
            }
            else if (ch=='\''){
                lexstate = CCONST_STATE1;
                type = CCONST;
            }
            else if (ch == '.' && isdigit(in.peek()))
            {
                lexstate = RCONST_STATE;
                type = RCONST;
            }

            //|| ch == ',' || ch == ';' || ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '.')
            else if (ch ==','){
                lexstate =DONE_STATE;
                type = COMMA;
            }
            else if (ch == ';'){
                lexstate = DONE_STATE;
                type = SEMICOL;
            }
            else if (ch == '('){
                lexstate = DONE_STATE;
                type = LPAREN;
            }
            else if (ch==')'){
                lexstate = DONE_STATE;
                type = RPAREN;
            }
            else if (ch == '{'){
                lexstate = DONE_STATE;
                type = LBRACE;
            }
            else if (ch=='}'){
                lexstate = DONE_STATE;
                type = RBRACE;
            }
            else if (ch == '.'){
                lexstate = DONE_STATE;
                type = DOT;
            }


            else if (ch == '\"') // CODE FOR SINGLE QUOTE NEED TEST CASES
            {
                lexstate = SCONST_STATE;
                type = SCONST;
            }
            else if (ch == '/')
            {
                char peek = in.peek();
                if (peek == '=')
                {
                    in.get(ch);
                    type = DIVASSOP;
                    lexstate = DONE_STATE;
                    break;
                }
                else if (peek == '/' || peek == '*')
                {
                    lexstate = COMMENT_STATE_INCOMPLETE; // might need to add a type
                }
                else
                {
                    type = DIV;
                    lexstate = DONE_STATE;
                    break;
                }
            }
            else if (ch == '+')
            {
                char peekvar = in.peek();
                if (peekvar == '=')
                {
                    in.get(ch);
                    type = ADDASSOP;
                    lexstate = DONE_STATE;
                    break; // breaks out of the switch statement to the check if "DONE"
                }
                else if (isdigit(peekvar))
                {
                    lexstate = ICONST_STATE;
                    type = ICONST;
                }
                else if (peekvar == '.')
                {
                    lexstate = ICONST_STATE; // PUT IN ICONST BC WE HAVENT CONSUMED THE '.'
                    type = RCONST;
                }
                else
                {
                    type = PLUS;
                    lexstate = DONE_STATE;
                    break;
                }
            }
            else if (ch == '-')
            {
                char peekvar = in.peek();
                if (peekvar == '=')
                { // might need to "CONSUME" next char
                in.get(ch);
                    type = SUBASSOP;
                    lexstate = DONE_STATE;
                    break;
                }
                else if (isdigit(peekvar))
                {
                    lexstate = ICONST_STATE;
                    type = ICONST;
                }
                else if (peekvar == '.')
                {
                    lexstate = ICONST_STATE;
                    type = RCONST;
                }
                else
                {
                    type = MINUS;
                    lexstate = DONE_STATE;
                    break;
                }
            }
            else if (ch == '*')
            {
                if (in.peek() == '=')
                {
                    in.get(ch);
                    type = MULASSOP;
                    lexstate = DONE_STATE;
                    break;
                }
                else
                {
                    type = MULT;
                    lexstate = DONE_STATE;
                    break;
                }
            }

            else if (ch == '=')
            {
                if (in.peek() == '=')
                {
                    in.get(ch);
                    type = EQ;
                    lexstate = DONE_STATE;
                    break;
                }
                else
                {
                    type = ASSOP;
                    lexstate = DONE_STATE;
                    break;
                }
            }
            else if (ch == '!')
            {
                if (in.peek() == '=')
                {
                    in.get(ch);
                    type = NEQ;
                    lexstate = DONE_STATE;
                    break;
                }
                else
                {
                    type = NOT;
                    lexstate = DONE_STATE;
                    break;
                }
            }
            else if (ch == '<')
            {
                type = LTHAN;
                lexstate = DONE_STATE;
                break;
            }
            else if (ch == '>')
            {
                type = GTHAN;
                lexstate = DONE_STATE;
                break;
            }
            else if (ch == '%')
            {
                if (in.peek() == '=')
                {
                    in.get(ch);
                    type = REMASSOP;
                    lexstate = DONE_STATE;
                    break;
                }
                else
                {
                    type = REM;
                    lexstate = DONE_STATE;
                    break;
                }
            }
            else if (ch == '&')
            {
                if (in.peek() == '&')
                {
                    in.get(ch);
                    type = AND;
                    lexstate = DONE_STATE;
                    break;
                } // MIGHT NEED TO ADD AN ERROR STATEMENT IF THERE ARENT TWO &&
            }
            else if (ch == '|')
            {
                if (in.peek() == '|')
                {
                    in.get(ch);
                    type = OR;
                    lexstate = DONE_STATE;
                    break;
                } // MIGHT NEED TO ADD AN ERROR STATEMENT IF THERE ARENT TWO ||
            }

            break;
        case IDENT_STATE:

            if (isalpha(ch) || isdigit(ch) || ch == '_')
            {
                lexstate = IDENT_STATE;
                type = IDENT;
            }

            break;
        case CCONST_STATE1:
            if(ch == '\''){
                lexstate = DONE_STATE;
            } else if(ch == '\n'){
                lexstate = ERROR_STATE;
                type = ERR;
                lexeme = "New Line is an invalid character constant.";
            }
            
            else{
                lexstate=CCONST_STATE2;
            }
            break;
        case CCONST_STATE2:
            if (ch == '\''){
                lexstate = DONE_STATE;
                type = CCONST;
            } else if(ch == '\n'){
                lexstate = ERROR_STATE;
                type = ERR;
                lexeme = "New Line is an invalid character constant.";
            }else {
            lexstate = ERROR_STATE;
            type = ERR;
            size_t firstNonSpace = 0;
        while (firstNonSpace < lexeme.length() && isspace(lexeme[firstNonSpace])) {
            firstNonSpace++;
        }
        if (firstNonSpace > 0) {
            lexeme = lexeme.substr(firstNonSpace);
        }
            lexeme = " Invalid character constant " + lexeme + "\'";
            }
            break;
        case ICONST_STATE:
            if (isdigit(ch))
            {
                lexstate = ICONST_STATE;
                type = ICONST;
            }
            else if (ch == '.')
            {
                lexstate = RCONST_STATE;
                type = RCONST;
            }
            break;
        case RCONST_STATE:
            if (isdigit(ch))
            {
                lexstate = RCONST_STATE;
                type = RCONST;
            }
            break;
        case SCONST_STATE:
            if (ch != '\"')
            {
                if (ch == '\'')
                {
                    lexstate = ERROR_STATE;
                    type = ERR;
                    size_t firstNonSpace = 0;
        while (firstNonSpace < lexeme.length() && isspace(lexeme[firstNonSpace])) {
            firstNonSpace++;
        }
        if (firstNonSpace > 0) {
            lexeme = lexeme.substr(firstNonSpace);
        }
                    lexeme = " Invalid string constant " + lexeme;
                }
                else if (ch == '\n')
                {
                    size_t firstNonSpace = 0;
        while (firstNonSpace < lexeme.length() && isspace(lexeme[firstNonSpace])) {
            firstNonSpace++;
        }
        if (firstNonSpace > 0) {
            lexeme = lexeme.substr(firstNonSpace);
        }
                    lexstate = ERROR_STATE;
                    type = ERR;
                    linenumber--;
                    lexeme = " Invalid string constant " + lexeme.substr(0,lexeme.length()-1);
                }
                else
                {
                    lexstate = SCONST_STATE;
                    type = SCONST;
                }
            }
            else
            {
                lexstate = DONE_STATE;
            }
            break;
        case COMMENT_STATE_INCOMPLETE:
            if (ch == '/')
            {
                lexstate = COMMENT_STATE;
            }
            else if (ch == '*')
            {
                lexstate = COMMENT_STATE_INCOMPLETE2;
            }
            else
            {
                lexstate = ERROR_STATE;
                type = ERR;
            }
            break;
        case COMMENT_STATE_INCOMPLETE2:
            if (ch == '*')
            {
                lexstate = COMMENT_STATE_INCOMPLETE3;
            }
            break;
        case COMMENT_STATE_INCOMPLETE3:
            if (ch == '/')
            {
                lexstate = START_STATE;
                lexeme = "";
            }
            else
            {
                lexstate = COMMENT_STATE_INCOMPLETE2;
            }
            break;
        case COMMENT_STATE:
            lexeme = "";
            break;
        case RESERVED_WORDS_STATE:
            break;
        case ERROR_STATE:
            break;
        case DONE_STATE:
            break;
        case DONE_WITH_FILE_STATE:
            break;
        }
        if (lexstate == DONE_STATE || lexstate == DONE_WITH_FILE_STATE || lexstate == ERROR_STATE)
        {
            break;
        }
        
        char peeking = in.peek();

        if((peeking == ',' || peeking == ';' || peeking == '(' || peeking == ')' || peeking == '{' || peeking == '}' || peeking == '!' || peeking == '?' || (peeking == '.' && lexstate!=ICONST_STATE)) && lexstate != SCONST_STATE && lexstate != COMMENT_STATE && lexstate != COMMENT_STATE_INCOMPLETE2 && lexstate != COMMENT_STATE_INCOMPLETE3 && lexstate != CCONST_STATE1 && lexstate != CCONST_STATE2){
            break;
        }
    }
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
        {"TRUE", TRUE},
        {"FALSE", FALSE}};

    string upper = lexeme;
    transform(lexeme.begin(), lexeme.end(), upper.begin(), ::toupper);
    if (type == IDENT)
    {

        if (StrToTok.find(upper) != StrToTok.end())
        {
            type = StrToTok[upper];
        }
    }

    if (upper == "TRUE" || upper == "FALSE")
    {
        type = BCONST;
    }
    if (in.eof())
    {
        if (lexstate == COMMENT_STATE_INCOMPLETE2 || lexstate == COMMENT_STATE_INCOMPLETE3 || lexstate == SCONST_STATE)
        {
            type = ERR;
        }
        else
        {
            type = DONE; // HERE
        }
        linenumber--;
    }
    
    return LexItem(type, lexeme, linenumber);
}






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

ostream &operator<<(ostream &out, const LexItem &tok)
{
    //cout<<tok.GetLinenum() << " ";
    switch (tok.GetToken())
    {
    case IDENT:
        cout << "IDENT: <" << tok.GetLexeme() << ">" << endl;
        break;
    case ICONST:
        cout << "ICONST: (" << tok.GetLexeme() << ")" << endl;
        break;
    case RCONST:
        cout << "RCONST: (" << tok.GetLexeme() << ")" << endl;
        break;
    case BCONST:
        cout << "BCONST: (" << tok.GetLexeme() << ")" << endl;
        break;
    case PROGRAM:
        out << "PROGRAM" << endl;
        break;
    case PRINT:
        out << "PRINT" << endl;
        break;
    case IF:
        out << "IF" << endl;
        break;
    case ELSE:
        out << "ELSE" << endl;
        break;
    case BOOL:
        out << "BOOL" << endl;
        break;
    case STRING:
        out << "STRING" << endl;
        break;
    case INT:
        out << "INT" << endl;
        break;
    case FLOAT:
        out << "FLOAT" << endl;
        break;
    case CHAR:
        out << "CHAR" << endl;
        break;
    case SCONST:
        out << "SCONST: " << tok.GetLexeme() << endl;
        break;
    case CCONST:
        out << "CCONST: " << tok.GetLexeme() << endl;
        break;
    case PLUS:
        out << "PLUS" << endl;
        break;
    case MINUS:
        out << "MINUS" << endl;
        break;
    case MULT:
        out << "MULT" << endl;
        break;
    case DIV:
        out << "DIV" << endl;
        break;
    case ASSOP:
        out << "ASSOP" << endl;
        break;
    case EQ:
        out << "EQ" << endl;
        break;
    case NEQ:
        out << "NEQ" << endl;
        break;
    case GTHAN:
        out << "GTHAN" << endl;
        break;
    case LTHAN:
        out << "LTHAN" << endl;
        break;
    case AND:
        out << "AND" << endl;
        break;
    case OR:
        out << "OR" << endl;
        break;
    case NOT:
        out << "NOT" << endl;
        break;
    case ADDASSOP:
        out << "ADDASSOP" << endl;
        break;
    case SUBASSOP:
        out << "SUBASSOP" << endl;
        break;
    case MULASSOP:
        out << "MULASSOP" << endl;
        break;
    case DIVASSOP:
        out << "DIVASSOP" << endl;
        break;
    case REMASSOP:
        out << "REMASSOP" << endl;
        break;
    case COMMA:
        out << "COMMA" << endl;
        break;
    case SEMICOL:
        out << "SEMICOL" << endl;
        break;
    case LPAREN:
        out << "LPAREN" << endl;
        break;
    case RPAREN:
        out << "RPAREN" << endl;
        break;
    case LBRACE:
        out << "LBRACE" << endl;
        break;
    case RBRACE:
        out << "RBRACE" << endl;
        break;
    case DOT:
        out << "DOT" << endl;
        break;
    case REM:
        out << "REM" << endl;
        break;
    case ERR:
        out << "ERR: Unrecognized Lexeme {" << tok.GetLexeme() << "} in line " << tok.GetLinenum() << endl;
        break;
    case DONE:
        out << "DONE: " << tok.GetLexeme() << endl;
        break;
    default:
        out << "UNKNOWN TOKEN: " << tok.GetLexeme() << endl;
        break;
    }
    return out;
}
