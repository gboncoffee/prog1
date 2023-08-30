#ifndef _LIBpilha_t_H
#define _LIBpilha_t_H

/* 
 * Tipos Abstratos de Dados - TAD's
 * Arquivo de header para TAD racional.
 * Feito em 17/08/2023 para a disciplina prog1.
*/

/*********** NAO ALTERE ESTE ARQUIVO *********************/

struct racional {
    unsigned short int valido; /* um racional eh invalido se den == 0 */
    int num;                   /* numerador do racional               */
    int den;                   /* denominador do racional             */
};

/* implemente as tres funcoes abaixo como funcoes internas no seu racionais.c */
/* elas nao devem fazer parte da interface do TAD racionais.                  */
/* int aleat (int min, int max); */
/* int mdc (int a, int b);       */
/* int mmc (int a, int b);       */

/* Cria um numero racional com base nas informacoes dos parametros. */
struct racional cria_r (int numerador, int denominador);

/* Retorna um numero racional aleatorio na forma simplificada.
 * O numerador e o denominador devem ser inteiros entre -max e max.
 * O racional deve conter a informacao se ele eh valido ou nao. */
struct racional sorteia_r (int max);

/* Retorna o numerador de um racional */
int numerador_r (struct racional r);

/* Retorna o denominador de um racional */
int denominador_r (struct racional r);

/* Retorna 1 se o racional r eh valido ou 0 caso contrario
 * Um racional eh invalido se o denominador for nulo */
int valido_r (struct racional r);

/* Imprime um racional r
   A impressao deve respeitar o seguinte:
   - o racional deve estar na forma simplificada;
   - o formato de saida deve ser "r.num/r.den", a menos dos casos abaixo;
   - nao use espacos em branco apos o numero e nao mude de linha;
   - se o numerador for igual a 0, deve ser impresso somente zero;
   - se o denominador for igual a 1, deve ser impresso somente o numerador;
   - se o numerador e denominador forem o mesmo valor, deve imprimir somente 1;
   - se o racional for negativo, o sinal deve ser impresso antes do numero;
   - se ambos numerador e denominador forem negativos, o racional eh positivo;
   - se o racional for invalido, deve imprimir a mensagem "INVALIDO"           */
void imprime_r (struct racional r);
 
/* Retorna -1 se r1 < r2; 0 se r1 = r2; 1 se r1 > r2 */
int compara_r (struct racional r1, struct racional r2);

/* Recebe um numero racional no parametro e o simplifica. 
 * Retorna 1 caso a operacao seja bem sucedida e 0 caso o racional seja invalido.
 * Por exemplo, ao receber 10/8 devera retornar 5/4
 * Se ambos numerador e denominador forem negativos, devera retornar um positivo 
 * Se o denominador for negativo, o sinal deve migrar para o numerador 
 * Quem chama esta funcao deve garantir que o racional r eh valido */
int simplifica_r (struct racional *r);

/* Retorna a soma dos racionais r1 e r2 no parametro *r3 e 
 * Retorna 1 se a operacao foi bem sucedida ou 0 caso algum dentre
 * r1, r2, r3 forem invalidos */
int soma_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna a subtracao dos racionais r1 e r2 no parametro *r3 e
 * Retorna 1 se a operacao foi bem sucedida ou 0 caso algum dentre
 * r1, r2, r3 forem invalidos */
int subtrai_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna a multiplicacao dos racionais r1 e r2 no parametro *r3 e
 * Retorna 1 se a operacao foi bem sucedida ou 0 caso algum dentre
 * r1, r2, r3 forem invalidos */
int multiplica_r (struct racional r1, struct racional r2, struct racional *r3);

/* Retorna a divisao dos racionais r1 e r2 no parametro *r3 e
 * Retorna 1 se a operacao foi bem sucedida ou 0 caso algum dentre
 * r1, r2, r3 forem invalidos */
int divide_r (struct racional r1, struct racional r2, struct racional *r3);

#endif

