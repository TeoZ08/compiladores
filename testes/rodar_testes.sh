#!/bin/bash

# =============================================================
# Script de testes para o compilador C--
# Uso: ./rodar_testes.sh [caminho_do_executavel]
# Exemplo: ./rodar_testes.sh ../cmm
# Padrão: ./cmm (mesmo diretório)
# =============================================================

CMM=${1:-./cmm}
DIR="$(cd "$(dirname "$0")" && pwd)"

PASS=0
FAIL=0
TOTAL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'

run_test() {
    local descricao=$1
    local arquivo=$2
    local esperado=$3   # "sucesso" ou "erros"

    TOTAL=$((TOTAL + 1))

    if [ ! -f "$DIR/$arquivo" ]; then
        echo -e "${YELLOW}SKIP${NC}: $descricao ($arquivo não encontrado)"
        return
    fi

    output=$("$CMM" "$DIR/$arquivo" 2>&1)

    if echo "$output" | grep -q "$esperado"; then
        echo -e "${GREEN}PASS${NC}: $descricao"
        PASS=$((PASS + 1))
    else
        echo -e "${RED}FAIL${NC}: $descricao"
        echo    "       Arquivo  : $arquivo"
        echo    "       Esperado : contém '$esperado'"
        echo    "       Obtido   : $output"
        FAIL=$((FAIL + 1))
    fi
}

# =============================================================
echo -e "\n${CYAN}==============================${NC}"
echo -e "${CYAN} PROGRAMAS VÁLIDOS (v*)${NC}"
echo -e "${CYAN}==============================${NC}"
# =============================================================

run_test "Programa mínimo"                        v01_minimo.cmm                 "sucesso"
run_test "Funções com int e char de retorno"       v02_tipos_retorno.cmm          "sucesso"
run_test "Parâmetros simples"                      v03_parametros.cmm             "sucesso"
run_test "Parâmetro array"                         v04_parametro_array.cmm        "sucesso"
run_test "Declarações de variáveis"               v05_declaracoes.cmm            "sucesso"
run_test "if e if-else"                            v06_if_else.cmm                "sucesso"
run_test "if aninhado (dangling else)"             v07_if_aninhado.cmm            "sucesso"
run_test "while"                                   v08_while.cmm                  "sucesso"
run_test "for completo"                            v09_for_completo.cmm           "sucesso"
run_test "for parcial (partes opcionais)"          v10_for_parcial.cmm            "sucesso"
run_test "for vazio (;;)"                          v11_for_vazio.cmm              "sucesso"
run_test "Operadores aritméticos"                 v12_operadores_aritmeticos.cmm "sucesso"
run_test "Operadores relacionais"                  v13_operadores_relacionais.cmm "sucesso"
run_test "Operadores lógicos"                     v14_operadores_logicos.cmm     "sucesso"
run_test "Acesso a array"                          v15_array_acesso.cmm           "sucesso"
run_test "Chamada de função"                      v16_chamada_funcao.cmm         "sucesso"
run_test "Sentença vazia"                         v17_sentenca_vazia.cmm         "sucesso"
run_test "Bloco composto"                          v18_bloco_composto.cmm         "sucesso"
run_test "Constantes (int, char, escapes)"         v19_constantes.cmm             "sucesso"
run_test "Múltiplas funções"                      v20_multiplas_funcoes.cmm      "sucesso"

# =============================================================
echo -e "\n${CYAN}==============================${NC}"
echo -e "${CYAN} ERROS LÉXICOS (l*)${NC}"
echo -e "${CYAN}==============================${NC}"
# =============================================================

run_test "Caractere inválido (@)"                  l01_caractere_invalido.cmm     "erros"
run_test "String não fechada"                     l02_string_nao_fechada.cmm     "erros"
run_test "Char não fechado"                       l03_char_nao_fechado.cmm       "erros"
run_test "Escape inválido (\\t)"                   l04_escape_invalido.cmm        "erros"
run_test "& isolado"                               l05_e_comercial_isolado.cmm    "erros"
run_test "| isolado"                               l06_pipe_isolado.cmm           "erros"
run_test "Comentário de bloco não fechado"        l07_comentario_bloco_aberto.cmm "erros"
run_test "Múltiplos erros léxicos"               l08_multiplos_erros_lexicos.cmm "erros"
run_test "Erro léxico seguido de erro sintático"  l09_lexico_e_sintatico.cmm     "erros"

# =============================================================
echo -e "\n${CYAN}==============================${NC}"
echo -e "${CYAN} ERROS SINTÁTICOS (s*)${NC}"
echo -e "${CYAN}==============================${NC}"
# =============================================================

run_test "Sentença sem ponto-e-vírgula"           s01_sem_ponto_virgula.cmm      "erros"
run_test "Sentença inválida (1x = 5)"              s02_sentenca_invalida.cmm      "erros"
run_test "Função sem corpo"                        s03_funcao_sem_corpo.cmm       "erros"
run_test "Parâmetro sem nome"                     s04_parametro_sem_nome.cmm     "erros"
run_test "if sem parêntese"                       s05_if_sem_parentese.cmm       "erros"
run_test "return sem ponto-e-vírgula"             s06_return_sem_ponto_virgula.cmm "erros"
run_test "Atribuição sem valor (x = ;)"           s07_atribuicao_sem_valor.cmm   "erros"
run_test "Chave não fechada"                      s08_chave_nao_fechada.cmm      "erros"
run_test "Lixo no nível global"                   s09_lixo_nivel_global.cmm      "erros"
run_test "while sem condição"                     s10_while_sem_condicao.cmm     "erros"
run_test "for com vírgula ao invés de ;"          s11_for_sem_ponto_virgula.cmm  "erros"
run_test "Chamada de função sem fechar parêntese" s12_chamada_sem_fechar.cmm     "erros"

# =============================================================
echo -e "\n${CYAN}==============================${NC}"
echo -e "${CYAN} MÚLTIPLOS ERROS (m*)${NC}"
echo -e "${CYAN}==============================${NC}"
# =============================================================

run_test "Dois erros sintáticos seguidos"          m01_dois_erros_sintaticos.cmm  "erros"
run_test "Três erros sintáticos seguidos"          m02_tres_erros_sintaticos.cmm  "erros"
run_test "Erros dentro de bloco composto"          m03_erro_em_bloco_composto.cmm "erros"
run_test "Erro não contamina função seguinte"      m04_erro_nao_contamina_funcao.cmm "erros"
run_test "Múltiplos léxicos e sintático"          m05_lexico_e_sintatico_multiplos.cmm "erros"
run_test "Código válido após sentença inválida"    m06_valido_apos_erro.cmm       "erros"

# =============================================================
echo -e "\n${CYAN}==============================${NC}"
echo -e "${CYAN} CASOS DE BORDA (b*)${NC}"
echo -e "${CYAN}==============================${NC}"
# =============================================================

run_test "Precedência de expressões"               b01_precedencia_expressao.cmm  "sucesso"
run_test "Expressão lógica complexa"              b02_expressao_logica_complexa.cmm "sucesso"
run_test "for com array"                           b03_for_array.cmm              "sucesso"
run_test "Chamada de função sem argumentos"        b04_chamada_sem_argumentos.cmm "sucesso"
run_test "Chamada com múltiplos argumentos"       b05_chamada_multiplos_args.cmm "sucesso"
run_test "return sem valor (void)"                 b06_retorno_sem_valor.cmm      "sucesso"
run_test "String como argumento"                   b07_string_como_argumento.cmm  "sucesso"
run_test "Aninhamento profundo de estruturas"      b08_aninhamento_profundo.cmm   "sucesso"
run_test "Expressão como índice de array"          b09_expressao_como_indice.cmm  "sucesso"
run_test "Múltiplas declarações por linha"        b10_multiplas_declaracoes_linha.cmm "sucesso"

# =============================================================
echo -e "\n${CYAN}=============================="
echo -e " RESULTADO FINAL"
echo -e "==============================${NC}"
echo -e " Total : $TOTAL"
echo -e "${GREEN} Pass  : $PASS${NC}"
if [ $FAIL -gt 0 ]; then
    echo -e "${RED} Fail  : $FAIL${NC}"
else
    echo -e " Fail  : $FAIL"
fi
echo ""
if [ $FAIL -eq 0 ]; then
    echo -e "${GREEN} Todos os testes passaram!${NC}"
else
    PERC=$(( (PASS * 100) / TOTAL ))
    echo -e "${YELLOW} Aproveitamento: $PERC%${NC}"
fi
echo ""
