#ifndef _RACIONAIS_H
#define _RACIONAIS_H

/* 
 * Tipos Abstratos de Dados - TAD's
 * Arquivo de header para TAD racional.
 * Feito em 17/08/2023 para a disciplina prog1.
*/

/*********** NAO ALTERE ESTE ARQUIVO *********************/

struct racional {
    long int num;                   /* numerador do racional */
    long int den;                   /* denominador do racional */
};

/* Cria um numero racional simplificado com base nas informacoes
 * dos parametros e retorna um ponteiro que aponta para ele.
 * Retorna NULL se nao conseguiu alocar a memoria. */
struct racional *cria_r (long int numerador, long int denominador);

/* Libera o espaco alocado para o racional */
void destroi_r (struct racional *r);

/* Retorna o numerador de um racional ou zero se r for nulo */
long int numerador_r (struct racional *r);

/* Retorna o denominador de um racional ou zero se r for nulo */
long int denominador_r (struct racional *r);

/* Retorna 1 se o racional r eh valido ou 0 se for inválido
 * Um racional eh invalido se o denominador for zero ou
 * se ele não tiver sido alocado. */
int valido_r (struct racional *r);

/* Imprime um racional r, respeitando estas regras:
   - o racional deve estar na forma simplificada;
   - nao use espacos em branco e nao mude de linha;
   - o formato de saida deve ser "num/den", a menos dos casos abaixo;
     - se o ponteiro for nulo, deve imprimir a mensagem "NULO";
     - se o racional for invalido, deve imprimir a mensagem "INVALIDO";
     - se o numerador for 0, deve imprimir somente "0";
     - se o denominador for 1, deve imprimir somente o numerador;
     - se o numerador e denominador forem iguais, deve imprimir somente "1";
     - se o racional for negativo, o sinal deve ser impresso antes do numero;
     - se numerador e denominador forem negativos, o racional eh positivo. */
void imprime_r (struct racional *r);

/* Compara dois números racionais r1 e r2.
 * Retorna -2 se r1 ou r2 for invalido ou se o respectivo ponteiro for nulo.
 * Retorna -1 se r1 < r2; 0 se r1 = r2; 1 se r1 > r2.
 * Atencao: faca a comparacao normalizando os denominadores pelo MMC.
 * Fazer a comparacao baseado na divisao de numerador pelo denominador
 * pode resultar em erro numerico e falsear o teste. */
int compara_r (struct racional *r1, struct racional *r2);

/* Simplifica o número racional indicado no parâmetro.
 * Por exemplo, se o número for 10/8 devera mudar para 5/4.
 * Retorna 1 em sucesso e 0 se r for inválido ou o ponteiro for nulo.
 * Se ambos numerador e denominador forem negativos, o resultado é positivo.
 * Se o denominador for negativo, o sinal deve migrar para o numerador. */
int simplifica_r (struct racional *r);

/* Coloca em *r3 a soma simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for nulo ou inválido. */
int soma_r (struct racional *r1, struct racional *r2, struct racional *r3);

/* Coloca em *r3 a diferença simplificada dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for nulo ou inválido. */
int subtrai_r (struct racional *r1, struct racional *r2, struct racional *r3);

/* Coloca em *r3 o produto simplificado dos racionais *r1 e *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for nulo ou inválido. */
int multiplica_r (struct racional *r1, struct racional *r, struct racional *r32);

/* Coloca em *r3 a divisão simplificada do racional *r1 por *r2.
 * Retorna 1 em sucesso e 0 se r1 ou r2 for nulo ou inválido. */
int divide_r (struct racional *r1, struct racional *r2, struct racional *r3);

#endif

