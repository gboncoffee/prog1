/* 
 * Tipos Abstratos de Dados - TAD's
 * Arquivo de header para TAD racional.
 * Feito em 15/08/2023 para a disciplina prog1.
*/

/*********** NAO ALTERE ESTE ARQUIVO *********************/

struct racional {
    unsigned short int valido; /* um racional eh invalido se den == 0 */
    int num;                   /* numerador do racional               */
    int den;                   /* denominador do racional             */
};

/* retorna um numero aleatorio entre min e max, inclusive. */
int aleat (int min, int max);

/* Maximo Divisor Comum entre a e b      */
/* calcula o mdc pelo metodo de Euclides */
int mdc (int a, int b);

/* Minimo Multiplo Comum entre a e b */
/* mmc = (a * b) / mdc (a, b)        */
int mmc (int a, int b);

/* Recebe um numero racional e o simplifica. 
 * Por exemplo, ao receber 10/8 devera retornar 5/4
 * Se ambos numerador e denominador forem negativos, devera retornar um positivo 
 * Se o denominador for negativo, o sinal deve migrar para o numerador 
 * Quem chama esta funcao deve garantir que o racional r eh valido */
struct racional simplifica_r (struct racional r);

/* Cria um numero racional com base nas informacoes dos parametros. */
struct racional cria_r (int numerador, int denominador);

/* Retorna um numero racional aleatorio na forma simplificada.
 * O numerador e o denominador devem ser inteiros entre 0 e n.
 * O racional deve conter a informacao se ele eh valido ou nao */
struct racional sorteia_r (int n);

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
 
/* Retorna 1 se o racional r eh valido ou 0 caso contrario
 * Um racional eh invalido se o denominador for nulo */
int valido_r (struct racional r);

/* Retorna a soma dos racionais r1 e r2.
 * Quem chama esta funcao deve garantir que os racionais r1 e r2 sao validos */
struct racional soma_r (struct racional r1, struct racional r2);

/* Retorna a subtracao dos racionais r1 e r2.
 * Quem chama esta funcao deve garantir que os racionais r1 e r2 sao validos */
struct racional subtrai_r (struct racional r1, struct racional r2);

/* Retorna a multiplicacao dos racionais r1 e r2.
 * Quem chama esta funcao deve garantir que os racionais r1 e r2 sao validos */
struct racional multiplica_r (struct racional r1, struct racional r2);

/* Retorna a divisao dos racionais r1 e r2.
 * Quem chama esta funcao deve:
     - garantir que os racionais r1 e r2 sao validos
     - em seguida deve garantir que o racional retornado tambem seja valido */
struct racional divide_r (struct racional r1, struct racional r2);
