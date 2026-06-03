#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <ctype.h>//Funções de caracteres
#include <string>

using namespace std;

enum Names 
{
    UNDEF,
    //Identificadores e constantes
    ID,
    INTEGERCONST,
    CHARCONST,
    STRINGCONST,

    //Operadores
    PLUS,
    MINUS,
    MULT,
    DIV,
    ASSIGN,
    EQUAL,
    NOTEQUAL,
    LESSEQUAL,
    LESS,
    GREATEREQUAL,
    GREATER,
    AND,
    OR,
    NOT,

    //Separadores
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,
    LBRACKET,
    RBRACKET,
    COMMA,
    SEMICOLON,

    END_OF_FILE
};

class Token 
{
    public: 
        int name;
        int attribute;
        string lexeme;
    
        Token(int name)
        {
            this->name = name;
            attribute = UNDEF;
        }

        Token(int name, string l)
        {
            this->name = name;
            attribute = UNDEF;
            lexeme = l;
        }
        
        Token(int name, int attr)
        {
            this->name = name;
            attribute = attr;
        }
};

#endif
