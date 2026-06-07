#ifndef PARSER_H
#define PARSER_H

#include "scanner.h"
#include <set>

class Parser
{
    private:
        Scanner* scanner;
        Token* lToken;
        SymbolTable* globalST;
        SymbolTable* currentST;
        int errors;

        void advance();
        void match(int);
        void error(string);
        void initSimbolTable();
        string tokenName(int);
        string lookahead();

        bool isType(int);
        bool isStatementStart(int);
        bool isExpressionStart(int);
        bool isRelOp(int);
        bool isAddOp(int);
        bool isMulOp(int);
        bool isSyncToken(int);
        bool isFunctionStart(int);
        void syncTo(set<int>);

        void program();
        void function();
        void type();
        void paramTypes();
        void paramRest();
        void varDeclaration();
        void declarationList();
        void statementList();
        void statement();
        void compoundStmt();
        void assign();
        void assignRest();
        void callRest();
        void expressionList();

        void expression();
        void logOr();
        void logAnd();
        void rel();
        void add();
        void mult();
        void unary();
        void primary();

    public:
        Parser(string);
        void run();
};

#endif
