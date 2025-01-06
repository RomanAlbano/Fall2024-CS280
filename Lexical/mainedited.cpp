// Roman Albano
// rba
// CS 280-011

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include "lex.h"
#include <string>
using namespace std;

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        cout << "No specified input file." << endl;
        return -1;
    }

    ifstream inFile;
    inFile.open(argv[1]);
    if (inFile.fail())
    {
        cout << "CANNOT OPEN THE FILE " << argv[1] << endl;
        return -1;
    }

    if (inFile.peek() == std::ifstream::traits_type::eof())
    {
        cout << "Empty file." << endl;
        return 0; // Return 0 here as no error but empty file
    }

    // Token collections
    set<string> identifiers;
    map<string, int> keywords;
    set<string> numericConstants;
    set<string> booleanConstants;
    set<string> stringConstants;
    set<string> charConstants;

    // Counters
    // int lineCount = 0;
    int tokenCount = 0, identifierCount = 0, numCount = 0, boolCount = 0, strCount = 0, charCount = 0, keyCount = 0;

    LexItem currentToken;
    int currentLineNum = 1;
    bool all = false, num = false, bool1 = false, char1 = false, str = false, id = false, kw = false;
    for (int i = 2; i < argc; i++)
    {
        string argument = argv[i];
        if (argument[0] != '-')
        {
            cout << "Only one file name is allowed." << endl;
            return -1;
        }
        if (argument == "-all")
        {
            all = true;
        }
        else if (argument == "-num")
        {
            num = true;
        }
        else if (argument == "-bool")
        {
            bool1 = true;
        }
        else if (argument == "-str")
        {
            str = true;
        }
        else if (argument == "-id")
        {
            id = true;
        }
        else if (argument == "-kw")
        {
            kw = true;
        }
        else if (argument == "-char")
        {
            char1 = true;
        }
        else
        {
            cout << "Unrecognized flag {" << argv[i] << "}" << endl;
            return -1;
        }
    }
    while ((currentToken = getNextToken(inFile, currentLineNum)).GetToken() != DONE)
    {
        if (currentToken == ERR)
        {
            cout << "ERR: In line " << currentToken.GetLinenum() << ", Error Message {" << currentToken.GetLexeme() << "}" << endl;
            cout << endl;
            return -1;
        }

        tokenCount++; // Increment total token count

        if (all)
        {
            operator<<(cout, currentToken);
        }
        // Track tokens by type
        switch (currentToken.GetToken())
        {
        case IDENT:
            if (identifiers.find(currentToken.GetLexeme()) == identifiers.end()) {
            identifiers.insert(currentToken.GetLexeme());
            identifierCount++;
        }
            break;
        case ICONST:
        case RCONST:
            if (numericConstants.find(currentToken.GetLexeme()) == numericConstants.end())
            {

                numericConstants.insert(currentToken.GetLexeme());
                numCount++;
            }
            break;
        case SCONST:
            stringConstants.insert(currentToken.GetLexeme());
            strCount++;
            break;
        case BCONST:
            booleanConstants.insert(currentToken.GetLexeme());
            boolCount++;
            break;
        case CCONST:
            charConstants.insert(currentToken.GetLexeme());
            charCount++;
            break;
        case PLUS:
            break;
        case MINUS:
            break;
        case MULT:
            break;
        case DIV:
            break;
        case ASSOP:
            break;
        case EQ:
            break;
        case NEQ:
            break;
        case LTHAN:
            break;
        case GTHAN:
            break;
        case REM:
            break;
        case AND:
            break;
        case OR:
            break;
        case NOT:
            break;
        case ADDASSOP:
            break;
        case SUBASSOP:
            break;
        case MULASSOP:
            break;
        case DIVASSOP:
            break;
        case REMASSOP:
            break;
        case COMMA:
            break;
        case SEMICOL:
            break;
        case LPAREN:
            break;
        case RPAREN:
            break;
        case LBRACE:
            break;
        case RBRACE:
            break;
        case DOT:
            break;

        //"if", "else", "print", "int", "float", "char", "string", "bool", "program"
        case IF:
        case ELSE:
        case PRINT:
        case INT:
        case FLOAT:
        case CHAR:
        case STRING:
        case BOOL:
        case PROGRAM:
            if (keywords.find(currentToken.GetLexeme()) == keywords.end())
        {
            keywords[currentToken.GetLexeme()] = 1;
            keyCount++;
        }

            break;
        case TRUE:
            break;
        case FALSE:
            break;
        case ERR:
            break;
        case DONE:
            break;
        }
    }

    cout << endl;
    cout << "Lines: " << currentToken.GetLinenum() << endl;
    cout << "Total Tokens: " << tokenCount << endl;
    cout << "Identifiers and Keywords: " << identifierCount + keyCount << endl;
    cout << "Numerics: " << numCount << endl;
    cout << "Booleans: " << boolCount << endl;
    cout << "Strings and Characters: " << strCount + charCount << endl;

    // Handle the output for each flag

    if (num)
    {
        cout << "NUMERIC CONSTANTS:" << endl;

        // Convert the set of numeric constants to a vector of floating-point numbers for sorting
        vector<double> numList;
        for (const auto &numStr : numericConstants)
        {
            // Convert the string to a double
            double numValue = stod(numStr);
            numList.push_back(numValue);
        }

        // Sort the numeric constants in numerical order
        sort(numList.begin(), numList.end());

        // Print each numeric constant, formatted without a leading positive sign and with 0 before the decimal point
        for (auto it = numList.begin(); it != numList.end(); ++it)
        {
            if (it != numList.begin())
            {
                cout << ", ";
            }

            // Convert the double back to string to format it
            string numStr = to_string(*it);

            // Remove any trailing zeroes after the decimal point and ensure there's no positive sign
            numStr.erase(numStr.find_last_not_of('0') + 1, string::npos); // Remove trailing zeroes
            if (numStr.back() == '.')
            { // If the string ends with a decimal point, remove it
                numStr.pop_back();
            }
            // Ensure there's a 0 before the decimal point for numbers like .5
            if (numStr[0] == '-' && numStr[1] == '.')
            {
                numStr = "-0" + numStr.substr(1, numStr.length() - 1);
            }
            if (numStr[0] == '.')
            {
                numStr = "0" + numStr;
            }

            // Print the formatted number
            cout << numStr;
        }
        cout << endl;
    }

    if (bool1)
    {
        cout << "BOOLEAN CONSTANTS:" << endl;
        for (auto it = booleanConstants.begin(); it != booleanConstants.end(); ++it)
        {
            if (it != booleanConstants.begin())
                cout << ", ";
            cout << *it;
        }
        cout << endl;
    }
    if (char1)
    {
        cout << "CHARACTER CONSTANTS:" << endl;
        for (auto it = charConstants.begin(); it != charConstants.end(); ++it)
        {
            if (it != charConstants.begin())
                cout << ", ";
            cout << *it;
        }
        cout << endl;
    }
    if (str)
    {
        cout << "STRINGS:" << endl;
        for (auto it = stringConstants.begin(); it != stringConstants.end(); ++it)
        {
            if (it != stringConstants.begin())
                cout << ", ";
            cout << *it;
        }
        cout << endl;
    }
    if (id)
    {
        cout << "IDENTIFIERS:" << endl;
        for (auto it = identifiers.begin(); it != identifiers.end(); ++it)
        {
            if (it != identifiers.begin())
                cout << ", ";
            cout << *it;
        }
        cout << endl;
    }
    if (kw)
    {
        // Custom order of keywords
        vector<string> customOrder = {
            "if", "else", "print", "int", "float", "char", "string", "bool", "program"};

        cout << "KEYWORDS:" << endl;

        bool first = true; // To manage comma placement

        // Iterate through the custom order and print only if the keyword is found in the map
        for (const string &kw : customOrder)
        {
            if (keywords.find(kw) != keywords.end())
            {
                if (!first)
                {
                    cout << ", ";
                }
                cout << kw;
                first = false;
            }
        }

        cout << endl;
    }

    return 0;
}
