using namespace std;
#include <iostream>
#include <fstream>
#include <vector>
#include "SyntaxAnalyzer.h"
#include "SyntaxAnalyzer.cpp"


int main() {
    ifstream sourceLexemes ("sourceLexemes.txt");
    SyntaxAnalyzer syntax_analyzer (sourceLexemes);
    if (syntax_analyzer.parse()) {
        cout << "Success" << endl;
    } else {
        cout << "Failure" << endl;
    }
    return 0;
}