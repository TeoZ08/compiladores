# Compilador C--

Trabalho Prático — Compiladores I (2026/1)  

## Grupo

- Heitor Arruda da Silva
- Matteo Lima Scotti
- Wellington Cintra da Silva

## Descrição

Implementação de um compilador para a linguagem C--, subconjunto da linguagem C, desenvolvido em C++. O compilador contempla as três etapas do trabalho prático:

- **Etapa 1 — Análise Léxica:** reconhecimento de todos os tokens da linguagem C--, tratamento de erros léxicos com continuação da análise.
- **Etapa 2 — Análise Sintática:** parser descendente preditivo (LL(1)) com recuperação de erros por modo pânico.
- **Etapa 3 — Tabela de Símbolos:** diferenciação de identificadores e palavras reservadas via tabela de símbolos pré-carregada.

## Arquivos

| Arquivo | Descrição |
|---|---|
| `principal.cpp` | Ponto de entrada do programa |
| `scanner.h / scanner.cpp` | Analisador léxico |
| `parser.h / parser.cpp` | Analisador sintático |
| `token.h` | Definição dos tokens |
| `symboltable.h / symboltable.cpp` | Tabela de símbolos |
| `stentry.h / stentry.cpp` | Entrada da tabela de símbolos |

## Compilação

A compilação segue exatamente o comando especificado na descrição do trabalho:

```bash
g++ *.cpp -o cmm
```

Nenhum passo adicional é necessário.

## Execução

```bash
./cmm arquivo.cmm
```

Onde `arquivo.cmm` é o caminho para o arquivo fonte a ser compilado.

## Saída

O compilador emite mensagens de erro no terminal informando a linha de ocorrência. Ao final, informa o resultado da compilação:

- `Compilação encerrada com sucesso!` — nenhum erro encontrado.
- `Compilação encerrada com erros.` — um ou mais erros foram detectados.

Exemplo de saída com erro:

```
Linha 3: sentença inválida no corpo da função
Compilação encerrada com erros.
```
