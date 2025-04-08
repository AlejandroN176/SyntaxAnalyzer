// Alejandro, Marvin, Kevin, Karen
/*
 * This code analyzes a file of tokens and lexemes that were derived from a source file.
 * The first thing our code does is populate the token and lexeme vectors.
 * Then there is a call to the parse function, which oversees parsing through the token/lexeme vectors,
 * and ensuring that they follow the grammar by checking for specific tokens and
 * calling on helper functions in the particular order of the grammar.
 * When a helper function returns true, it indicates that no error has been found and the code proceeds.
 * When false is returned, the code ends and
 * specific error messages are displayed to help locate the incorrect token/lexeme.
 * If the token and lexeme vectors have been successfully parsed,
 * the symbol table map that was populated within the code is displayed along with a success message.
 */

using namespace std;
#include "SyntaxAnalyzer.h"

int SyntaxAnalyzer::vars() {
    // Karen
    bool moreVars = false;
    string inputLine = *lexitr;
    string dataType;

    // Check if initial token is an integer or string, move on to next token if it is.
    if (tokitr != tokens.end() && (*tokitr == "t_integer" || *tokitr == "t_string")) {
        dataType = *lexitr;
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
        // Check if token is an id, if it is, add it to the symbol table and move on to next token .
        if (tokitr != tokens.end() && *tokitr == "t_id") {
            symboltable.insert(make_pair(*lexitr, dataType));
            tokitr++; lexitr++;
            inputLine += " " + *lexitr;
            // If the next token is a comma, move on to the next token and set moreVars to true.
            // If the token is a semicolon, return 1
            cout << "Passed id check in vars. Now checking for comma or semicolon: " << *lexitr << endl;
            if (tokitr != tokens.end() && *tokitr == "s_comma") {
                moreVars = true;
                tokitr++; lexitr++;
                inputLine += " " + *lexitr;
            } else if (tokitr != tokens.end() && *tokitr == "s_semi") {
                cout << "Done in vars: " << inputLine << endl;
                tokitr++; lexitr++;
                if (tokitr != tokens.end() && (*tokitr != "t_integer" || *tokitr != "t_string"))
                return 1;
            } else {
                cout << "Error in Vars: missing semicolon or comma: " << inputLine << endl;
            }
        } else {
            cout << "Error in Vars: no id found" << inputLine << endl;
            tokitr++; lexitr++;
            return -1;
        }
    } else {
        cout << "Error in Vars: expected data type" << inputLine << endl;
        return -1;
    }

    // Check for more vars until a semicolon or an error is found.
    // Adds new variables to symbol table
    while (moreVars) {
        if (tokitr != tokens.end() && *tokitr == "t_id") {
            symboltable.insert(std::make_pair(*lexitr, dataType));
            tokitr++; lexitr++;
            inputLine += " " + *lexitr;
            if (tokitr != tokens.end() && *tokitr == "s_comma") {
                tokitr++; lexitr++;
                inputLine += " " + *lexitr;
                moreVars = true;
            } else if (tokitr != tokens.end() && *tokitr == "s_semi") {
                tokitr++; lexitr++;
                inputLine += " " + *lexitr;
                if (tokitr != tokens.end() && (*tokitr != "t_integer" && *tokitr != "t_string")) {
                    moreVars = false;
                }else {
                    dataType = *lexitr;
                    tokitr++; lexitr++;
                }
            } else {
                cout << "Error in vars: expected comma or semicolon: " << inputLine << endl;
                return -1;
            }
        } else {
            cout << "Error in vars: expected an id: " << inputLine << endl;
            return -1;
        }
    }

    //return 1 if everything was successful
    return 1;
}

bool SyntaxAnalyzer::inputstmt() {
    // Karen
    string inputLine = *lexitr;
    // Check for input
    if (tokitr != tokens.end() && *tokitr == "t_input") {
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
        // Check for left parenthesis
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            tokitr++; lexitr++;
            inputLine += " " + *lexitr;
            // Check for id
            if (tokitr != tokens.end() && *tokitr == "t_id") {
                // Make sure variable used was declared
                if (symboltable.contains(*lexitr)) {
                    tokitr++; lexitr++;
                    inputLine += " " + *lexitr;
                } else {
                    cout << "Error in InputStmt: undeclared variable used" << endl;
                    return false;
                }
                // Check for the final right parenthesis
                if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                    tokitr++; lexitr++;
                    return true;
                }
                cout << "Error in InputStmt: right parenthesis not found" << inputLine << endl;

            } else {
                cout << "Error in InputStmt: id not found " << inputLine << endl;
            }
        } else {
            cout << "Error in InputStmt: left parenthesis not found " << inputLine << endl;
        }
    } else {
        cout << "Error in InputStmt: input statement not found" << inputLine << endl;
    }
    return false;
}

bool SyntaxAnalyzer::expr() {
    // Karen
    // Check for simpleexpr()
    if (tokitr != tokens.end() && simpleexpr()) {
        // Check for logicop()
        if (tokitr != tokens.end() && logicop()) {
            tokitr++; lexitr++;
            // Check for simpleexpr()
            if (tokitr != tokens.end() && simpleexpr()) {
                tokitr++; lexitr++;
                return true;
            }
            cout << "Error in Expr: problem with simpleeexpr()" << endl;
            return false;
        }
        // true is returned because logicop() and simpleeexpr() are optional
        return true;
    }
    cout << "Error in Expr: problem with simpleexpr()" << endl;
    return false;
}

bool SyntaxAnalyzer::whilestmt() {
    // Karen
    // Check for while
    string inputLine = *lexitr;
    if (tokitr != tokens.end() && *tokitr == "t_while") {
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
        // Check for left parenthesis
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            tokitr++; lexitr++;
            inputLine += " " + *lexitr;
            // Check for expr()
            if (tokitr != tokens.end() && expr()) {
                tokitr++; lexitr++;
                inputLine += "[...expr] " + *lexitr;
                // Check for left brace
                if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
                    tokitr++; lexitr++;
                    inputLine += " " + *lexitr;
                    // Check for stmtlist()
                    if (tokitr != tokens.end() && stmtlist()) {
                        tokitr++; lexitr++;
                        inputLine += "[... stmtlist] " + *lexitr;
                        // Last check for right brace
                        if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                            return true;
                        }
                        cout << "Error in whilestmt: expected right brace"  << inputLine <<endl;
                        return false;
                    }
                    cout << "Error in whilestmt: problem with stmtlist()" << inputLine << endl;
                    return false;
                }
                cout << "Error in whilestmt: expected left brace" << inputLine << endl;
                return false;
            }
            cout << "Error in whilestmt: problem with expr()" << inputLine << endl;
            return false;
        }
        cout << "Error in whilestmt: expected left parenthesis "  << inputLine << endl;
        return false;
    }
    cout << "Error in whilestmt: while statement expected" << inputLine << endl;
    return false;
}

bool SyntaxAnalyzer::logicop() {
    // Marvin
    if(tokitr != tokens.end() && *tokitr == "s_and"){
        tokitr++; lexitr++;
        return true;
    }
    if(tokitr != tokens.end() && *tokitr == "s_or"){
        tokitr++; lexitr++;
        return true;
    }
    return false;
}


bool SyntaxAnalyzer::simpleexpr() {
    // Kevin
        if (tokitr != tokens.end() && term()) {
            if (tokitr != tokens.end() && (relop() || arithop())) {
                if (tokitr != tokens.end() && term()) {
                    return true;
                }else cout << "Error: expected term symbol at line: " << *lexitr << endl;
            }
            else {
                return true;
            }
        }else cout << "Error: expected term symbol at line: " << *lexitr << endl;
    return false;
}

bool SyntaxAnalyzer::arithop() {
    // Marvin
    if (tokitr != tokens.end() && *tokitr == "s_plus"){
        tokitr++; lexitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "s_minus"){
        tokitr++; lexitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "s_div"){
        tokitr++; lexitr++;
        return true;
    }
    return false;
}

bool SyntaxAnalyzer::assignstmt() {
    // Alejandro
    if (tokitr != tokens.end() && *tokitr == "t_id") {
        if (symboltable.contains(*lexitr)) {
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && *tokitr == "s_assign"){
                tokitr++; lexitr++;
                if (tokitr != tokens.end() && expr()) {
                    if (tokitr != tokens.end() && *tokitr == "s_semi") {
                        tokitr++; lexitr++;
                        return true;
                    }else {
                        cout<< "Error expected semicolon at line: " << *lexitr << endl;
                    }

                }else {
                    cout << "Error expected valid expression at line: "<< *lexitr << endl;
                    return false;
                }
            }else {
                cout << "Error expected assignment symbol at line: "<< *lexitr << endl;
                return false;
            }
        } else {
            cout << "Error id not inside symbol table at line: "<< *lexitr << endl;
            return false;
        }
    }else {
        cout << "Error Expected id at line: "<< *lexitr << endl;
        return false;
    }
}

bool SyntaxAnalyzer::elsepart() {
    // Alejandro
    if (tokitr != tokens.end() && *tokitr == "t_else") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
            tokitr++; lexitr++;
            if (stmtlist()) {
                if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                    tokitr++; lexitr++;
                    return true;
                }else cout << "Error: expected right brace symbol at line: " << *lexitr << endl;
            }else cout << "Error: expected valid statement list type at line :" << *lexitr  << endl;
        }else cout << "Error: expected left brace symbol at line: " << *lexitr << endl;
    }
    //it can be nothing so return true?
    return true;
}

bool SyntaxAnalyzer::vdec() {
    // Alejandro
    if(tokitr != tokens.end() && *tokitr == "t_var") {
        tokitr++; lexitr++;
        if (vars() != 1) {
            return false;
        }
    }
    return true;
}

bool SyntaxAnalyzer::ifstmt() {
    // Marvin
    bool flag = false;
    if (tokitr != tokens.end() && *tokitr == "t_if") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lparen"){
            tokitr++; lexitr++;
            if (expr()){
                if (tokitr != tokens.end() && *tokitr == "s_rparen"){
                    tokitr++; lexitr++;
                    if (tokitr != tokens.end() && *tokitr == "s_lbrace"){
                        tokitr++; lexitr++;
                        if (stmtlist()){
                            if (tokitr != tokens.end() && *tokitr == "s_rbrace"){
                                tokitr++; lexitr++;
                                if (elsepart()){
                                    flag = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return flag;
}

bool SyntaxAnalyzer::outputstmt() {
    // Kevin
    if (tokitr != tokens.end() && *tokitr == "t_output") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            tokitr++; lexitr++;
        	if (tokitr != tokens.end() && expr() || *tokitr == "t_text") {
            	if (tokitr != tokens.end() && *tokitr == "s_rparen") {
            	    tokitr++; lexitr++;
            	    return true;
            	}else cout << "Error: expected right parenthesis symbol at line: " << *lexitr << endl;
        	}else cout << "Error: expected text or expr symbol at line: " << *lexitr << endl;
        }else cout << "Error: expected left parenthesis symbol at line: " << *lexitr << endl;
    }else cout << "Error: expected output method at line: " << *lexitr << endl;
        return false;
}

bool SyntaxAnalyzer::relop() {
    // Kevin
    if (tokitr != tokens.end() && *tokitr == "s_ne" || *tokitr == "s_eq" || *tokitr == "s_gt" || *tokitr == "s_lt") {
        tokitr++; lexitr++;
        return true;
    }
    else {
        return false;
    }
}

int SyntaxAnalyzer::stmt() {
    // Alejandro
    if (tokitr != tokens.end()) {
        if (*tokitr == "t_if") {
            if (ifstmt())
                return 1;
        }else if (*tokitr == "t_id") {
            if (assignstmt())
                return 1;
        }else if (*tokitr == "t_while") {
            if (whilestmt())
                return 1;
        }else if (*tokitr == "t_input") {
            if (inputstmt())
                return 1;
        }else if (*tokitr == "t_output") {
           if (outputstmt())
               return 1;
        }
    }
    return -1;
}

bool SyntaxAnalyzer::term() {
    // Marvin
    if (tokitr != tokens.end() && *tokitr == "t_id"){
        tokitr++; lexitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "t_text"){
        tokitr++; lexitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "t_number"){
        tokitr++; lexitr++;
        return true;
    }else if (expr()){
        tokitr++; lexitr++;
        return true;
    }
    tokitr++; lexitr++;
    return false;
}

bool SyntaxAnalyzer::stmtlist() {
    // Kevin
    bool stmtContinue = true;
    while (tokitr != tokens.end() && stmtContinue) {
        if (stmt() != 1) {
            stmtContinue = false;
        }
    }
    return true;
}

SyntaxAnalyzer::SyntaxAnalyzer(istream &infile) {
    // Alejandro, Marvin, Kevin, Karen
    if (!infile) {
        cout << "sourceLexemes.txt not found" << endl;
    }
    char semicolon;
    string token, lexeme;
    while (infile >> token >> semicolon) {
        getline(infile >> ws, lexeme);
        cout << token << " " << lexeme << endl;
        tokens.push_back(token);
        lexemes.push_back(lexeme);
    }
}

bool SyntaxAnalyzer::parse() {
    // Alejandro, Marvin, Kevin, Karen
    tokitr = tokens.begin();
    lexitr = lexemes.begin();
    bool success = false;
    if (vdec()){
        if (tokitr != tokens.end() && *tokitr == "t_main") {
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
                tokitr++; lexitr++;
                if (tokitr != tokens.end() && stmtlist()) {
                    if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                        tokitr++; lexitr++;
                        success = true;
                    }
                } else {
                    cout << "Token/Lexime Pair that caused error: "<< *tokitr-- << " : " << *lexitr-- << endl;
                    return success;
                }
            } else {
                cout << "Token/Lexime Pair that caused error: "<< *tokitr-- << " : " << *lexitr-- << endl;
                return success;
            }
        }else {
            cout << "Token/Lexime Pair that caused error: "<< *tokitr-- << " : " << *lexitr-- << endl;
            return success;
        }
    } else {
        cout << "Token/Lexime Pair that caused error: "<< *tokitr-- << " : " << *lexitr-- << endl;
        return success;
    }

    if (tokitr != tokens.end()) {
        success = false;
        cout << "Token/Lexime Pair that caused error: "<< *tokitr-- << " : " << *lexitr-- << endl;
        return success;
    }
    cout << "Symbol Table" << endl;
    cout << "------------"<< endl;
    for (const auto& pair : symboltable) {
        cout << pair.first << ": " << pair.second << endl;
    }
    cout << "------------"<< endl;
    return success;
}
