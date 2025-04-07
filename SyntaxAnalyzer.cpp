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
    if (tokitr != tokens.end() && *tokitr == "s_comma") {
        moreVars = true;
        tokitr++; lexitr++;
        inputLine += " " + *lexitr;
    } else if (tokitr != tokens.end() && *tokitr == "s_semi") {
        tokitr++; lexitr++;
        return 1;
    } else {
        cout << "Error in Vars: missing semicolon or comma: " << inputLine << endl;
    }

    // Check for more vars until a semicolon or an error is found.
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
            if (tokitr != tokens.end() && *tokitr == "t_id") {// add a symbol table check
                tokitr++; lexitr++;
                inputLine += " " + *lexitr;
                if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                    tokitr++; lexitr++;
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
                if (tokitr != tokens.end() && *tokitr == "s_rparen") {
                    tokitr++; lexitr++;
                    if (tokitr != tokens.end() && *tokitr == "s_lbrace") {
                        tokitr++; lexitr++;
                        if (tokitr != tokens.end() && stmtlist()) {
                            tokitr++; lexitr++;
                            if (tokitr != tokens.end() && *tokitr == "s_rbrace") {
                                tokitr++; lexitr++;
                                return true;
                            }
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
        if (tokitr != tokens.end() && term()) {
            if (tokitr != tokens.end() && *tokitr == "s_semi") {
                tokitr++; lexitr++;
                return true;
            }
            if (tokitr != tokens.end() && (relop() || arithop())) {
                if (tokitr != tokens.end() && term()) {
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
    if (tokitr != tokens.end() && *tokitr == "t_id") {
        int found = symboltable.count(*lexitr);
        tokitr++; lexitr++;
        if (tokitr != tokens.end() && *tokitr == "s_assign"){
            tokitr++; lexitr++;
            if (tokitr != tokens.end() && expr()) {
                return true;
            }else {
                cout << "Error expected valid expression at line: "<< *lexitr << endl;
                return false;
            }
        }else {
            cout << "Error expected assignment symbol at line: "<< *lexitr << endl;
            return false;
        }
    }else {
        cout << "Error id not inside symbol table at line: "<< *lexitr << endl;
        return false;
    }
}

bool SyntaxAnalyzer::elsepart() {
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
    if (tokitr != tokens.end() && *tokitr == "s_ne" || *tokitr == "s_eq" || *tokitr == "s_gt" || *tokitr == "s_lt") {
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
        }else if (*tokitr == "t_id") {
            if (assignstmt())
                return 1;
        }else if (*tokitr == "t_while") {
            whilestmt();
            return 1;
        }else if (*tokitr == "t_input") {
            inputstmt();
            return 1;
        }else if (*tokitr == "t_output") {
            outputstmt();
            return 1;
        }else {
            return -1;
        }
    }
}

bool SyntaxAnalyzer::term() {
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
    bool stmtContinue = true;
    while (tokitr != tokens.end() && stmtContinue) {
        if (stmt() != 1) {
            stmtContinue = false;
        }
    }
    return true;
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
