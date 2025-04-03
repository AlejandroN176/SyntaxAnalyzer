#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H
using namespace std;
#include <iostream>
#include <fstream>
#include <map>
#include <vector>


class SyntaxAnalyzer{
private:
    vector<string> lexemes;
    vector<string> tokens;
    vector<string>::iterator lexitr;
    vector<string>::iterator tokitr;
    // map of variables and their datatype
    // i.e. sum t_integer
    map<string, string> symboltable; 

    // other private methods
    bool vdec(); // Alejandro
    int vars(); // Karen
    bool stmtlist(); //Kevin
    int stmt(); // Alejandro

    bool ifstmt(); // Marvin
    bool elsepart(); // Alejandro
    bool whilestmt(); // Karen
    bool assignstmt(); // Alejandro

    bool outputstmt(); // Kevin
    bool inputstmt(); //Karen
    bool expr(); // Karen
    bool simpleexpr(); //Kevin

    bool term(); // Marvin
    bool logicop(); // Marvin
    bool arithop(); // Marvin
    bool relop(); //Kevin

public:
    SyntaxAnalyzer(istream& infile);
    // pre: 1st parameter consists of an open file containing a source code's
    //	valid scanner/lexical analyzer output.  This data must be in the form: token : lexeme
    // post: the vectors have been populated

    bool parse();
    // pre: none
    // post: The lexemes/tokens have been parsed.
    // If an error occurs, a message prints indicating the token/lexeme pair
    // that caused the error.
    // If no error, vectors contain syntactically correct source code
};



#endif //SYNTAXANALYZER_H
