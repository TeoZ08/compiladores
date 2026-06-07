#ifndef SCANNER_H
#define SCANNER_H

#include "symboltable.h"
#include <fstream>

class Scanner
{
    private:
        string input;//Armazena o texto de entrada
        int pos;//Posição atual
        int line;
        int lexicalErrors;
        SymbolTable* st;

        bool isAtEnd();
        char currentChar();
        char nextChar();
        void skipSpacesAndComments();

    public:
    //Construtor
        Scanner(string, SymbolTable*);

        int getLine();
        int getLexicalErrors();

        //Método que retorna o próximo token da entrada
        Token* nextToken();

        //Método para manipular erros
        Token* lexicalError(string);
};

#endif
