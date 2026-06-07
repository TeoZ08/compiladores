# =============================================================
# Script de testes para o compilador C--
# Uso: .\rodar_testes.ps1 [-cmm <caminho_do_executavel>]
# Exemplo: .\rodar_testes.ps1 -cmm ..\cmm.exe
# Padrao: .\cmm.exe (mesmo diretorio)
# =============================================================

param(
    [string]$cmm = ".\cmm.exe"
)

$DIR = Split-Path -Parent $MyInvocation.MyCommand.Path
$PASS = 0
$FAIL = 0
$TOTAL = 0

function Run-Test {
    param(
        [string]$descricao,
        [string]$arquivo,
        [string]$esperado   # "sucesso" ou "erros"
    )

    $script:TOTAL++
    $caminho = Join-Path $DIR $arquivo

    if (-not (Test-Path $caminho)) {
        Write-Host "SKIP: $descricao ($arquivo nao encontrado)" -ForegroundColor Yellow
        return
    }

    $output = & $cmm $caminho 2>&1

    if ($output -match $esperado) {
        Write-Host "PASS: $descricao" -ForegroundColor Green
        $script:PASS++
    } else {
        Write-Host "FAIL: $descricao" -ForegroundColor Red
        Write-Host "      Arquivo  : $arquivo"
        Write-Host "      Esperado : contem '$esperado'"
        Write-Host "      Obtido   : $output"
        $script:FAIL++
    }
}

# =============================================================
Write-Host ""
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " PROGRAMAS VALIDOS (v*)"       -ForegroundColor Cyan
Write-Host "==============================" -ForegroundColor Cyan
# =============================================================

Run-Test "Programa minimo"                        v01_minimo.cmm                    "sucesso"
Run-Test "Funcoes com int e char de retorno"      v02_tipos_retorno.cmm             "sucesso"
Run-Test "Parametros simples"                     v03_parametros.cmm                "sucesso"
Run-Test "Parametro array"                        v04_parametro_array.cmm           "sucesso"
Run-Test "Declaracoes de variaveis"               v05_declaracoes.cmm               "sucesso"
Run-Test "if e if-else"                           v06_if_else.cmm                   "sucesso"
Run-Test "if aninhado (dangling else)"            v07_if_aninhado.cmm               "sucesso"
Run-Test "while"                                  v08_while.cmm                     "sucesso"
Run-Test "for completo"                           v09_for_completo.cmm              "sucesso"
Run-Test "for parcial (partes opcionais)"         v10_for_parcial.cmm               "sucesso"
Run-Test "for vazio (;;)"                         v11_for_vazio.cmm                 "sucesso"
Run-Test "Operadores aritmeticos"                 v12_operadores_aritmeticos.cmm    "sucesso"
Run-Test "Operadores relacionais"                 v13_operadores_relacionais.cmm    "sucesso"
Run-Test "Operadores logicos"                     v14_operadores_logicos.cmm        "sucesso"
Run-Test "Acesso a array"                         v15_array_acesso.cmm              "sucesso"
Run-Test "Chamada de funcao"                      v16_chamada_funcao.cmm            "sucesso"
Run-Test "Sentenca vazia"                         v17_sentenca_vazia.cmm            "sucesso"
Run-Test "Bloco composto"                         v18_bloco_composto.cmm            "sucesso"
Run-Test "Constantes (int, char, escapes)"        v19_constantes.cmm                "sucesso"
Run-Test "Multiplas funcoes"                      v20_multiplas_funcoes.cmm         "sucesso"

# =============================================================
Write-Host ""
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " ERROS LEXICOS (l*)"           -ForegroundColor Cyan
Write-Host "==============================" -ForegroundColor Cyan
# =============================================================

Run-Test "Caractere invalido (@)"                 l01_caractere_invalido.cmm        "erros"
Run-Test "String nao fechada"                     l02_string_nao_fechada.cmm        "erros"
Run-Test "Char nao fechado"                       l03_char_nao_fechado.cmm          "erros"
Run-Test "Escape invalido (\t)"                   l04_escape_invalido.cmm           "erros"
Run-Test "& isolado"                              l05_e_comercial_isolado.cmm       "erros"
Run-Test "| isolado"                              l06_pipe_isolado.cmm              "erros"
Run-Test "Comentario de bloco nao fechado"        l07_comentario_bloco_aberto.cmm   "erros"
Run-Test "Multiplos erros lexicos"                l08_multiplos_erros_lexicos.cmm   "erros"
Run-Test "Erro lexico seguido de erro sintatico"  l09_lexico_e_sintatico.cmm        "erros"

# =============================================================
Write-Host ""
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " ERROS SINTATICOS (s*)"        -ForegroundColor Cyan
Write-Host "==============================" -ForegroundColor Cyan
# =============================================================

Run-Test "Sentenca sem ponto-e-virgula"           s01_sem_ponto_virgula.cmm         "erros"
Run-Test "Sentenca invalida (1x = 5)"             s02_sentenca_invalida.cmm         "erros"
Run-Test "Funcao sem corpo"                       s03_funcao_sem_corpo.cmm          "erros"
Run-Test "Parametro sem nome"                     s04_parametro_sem_nome.cmm        "erros"
Run-Test "if sem parentese"                       s05_if_sem_parentese.cmm          "erros"
Run-Test "return sem ponto-e-virgula"             s06_return_sem_ponto_virgula.cmm  "erros"
Run-Test "Atribuicao sem valor (x = ;)"           s07_atribuicao_sem_valor.cmm      "erros"
Run-Test "Chave nao fechada"                      s08_chave_nao_fechada.cmm         "erros"
Run-Test "Lixo no nivel global"                   s09_lixo_nivel_global.cmm         "erros"
Run-Test "while sem condicao"                     s10_while_sem_condicao.cmm        "erros"
Run-Test "for com virgula ao inves de ;"          s11_for_sem_ponto_virgula.cmm     "erros"
Run-Test "Chamada de funcao sem fechar parentese" s12_chamada_sem_fechar.cmm        "erros"

# =============================================================
Write-Host ""
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " MULTIPLOS ERROS (m*)"         -ForegroundColor Cyan
Write-Host "==============================" -ForegroundColor Cyan
# =============================================================

Run-Test "Dois erros sintaticos seguidos"         m01_dois_erros_sintaticos.cmm     "erros"
Run-Test "Tres erros sintaticos seguidos"         m02_tres_erros_sintaticos.cmm     "erros"
Run-Test "Erros dentro de bloco composto"         m03_erro_em_bloco_composto.cmm    "erros"
Run-Test "Erro nao contamina funcao seguinte"     m04_erro_nao_contamina_funcao.cmm "erros"
Run-Test "Multiplos lexicos e sintatico"          m05_lexico_e_sintatico_multiplos.cmm "erros"
Run-Test "Codigo valido apos sentenca invalida"   m06_valido_apos_erro.cmm          "erros"

# =============================================================
Write-Host ""
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " CASOS DE BORDA (b*)"          -ForegroundColor Cyan
Write-Host "==============================" -ForegroundColor Cyan
# =============================================================

Run-Test "Precedencia de expressoes"              b01_precedencia_expressao.cmm     "sucesso"
Run-Test "Expressao logica complexa"              b02_expressao_logica_complexa.cmm "sucesso"
Run-Test "for com array"                          b03_for_array.cmm                 "sucesso"
Run-Test "Chamada de funcao sem argumentos"       b04_chamada_sem_argumentos.cmm    "sucesso"
Run-Test "Chamada com multiplos argumentos"       b05_chamada_multiplos_args.cmm    "sucesso"
Run-Test "return sem valor (void)"                b06_retorno_sem_valor.cmm         "sucesso"
Run-Test "String como argumento"                  b07_string_como_argumento.cmm     "sucesso"
Run-Test "Aninhamento profundo de estruturas"     b08_aninhamento_profundo.cmm      "sucesso"
Run-Test "Expressao como indice de array"         b09_expressao_como_indice.cmm     "sucesso"
Run-Test "Multiplas declaracoes por linha"        b10_multiplas_declaracoes_linha.cmm "sucesso"

# =============================================================
Write-Host ""
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " RESULTADO FINAL"              -ForegroundColor Cyan
Write-Host "==============================" -ForegroundColor Cyan
Write-Host " Total : $TOTAL"
Write-Host " Pass  : $PASS" -ForegroundColor Green

if ($FAIL -gt 0) {
    Write-Host " Fail  : $FAIL" -ForegroundColor Red
} else {
    Write-Host " Fail  : $FAIL"
}

Write-Host ""
if ($FAIL -eq 0) {
    Write-Host " Todos os testes passaram!" -ForegroundColor Green
} else {
    $perc = [math]::Floor(($PASS * 100) / $TOTAL)
    Write-Host " Aproveitamento: $perc%" -ForegroundColor Yellow
}
Write-Host ""
