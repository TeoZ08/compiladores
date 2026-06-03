#include "parser.h"

Parser::Parser(string input)
{
    currentST = globalST = new SymbolTable();
    initSimbolTable();

    scanner = new Scanner(input, globalST);
    lToken = 0;
    errors = 0;
}

void
Parser::advance()
{
    lToken = scanner->nextToken();
}

void
Parser::match(int t)
{
    if (lToken->name == t || lToken->attribute == t)
        advance();
    else
        error("esperado " + tokenName(t) + ", encontrado " + lookahead());
}

void
Parser::run()
{
    advance();

    program();

    if (errors == 0)
        cout << "Compilação encerrada com sucesso!" << endl;
    else
        cout << "Compilação encerrada com erros." << endl;
}

void
Parser::program()
{
    while (lToken->name != END_OF_FILE)
    {
        if (isType(lToken->name) || lToken->name == VOID)
            function();
        else
        {
            error("declaração de função esperada");
            advance();
        }
    }

    match(END_OF_FILE);
}

void
Parser::function()
{
    if (isType(lToken->name))
        type();
    else if (lToken->name == VOID)
        match(VOID);
    else
        error("tipo de retorno esperado");

    match(ID);
    match(LPARENTHESE);
    paramTypes();
    match(RPARENTHESE);
    match(LBRACE);

    declarationList();
    statementList();

    while (lToken->name != RBRACE && lToken->name != END_OF_FILE)
    {
        error("sentença inválida no corpo da função");
        advance();
    }

    match(RBRACE);
}

void
Parser::type()
{
    if (lToken->name == INT)
        match(INT);
    else if (lToken->name == CHAR)
        match(CHAR);
    else
        error("tipo esperado");
}

void
Parser::paramTypes()
{
    if (lToken->name == VOID)
    {
        match(VOID);
        return;
    }

    if (isType(lToken->name))
    {
        type();
        match(ID);
        paramRest();

        while (lToken->name == COMMA)
        {
            match(COMMA);
            type();
            match(ID);
            paramRest();
        }
    }
    else
        error("lista de parâmetros esperada");
}

void
Parser::paramRest()
{
    if (lToken->name == LBRACKET)
    {
        match(LBRACKET);
        match(RBRACKET);
    }
}

void
Parser::declarationList()
{
    while (isType(lToken->name))
    {
        type();
        varDeclaration();

        while (lToken->name == COMMA)
        {
            match(COMMA);
            varDeclaration();
        }

        match(SEMICOLON);
    }
}

void
Parser::varDeclaration()
{
    match(ID);

    if (lToken->name == LBRACKET)
    {
        match(LBRACKET);
        match(INTEGERCONST);
        match(RBRACKET);
    }
}

void
Parser::statementList()
{
    while (isStatementStart(lToken->name))
        statement();
}

void
Parser::statement()
{
    if (lToken->name == IF)
    {
        match(IF);
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
        statement();

        if (lToken->name == ELSE)
        {
            match(ELSE);
            statement();
        }
    }
    else if (lToken->name == WHILE)
    {
        match(WHILE);
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
        statement();
    }
    else if (lToken->name == FOR)
    {
        match(FOR);
        match(LPARENTHESE);

        if (lToken->name == ID)
            assign();
        match(SEMICOLON);

        if (isExpressionStart(lToken->name))
            expression();
        match(SEMICOLON);

        if (lToken->name == ID)
            assign();
        match(RPARENTHESE);
        statement();
    }
    else if (lToken->name == RETURN)
    {
        match(RETURN);
        if (isExpressionStart(lToken->name))
            expression();
        match(SEMICOLON);
    }
    else if (lToken->name == ID)
    {
        match(ID);

        if (lToken->name == LPARENTHESE)
            callRest();
        else
            assignRest();

        match(SEMICOLON);
    }
    else if (lToken->name == LBRACE)
        compoundStmt();
    else if (lToken->name == SEMICOLON)
        match(SEMICOLON);
    else
    {
        error("sentença esperada");
        if (lToken->name != END_OF_FILE)
            advance();
    }
}

void
Parser::compoundStmt()
{
    match(LBRACE);

    while (isStatementStart(lToken->name))
        statement();

    while (lToken->name != RBRACE && lToken->name != END_OF_FILE)
    {
        error("sentença inválida no bloco");
        advance();
    }

    match(RBRACE);
}

void
Parser::assign()
{
    match(ID);
    assignRest();
}

void
Parser::assignRest()
{
    if (lToken->name == LBRACKET)
    {
        match(LBRACKET);
        expression();
        match(RBRACKET);
    }

    match(ASSIGN);
    expression();
}

void
Parser::callRest()
{
    match(LPARENTHESE);

    if (isExpressionStart(lToken->name))
        expressionList();

    match(RPARENTHESE);
}

void
Parser::expressionList()
{
    expression();

    while (lToken->name == COMMA)
    {
        match(COMMA);
        expression();
    }
}

void
Parser::expression()
{
    if (!isExpressionStart(lToken->name))
    {
        error("expressão esperada");
        return;
    }

    logOr();
}

void
Parser::logOr()
{
    logAnd();

    while (lToken->name == OR)
    {
        match(OR);
        logAnd();
    }
}

void
Parser::logAnd()
{
    rel();

    while (lToken->name == AND)
    {
        match(AND);
        rel();
    }
}

void
Parser::rel()
{
    add();

    while (isRelOp(lToken->name))
    {
        advance();
        add();
    }
}

void
Parser::add()
{
    mult();

    while (isAddOp(lToken->name))
    {
        advance();
        mult();
    }
}

void
Parser::mult()
{
    unary();

    while (isMulOp(lToken->name))
    {
        advance();
        unary();
    }
}

void
Parser::unary()
{
    if (lToken->name == MINUS || lToken->name == NOT)
    {
        advance();
        unary();
    }
    else
        primary();
}

void
Parser::primary()
{
    if (lToken->name == ID)
    {
        match(ID);

        if (lToken->name == LPARENTHESE)
        {
            match(LPARENTHESE);
            if (isExpressionStart(lToken->name))
                expressionList();
            match(RPARENTHESE);
        }
        else if (lToken->name == LBRACKET)
        {
            match(LBRACKET);
            expression();
            match(RBRACKET);
        }
    }
    else if (lToken->name == LPARENTHESE)
    {
        match(LPARENTHESE);
        expression();
        match(RPARENTHESE);
    }
    else if (lToken->name == INTEGERCONST)
        match(INTEGERCONST);
    else if (lToken->name == CHARCONST)
        match(CHARCONST);
    else if (lToken->name == STRINGCONST)
        match(STRINGCONST);
    else
    {
        error("expressão esperada");
        if (!isSyncToken(lToken->name) && lToken->name != END_OF_FILE)
            advance();
    }
}

bool
Parser::isType(int t)
{
    return t == INT || t == CHAR;
}

bool
Parser::isStatementStart(int t)
{
    return t == IF || t == WHILE || t == FOR || t == RETURN ||
           t == ID || t == LBRACE || t == SEMICOLON;
}

bool
Parser::isExpressionStart(int t)
{
    return t == ID || t == INTEGERCONST || t == CHARCONST ||
           t == STRINGCONST || t == LPARENTHESE || t == MINUS || t == NOT;
}

bool
Parser::isRelOp(int t)
{
    return t == EQUAL || t == NOTEQUAL || t == LESSEQUAL ||
           t == LESS || t == GREATEREQUAL || t == GREATER;
}

bool
Parser::isAddOp(int t)
{
    return t == PLUS || t == MINUS;
}

bool
Parser::isMulOp(int t)
{
    return t == MULT || t == DIV;
}

bool
Parser::isSyncToken(int t)
{
    return t == SEMICOLON || t == RPARENTHESE || t == RBRACE ||
           t == RBRACKET || t == COMMA;
}

void
Parser::initSimbolTable()
{
    Token* t;

    t = new Token(INT, "int");
    globalST->add(new STEntry(t, true));

    t = new Token(CHAR, "char");
    globalST->add(new STEntry(t, true));

    t = new Token(VOID, "void");
    globalST->add(new STEntry(t, true));

    t = new Token(IF, "if");
    globalST->add(new STEntry(t, true));

    t = new Token(ELSE, "else");
    globalST->add(new STEntry(t, true));

    t = new Token(WHILE, "while");
    globalST->add(new STEntry(t, true));

    t = new Token(FOR, "for");
    globalST->add(new STEntry(t, true));

    t = new Token(RETURN, "return");
    globalST->add(new STEntry(t, true));
}

string
Parser::lookahead()
{
    if (lToken->lexeme != "")
        return "'" + lToken->lexeme + "'";

    return tokenName(lToken->name);
}

string
Parser::tokenName(int t)
{
    switch (t)
    {
        case ID: return "identificador";
        case INT: return "'int'";
        case CHAR: return "'char'";
        case VOID: return "'void'";
        case IF: return "'if'";
        case ELSE: return "'else'";
        case WHILE: return "'while'";
        case FOR: return "'for'";
        case RETURN: return "'return'";
        case INTEGERCONST: return "constante inteira";
        case CHARCONST: return "constante de caractere";
        case STRINGCONST: return "constante string";
        case PLUS: return "'+'";
        case MINUS: return "'-'";
        case MULT: return "'*'";
        case DIV: return "'/'";
        case ASSIGN: return "'='";
        case EQUAL: return "'=='";
        case NOTEQUAL: return "'!='";
        case LESSEQUAL: return "'<='";
        case LESS: return "'<'";
        case GREATEREQUAL: return "'>='";
        case GREATER: return "'>'";
        case AND: return "'&&'";
        case OR: return "'||'";
        case NOT: return "'!'";
        case LPARENTHESE: return "'('";
        case RPARENTHESE: return "')'";
        case LBRACE: return "'{'";
        case RBRACE: return "'}'";
        case LBRACKET: return "'['";
        case RBRACKET: return "']'";
        case COMMA: return "','";
        case SEMICOLON: return "';'";
        case END_OF_FILE: return "fim de arquivo";
        default: return "token inválido";
    }
}

void
Parser::error(string str)
{
    cout << "Linha " << scanner->getLine() << ": " << str << endl;
    errors++;
}
