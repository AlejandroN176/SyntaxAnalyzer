using namespace std;
#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(istream &infile) {
    if (!infile) {
        cout << "sourceLexemes.txt not found" << endl;
    }
    char semicolon;
    string token, lexeme;
    while (infile >> token >> semicolon >> lexeme) {
        tokens.push_back(token);
        lexemes.push_back(lexeme);
    }
}

int SyntaxAnalyzer::vars() {
    bool moreVars = false;
    string inputLine = *lexitr;
    string dataType;

    // Check if initial token is an integer or string, move on to next token if it is.
    // If it's not, display error and return -1
    cout << "In vars. First checking; " << *lexitr << endl;
    if (tokitr != tokens.end() && (*tokitr == "t_integer" || *tokitr == "t_string")) {
        dataType = *lexitr;
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
    } else {
        cout << "Error in Vars: incorrect data type" << inputLine << endl;
        tokitr++; lexitr++;
        return -1;
    }

    // Check if token is an id, if it is move one to next token and increment varCount.
    // If it's not, display an error and return -1
    cout << "passed data type check in vars. Now checking for id: " << *lexitr << endl;
    if (tokitr != tokens.end() && *tokitr == "t_id") {
        symboltable.insert(make_pair(*lexitr, dataType));
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
    } else {
        cout << "Error in Vars: no id found" << inputLine << endl;
        tokitr++; lexitr++;
        return -1;
    }

    // If the next token is a comma, move on to the next token and set moreVars to true.
    // If the token is a semicolon, return 1
    // If it's neither, display an error and return -1
    cout << "Passed id check in vars. Now checking for comma or semicolon: " << *lexitr << endl;
    if (tokitr != tokens.end() && *tokitr == "s_comma") {
        moreVars = true;
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
    } else if (tokitr != tokens.end() && *tokitr == "s_semi") {
        cout << "Done in vars: " << inputLine << endl;
        tokitr++; lexitr++;
        return 1;
    } else {
        cout << "Error in Vars: missing semicolon or comma: " << inputLine << endl;
    }

    // Check for more vars until a semicolon or an error is found.
    while (moreVars) {
        cout << "In loop for moreVars. Checking for: " << *lexitr << endl;
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
                cout << "All done in moreVars, returning: "<< *lexitr << endl;
                moreVars = false;
            } else {
                cout << "Error in Vars: missing semicolon: " << inputLine << endl;
                tokitr++; lexitr++;
                return -1;
            }
        } else {
            cout << "Error in Vars: no id found" << inputLine << endl;
            tokitr++; lexitr++;
            return -1;
        }
    }

    //return 1 if everything was successful
    return 1;
}

bool SyntaxAnalyzer::inputstmt() {
    string inputLine = *lexitr;
    if (tokitr != tokens.end() && *tokitr == "t_input") {
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            tokitr++; lexitr++;
            inputLine += " " + *lexitr;
            if (tokitr != tokens.end() && *tokitr == "t_id") {
                tokitr++; lexitr++;
                inputLine += " " + *lexitr;
                if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                    return true;
                }
                cout << "Error in InputStmt: " << inputLine << endl;
            } else {
                cout << "Error in InputStmt: " << inputLine << endl;
            }
        } else {
            cout << "Error in InputStmt: " << inputLine << endl;
        }
    } else {
        cout << "Error in InputStmt: " << inputLine << endl;
    }
    return false;
}

bool SyntaxAnalyzer::expr() {
    if (tokitr != tokens.end() && simpleexpr()) {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && logicop()) {
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && simpleexpr()) {
                tokitr++; lexitr++;
                return true;
            }
        } else {
            return true;
        }
    }
    return false;
}

bool SyntaxAnalyzer::whilestmt() {
    if (tokitr != tokens.end() && *tokitr == "t_while") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && expr()) {
                tokitr++; lexitr++;
                if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
                    tokitr++; lexitr++;
                    if (tokitr != tokens.end() && stmtlist()) {
                        tokitr++; lexitr++;
                        if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool SyntaxAnalyzer::logicop() {
    if(tokitr != tokens.end() && *tokitr == "s_and"){
        tokitr++;
        return true;
    }
    if(tokitr != tokens.end() && *tokitr == "s_or"){
        tokitr++;
        return true;
    }
    return false;
}
bool SyntaxAnalyzer::stmtlist() {
    while (tokitr != tokens.end()) {
        if (stmt() == -1) {
            return false;
        }
        tokitr++; lexitr++;
        return true;
    }
    return false;
}

bool SyntaxAnalyzer::simpleexpr() {
        if (tokitr != tokens.end() && term()) {
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && relop() || arithop()) {
                tokitr++; lexitr++;
                if (tokitr != tokens.end() &&term()) {
                    tokitr++; lexitr++;
                    return true;
                }
            }
            else {
                return true;
            }
        }
    return false;
}
bool SyntaxAnalyzer::arithop() {
    if (tokitr != tokens.end() && *tokitr == "s_plus"){
        tokitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "s_minus"){
        tokitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "s_div"){
        tokitr++;
        return true;
    }
}
bool SyntaxAnalyzer::assignstmt() {
    if (tokitr != tokens.begin()) {
        tokitr--; lexitr--;
    }
    if (tokitr != tokens.end() && *tokitr == "t_id") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_assign"){
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && expr()) {
                tokitr++; lexitr++;
                return true;
            }else {
                cout << "Error expected valid expression" << endl;
                return false;
            }
        }else {
            cout << "Error expected assignment symbol" << endl;
            return false;
        }
    }else {
        cout << "Error expected variable id " << endl;
        return false;
    }
}
bool SyntaxAnalyzer::elsepart() {
    if (tokitr != tokens.end() && *tokitr == "t_else") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
            tokitr++; lexitr++;
            if (stmtlist()) {
                if (tokitr != tokens.end() && *tokitr == "t_rbrace") {
                    tokitr++; lexitr++;
                    return true;
                }else cout << "Error: expected right brace symbol at" << tokitr->size() << endl;
            }else cout << "Error: expected valid statement list type at" << tokitr->size() << endl;
        }else cout << "Error: expected left brace symbol at" << tokitr->size() << endl;
    }
    //it can be nothing so return true?
    return true;
}
bool SyntaxAnalyzer::vdec() {

    while (tokitr != tokens.end() && *tokitr == "t_var") {
        tokitr++; lexitr++;
        if (vars() != 1) {
            return false;
        }
    }
    return true;
}

bool SyntaxAnalyzer::ifstmt() {
    bool flag = false;
    if (tokitr != tokens.end() && *tokitr == "t_if"){
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lparen"){
            tokitr++; lexitr++;
            if (expr()){
                tokitr++; lexitr++;
                if (tokitr != tokens.end() && *tokitr == "s_rparen"){
                    tokitr++; lexitr++;
                    if (tokitr != tokens.end() && *tokitr == "s_lbrace"){
                        tokitr++; lexitr++;
                        if (stmtlist()){
                            tokitr++; lexitr++;
                            if (tokitr != tokens.end() && *tokitr == "s_rbrace"){
                                //flag = true;
                                if (elsepart()){ //my version - elsepart() always
                                    //returns true
                                    flag = true;
                                }
                            }
                        }
                    }
                }
            }
        }

    }
    tokitr++;
    return flag;
}
bool SyntaxAnalyzer::outputstmt() {
    if (tokitr != tokens.end() && *tokitr == "t_output") {
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_lparen") {
            tokitr++; lexitr++;
        	if (tokitr != tokens.end() && expr() || *tokitr == "t_text") {
            	tokitr++; lexitr++;
            	if (tokitr != tokens.end() && *tokitr == "s_rparen") {
            	    tokitr++; lexitr++;
            	    return true;
            	}
        	}
        }
    }
        return false;
}

bool SyntaxAnalyzer::relop() {
    if (tokitr != tokens.end() && *tokitr == "s_nequal" || *tokitr == "s_eq" || *tokitr == "s_gt" || *tokitr == "s_lt") {
        tokitr++; lexitr++;
        return true;
    }
    else {
        return false;
    }
}


int SyntaxAnalyzer::stmt() {
    if (tokitr != tokens.end()) {
        if (*tokitr == "t_if") {
            ifstmt();
            return 1;
        }else if (*tokitr == "s_assign") {
            assignstmt();
            return 1;
        }else if (*tokitr == "s_while") {
            whilestmt();
            return 1;
        }else if (*tokitr == "t_input") {
            inputstmt();
            return 1;
        }else if (*tokitr == "t_output") {
            outputstmt();
            return 1;
        }else {
            cout << "Error expected valid Stmt" << endl;
            return -1;
        }
    }
}

bool SyntaxAnalyzer::term() {
    if (tokitr != tokens.end() && *tokitr == "t_id"){
        tokitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "t_text"){
        tokitr++;
        return true;
    }
    else if (tokitr != tokens.end() && *tokitr == "t_number"){
        tokitr++;
        return true;
    }

    if (expr()){
        tokitr++;
        return true;
    }
    tokitr++;
    return false;
}

bool SyntaxAnalyzer::parse() {
    tokitr = tokens.begin();
    lexitr = lexemes.begin();
    bool success = false;
    if (vdec()){
        if (tokitr != tokens.end() && *tokitr == "t_main") {
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
                tokitr++; lexitr++;
                cout << "Vdec intialized and main{ intialized"<< endl;
                // Calls stmtlist
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
    for (const auto& pair : symboltable) {
        cout << pair.first << ": " << pair.second << endl;
    }
    return success;
}
