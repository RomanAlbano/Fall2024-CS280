//Roman Albano
//rba
//CS 280-011
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

int main(int argc, char** argv){
    
    ifstream inFile;
    inFile.open(argv[1]);
    char ch;
    while(inFile.get(ch)){

    }
}

int BalancedBrackets(string& inputString){
    bool brackets = false;
    stack<char> stack; 
    for (char c : inputString){
        if(c == '[' || c == '{' || c == '('){
            brackets = true;
            stack.push(c);
        }
        else if (c == ']' || c=='}' || c==')'){
            brackets = true;
            if(stack.empty()){
                cout << "case 4: There are more closing brackets than opening ones for one or more bracket types in the input string." << endl;
                return -1;
            }
            char topVal = stack.top();
            stack.pop();
            if ((topVal== '{' && c!= '}') || (topVal== '(' && c!= ')') || (topVal== '[' && c!= ']')){
                cout << "case 3: Mismatching bracket type between an opening and a closing bracket." << endl;
                return -1;
            }
        }
    }
    if (!stack.empty()){
        cout<< "case 2: There are more opening brackets than closing ones for one or more bracket types in the input string." << endl;
        return -1;
    }
    if (!brackets){
        cout << "case 5: Scanning the input string is completed successfully." << endl;
        return 0;
    }
   cout << "case 1: Scanning the input string is completed successfully." << endl;
   return 1;

}