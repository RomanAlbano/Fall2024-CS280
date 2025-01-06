//Roman Albano
//rba
//CS 280-011

#include <iostream>
#include <fstream>
#include "lex.h"
using namespace std;

int main(int argc, char** argv){
    if(argc == 1){
        cout << "No specified input file." << endl; //Check right wording
        return -1;
    }
    ifstream inFile;
    inFile.open(argv[1]);
    if(inFile.fail()){
        cout<<"CANNOT OPEN THE FILE " << argv[1] << endl;
        return -1;
    }

    if(inFile.peek() == std::ifstream::traits_type::eof()){
        cout << "Empty file." << endl;
    }

    bool all=false, num=false, bool1=false, char1=false, str=false, id=false, kw=false;
    for (int i = 2; i < argc; i++)
    {
        string argument = argv[i];
        if (argument[0]!='-'){
            cout<<"Only one file name is allowed." << endl;
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
        else if(argument == "-kw"){
            kw= true;
        }
        else if (argument == "-char"){
            char1= true;
        }
        else
        {
            cout << "UNRECOGNIZED FLAG " << argv[i] << endl;
            return -1;
        }
    }

    LexItem currentToken;
    int currentLineNum=1;
    while((currentToken = getNextToken(inFile, currentLineNum)).GetToken() != DONE ){ //MIGHT NOT NEED ERR
        if(currentToken == ERR){
            cout << "ERR: In line " << currentToken.GetLinenum() << ", Error Message {" << currentToken.GetLexeme() << "}"<<endl;
            return -1;
        }
        if(all){
            operator<<(cout,currentToken); //CHECK HOW TO DISPLAY
    } 
}
   

    

    

}