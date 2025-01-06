//Roman Albano
//rba
//CS 280-011
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


string RemoveVowels(string str, int curr){
    if(str.length() == 0 || curr<0 || curr > str.length()-1){
        return str;
    }
    char lowerLetter = tolower(str[curr]);
    if(lowerLetter == 'a' || lowerLetter == 'e' || lowerLetter == 'i' || lowerLetter == 'o' || lowerLetter == 'u'){
        return RemoveVowels(str.substr(0, curr) + str.substr(curr+1, str.length()-curr - 1), curr);
    }
    return RemoveVowels(str, curr+1);
}

// string RemoveVowels(string str, int curr){
//     if(str.length() == 0 || curr<0 || curr > str.length()-1){
//         return str;
//     }
//     char currentChar = tolower(str[curr]);
//     if(currentChar == 'a' || currentChar == 'e' || currentChar == 'i' || currentChar == 'o' || currentChar == 'u'){
//         return RemoveVowels(str.substr(0, curr) + str.substr(curr+1, str.length()-curr - 1), curr);
//     }
//     return RemoveVowels(str, curr+1);
// }