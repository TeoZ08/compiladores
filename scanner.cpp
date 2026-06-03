#include "scanner.h"

//Construtor que recebe uma string com o nome do arquivo
//de entrada e preenche input com seu conteúdo.
Scanner::Scanner(string input, SymbolTable* table)
{
    pos = 0;
    line = 1;
    st = table;

    ifstream inputFile(input, ios::in);
    string line;

    if (inputFile.is_open())
    {
        while (getline(inputFile, line))
        {
            this->input.append(line + '\n');
        }
        inputFile.close();
    }
    else
    {
        cout << "Erro ao abrir arquivo de entrada" << endl;
        exit(EXIT_FAILURE);
    }
}

int
Scanner::getLine()
{
    return line;
}

bool
Scanner::isAtEnd()
{
    return pos >= (int) input.length();
}

char
Scanner::currentChar()
{
    if (isAtEnd())
        return '\0';

    return input[pos];
}

char
Scanner::nextChar()
{
    if (pos + 1 >= (int) input.length())
        return '\0';

    return input[pos + 1];
}

void
Scanner::skipSpacesAndComments()
{
    bool repeat = true;

    while (repeat && !isAtEnd())
    {
        repeat = false;

        while (!isAtEnd() && isspace((unsigned char) currentChar()))
        {
            if (currentChar() == '\n')
                line++;
            pos++;
        }

        if (currentChar() == '/' && nextChar() == '/')
        {
            pos += 2;
            while (!isAtEnd() && currentChar() != '\n')
                pos++;
            repeat = true;
        }
        else if (currentChar() == '/' && nextChar() == '*')
        {
            pos += 2;
            while (!isAtEnd() && !(currentChar() == '*' && nextChar() == '/'))
            {
                if (currentChar() == '\n')
                    line++;
                pos++;
            }

            if (isAtEnd())
                lexicalError("comentário de bloco não terminado");

            pos += 2;
            repeat = true;
        }
    }
}

//Método que retorna o próximo token da entrada
Token*
Scanner::nextToken()
{
    Token* tok;
    string lexeme;

    skipSpacesAndComments();

    if (isAtEnd())
        return new Token(END_OF_FILE, "EOF");

    //Trecho que reconhece identificadores e palavras reservadas
    if (isalpha((unsigned char) currentChar()))
    {
        lexeme.push_back(currentChar());
        pos++;

        while (!isAtEnd() && (isalnum((unsigned char) currentChar()) || currentChar() == '_'))
        {
            lexeme.push_back(currentChar());
            pos++;
        }

        STEntry* obj = st->get(lexeme);
        if (!obj)
            tok = new Token(ID, lexeme);
        else
            tok = new Token(obj->token->name, lexeme);

        return tok;
    }

    //Trecho que reconhece constantes inteiras
    if (isdigit((unsigned char) currentChar()))
    {
        lexeme.push_back(currentChar());
        pos++;

        while (!isAtEnd() && isdigit((unsigned char) currentChar()))
        {
            lexeme.push_back(currentChar());
            pos++;
        }

        return new Token(INTEGERCONST, lexeme);
    }

    //Trecho que reconhece constantes de caractere
    if (currentChar() == '\'')
    {
        lexeme.push_back(currentChar());
        pos++;

        if (isAtEnd() || currentChar() == '\n')
            lexicalError("constante de caractere inválida");

        if (currentChar() == '\\')
        {
            lexeme.push_back(currentChar());
            pos++;

            if (currentChar() == 'n' || currentChar() == '0')
            {
                lexeme.push_back(currentChar());
                pos++;
            }
            else
                lexicalError("sequência de escape inválida em constante de caractere");
        }
        else if (isprint((unsigned char) currentChar()) && currentChar() != '\\' && currentChar() != '\'')
        {
            lexeme.push_back(currentChar());
            pos++;
        }
        else
            lexicalError("constante de caractere inválida");

        if (currentChar() != '\'')
            lexicalError("constante de caractere não terminada");

        lexeme.push_back(currentChar());
        pos++;

        return new Token(CHARCONST, lexeme);
    }

    //Trecho que reconhece constantes string
    if (currentChar() == '"')
    {
        lexeme.push_back(currentChar());
        pos++;

        while (!isAtEnd() && currentChar() != '"' && currentChar() != '\n')
        {
            if (!isprint((unsigned char) currentChar()))
                lexicalError("caractere inválido em string");

            lexeme.push_back(currentChar());
            pos++;
        }

        if (isAtEnd() || currentChar() == '\n')
            lexicalError("string não terminada");

        lexeme.push_back(currentChar());
        pos++;

        return new Token(STRINGCONST, lexeme);
    }

    //Operadores e separadores
    switch (currentChar())
    {
        case '+':
            pos++;
            return new Token(PLUS, "+");

        case '-':
            pos++;
            return new Token(MINUS, "-");

        case '*':
            pos++;
            return new Token(MULT, "*");

        case '/':
            pos++;
            return new Token(DIV, "/");

        case '=':
            if (nextChar() == '=')
            {
                pos += 2;
                return new Token(EQUAL, "==");
            }
            pos++;
            return new Token(ASSIGN, "=");

        case '!':
            if (nextChar() == '=')
            {
                pos += 2;
                return new Token(NOTEQUAL, "!=");
            }
            pos++;
            return new Token(NOT, "!");

        case '<':
            if (nextChar() == '=')
            {
                pos += 2;
                return new Token(LESSEQUAL, "<=");
            }
            pos++;
            return new Token(LESS, "<");

        case '>':
            if (nextChar() == '=')
            {
                pos += 2;
                return new Token(GREATEREQUAL, ">=");
            }
            pos++;
            return new Token(GREATER, ">");

        case '&':
            if (nextChar() == '&')
            {
                pos += 2;
                return new Token(AND, "&&");
            }
            lexicalError("operador inválido");
            break;

        case '|':
            if (nextChar() == '|')
            {
                pos += 2;
                return new Token(OR, "||");
            }
            lexicalError("operador inválido");
            break;

        case '(':
            pos++;
            return new Token(LPARENTHESE, "(");

        case ')':
            pos++;
            return new Token(RPARENTHESE, ")");

        case '{':
            pos++;
            return new Token(LBRACE, "{");

        case '}':
            pos++;
            return new Token(RBRACE, "}");

        case '[':
            pos++;
            return new Token(LBRACKET, "[");

        case ']':
            pos++;
            return new Token(RBRACKET, "]");

        case ',':
            pos++;
            return new Token(COMMA, ",");

        case ';':
            pos++;
            return new Token(SEMICOLON, ";");

        default:
            lexeme.push_back(currentChar());
            lexicalError("caractere inválido: " + lexeme);
    }

    return new Token(UNDEF);
}

void
Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;

    exit(EXIT_FAILURE);
}
