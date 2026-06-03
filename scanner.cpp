#include "scanner.h"

/*Construtor que recebe uma string com o nome do arquivo
de entrada e preenche input com seu conteúdo. */
Scanner::Scanner(string input)
{
    /*this->input = input;
    cout << "Entrada: " << input << endl << "Tamanho: "
         << input.length() << endl;*/
    pos = 0;
    line = 1;

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
        cout << "Unable to open file\n";

    // A próxima linha deve ser comentada posteriormente.
    // Ela é utilizada apenas para verificar se o
    // preenchimento de input foi feito corretamente.
    // cout << this->input;
}

int Scanner::getLine()
{
    return line;
}

// Método que retorna o próximo token da entrada
Token *
Scanner::nextToken()
{
    Token *tok;
    string lexeme;
    bool continua = true;

    // Consumir espaços em branco e comentários
    while (continua)
    {
        continua = false;

        while (isspace(input[pos]))
        {
            if (input[pos] == '\n')
                line++;
            pos++;
        }

        if (input[pos] == '/' && input[pos + 1] == '/')
        {
            pos += 2;
            while (input[pos] != '\0' && input[pos] != '\n')
                pos++;
            continua = true;
        }
        else if (input[pos] == '/' && input[pos + 1] == '*')
        {
            pos += 2;

            while (input[pos] != '\0' && !(input[pos] == '*' && input[pos + 1] == '/'))
            {
                if (input[pos] == '\n')
                    line++;
                pos++;
            }

            if (input[pos] == '\0')
                lexicalError("comentario de bloco nao fechado");

            pos += 2;
            continua = true;
        }
    }

    // Fim de arquivo
    if (input[pos] == '\0')
    {
        tok = new Token(END_OF_FILE);
    }
    // Identificadores
    else if (isalpha(input[pos]))
    {
        lexeme.push_back(input[pos]);
        pos++;

        while (isalnum(input[pos]) || input[pos] == '_')
        {
            lexeme.push_back(input[pos]);
            pos++;
        }

        tok = new Token(ID, lexeme);
    }
    // Constantes inteiras
    else if (isdigit(input[pos]))
    {
        lexeme.push_back(input[pos]);
        pos++;

        while (isdigit(input[pos]))
        {
            lexeme.push_back(input[pos]);
            pos++;
        }

        tok = new Token(INTEGERCONST, lexeme);
    }
    // Constantes de caractere
    else if (input[pos] == '\'')
    {
        lexeme.push_back(input[pos]);
        pos++;

        if (input[pos] == '\\')
        {
            lexeme.push_back(input[pos]);
            pos++;

            if (input[pos] == 'n' || input[pos] == '0')
            {
                lexeme.push_back(input[pos]);
                pos++;
            }
            else
                lexicalError("constante de caractere mal formada");
        }
        else if (isprint(input[pos]) && input[pos] != '\\' && input[pos] != '\'')
        {
            lexeme.push_back(input[pos]);
            pos++;
        }
        else
            lexicalError("constante de caractere mal formada");

        if (input[pos] == '\'')
        {
            lexeme.push_back(input[pos]);
            pos++;
            tok = new Token(CHARCONST, lexeme);
        }
        else
            lexicalError("constante de caractere mal formada");
    }
    // Constantes string
    else if (input[pos] == '"')
    {
        lexeme.push_back(input[pos]);
        pos++;

        while (input[pos] != '\0' && input[pos] != '"')
        {
            if (input[pos] == '\n')
                lexicalError("constante string nao fechada");

            if (!isprint(input[pos]))
                lexicalError("constante string mal formada");

            lexeme.push_back(input[pos]);
            pos++;
        }

        if (input[pos] == '\0')
            lexicalError("constante string nao fechada");

        lexeme.push_back(input[pos]);
        pos++;
        tok = new Token(STRINGCONST, lexeme);
    }
    // Operadores compostos
    else if (input[pos] == '=' && input[pos + 1] == '=')
    {
        pos += 2;
        tok = new Token(EQUAL);
    }
    else if (input[pos] == '!' && input[pos + 1] == '=')
    {
        pos += 2;
        tok = new Token(NOTEQUAL);
    }
    else if (input[pos] == '<' && input[pos + 1] == '=')
    {
        pos += 2;
        tok = new Token(LESSEQUAL);
    }
    else if (input[pos] == '>' && input[pos + 1] == '=')
    {
        pos += 2;
        tok = new Token(GREATEREQUAL);
    }
    else if (input[pos] == '&' && input[pos + 1] == '&')
    {
        pos += 2;
        tok = new Token(AND);
    }
    else if (input[pos] == '|' && input[pos + 1] == '|')
    {
        pos += 2;
        tok = new Token(OR);
    }
    // Operadores simples
    else if (input[pos] == '+')
    {
        pos++;
        tok = new Token(PLUS);
    }
    else if (input[pos] == '-')
    {
        pos++;
        tok = new Token(MINUS);
    }
    else if (input[pos] == '*')
    {
        pos++;
        tok = new Token(MULT);
    }
    else if (input[pos] == '/')
    {
        pos++;
        tok = new Token(DIV);
    }
    else if (input[pos] == '=')
    {
        pos++;
        tok = new Token(ASSIGN);
    }
    else if (input[pos] == '<')
    {
        pos++;
        tok = new Token(LESS);
    }
    else if (input[pos] == '>')
    {
        pos++;
        tok = new Token(GREATER);
    }
    else if (input[pos] == '!')
    {
        pos++;
        tok = new Token(NOT);
    }
    // Separadores
    else if (input[pos] == '(')
    {
        pos++;
        tok = new Token(LPAREN);
    }
    else if (input[pos] == ')')
    {
        pos++;
        tok = new Token(RPAREN);
    }
    else if (input[pos] == '{')
    {
        pos++;
        tok = new Token(LBRACE);
    }
    else if (input[pos] == '}')
    {
        pos++;
        tok = new Token(RBRACE);
    }
    else if (input[pos] == '[')
    {
        pos++;
        tok = new Token(LBRACKET);
    }
    else if (input[pos] == ']')
    {
        pos++;
        tok = new Token(RBRACKET);
    }
    else if (input[pos] == ',')
    {
        pos++;
        tok = new Token(COMMA);
    }
    else if (input[pos] == ';')
    {
        pos++;
        tok = new Token(SEMICOLON);
    }
    else
        lexicalError("token mal formado");

    return tok;
}

void Scanner::lexicalError(string msg)
{
    cout << "Linha " << line << ": " << msg << endl;

    exit(EXIT_FAILURE);
}
