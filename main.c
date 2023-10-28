#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#define EPSILON 0.000001


/*
* make_matrix aloca uma matriz com l linhas e c colunas em m.
* caso haja falha na alocação da matriz, será desalocado todo o espaço de memória alocado e retornado 1.
*/
double** make_matrix(int l, int c) {
	int i, j;
    double **m;
	
	m = malloc(sizeof(double*) * l);
  	if (m == NULL) {
    	return NULL;
  	}
  	
  	for (i = 0; i < l; i++) {
  		m[i] = (double*)malloc(sizeof(double) * c);
	  	if (m[i] == NULL) {
			for (j = 0; j <= i; j++) {
		  		free(m[j]);
			}

			free(m);

			return NULL;
	  	}
	}
	
	return m;
}

/*
* read_matrix lê uma 
*/
void read_matrix(double** m, int l, int c) {
	int code;
	int i, j;
	
	for (i = 0; i < l; i++) {
		for (j = 0; j < c; j++) {
			printf("m[%d][%d] ", i+1, j+1);
			
			scanf("%lf", &m[i][j]);
		}
	}
}

void free_matrix(double** m, int l) {
	int i;
	
	for (i = 0; i < l; i++) {
  		free(m[i]);
	}
	
	free(m);
}

void print_matrix(double **m, int l, int c) {
	int i, j;
	for (i = 0; i < l; i++) {
	  	for (j = 0; j < c; j++) {
			printf("%10.3lf,", m[i][j]);
		}
		
		printf("\n");
	}
}

int linear_system() {
    char c;
    int n, i, j, k;
    double **matrix;
    char file_name[100];
    FILE *file;


    printf("Qual o nome do arquivo que contem o sistema linear? ");

    if(fgets(file_name, 100, stdin) == NULL) {
      printf("falha ao ler entrada");

      return 3;
    }

    i = 0;
    while (file_name[i] != '\n' && file_name[i] != '\0') i++;

    if (file_name[i] == '\n')
        file_name[i] = '\0';

    file = fopen(file_name, "r");
    if (!file) {
        printf("Erro na abertura do arquivo %s. Fim de programa.", file_name);

        return 2;
    }

    fscanf(file,  "%d", &n);

    matrix = make_matrix(n, n+1);
    if (matrix == NULL) {
        return 1;
    }

    for (i = 0; i < n; i++){
        for (j = 0; j < n+1; j++){
            fscanf(file, "%lf", &matrix[i][j]);
        }
    }

    int *xs_positions;
    xs_positions = malloc(sizeof(double) * n);
    if (xs_positions == NULL) {
        return 1;
    }

    for (i = 0; i < n; i++) {
        xs_positions[i] = i;
    }

    double *buff;
    buff = malloc(sizeof(double) * n);
    if (buff == NULL) {
        return 1;
    }

    for (i = 0; i < n; i++) {
        // se pivo nulo, tentar trocar coluna de coeficientes i a direita que tenha um valor não nulo na linha i
        if (abs(matrix[i][i]) < EPSILON) {
            // busca pela coluna
            j = i+1;
            while (j < n && abs(matrix[i][j]) < EPSILON) j++;

            // caso coluna não seja encontrada, verificar se o sistema linear é incompatível
            if (j == n) {
                double b = matrix[i][n];

                if (abs(b) > EPSILON) {
                    printf("O sistema linear é incompatível");

                    return 0;
                }

                // se a varável for lívre, zerar toda a coluna e considerar a variável referente a coluna zero
                for (k = 0; k < n; k++) {
                    matrix[k][i] = 0;
                }

                continue;
            }

            xs_positions[i] = j;
            xs_positions[j] = i;

            for (k = 0; k < n; k++) {
                buff[k] = matrix[k][i];
            }
            for (k = 0; k < n; k++) {
                matrix[k][i] = matrix[k][j];
            }
            for (k = 0; k < n; k++) {
                matrix[k][j] = buff[k];
            }
        }

        // pivotar se possível
        if (abs(matrix[i][i]) > EPSILON) {
            for (j = 0; j < n; j++) {
                if (j == i) {
                    continue;
                }

                double c = -matrix[j][i]/matrix[i][i];
                for (k = 0; k <= n; k++) {
                    matrix[j][k] += matrix[i][k] * c;
                }
            }
        }
    }

    print_matrix(matrix, n, n+1);

    for (i = 0; i < n; i++) {
        if (matrix[i][i] == 0) {
            printf("x(%d) = %d (variavel livre)\n",xs_positions[i], 0);
            continue;
        }

        printf("x(%d) = %lf\n", xs_positions[i], matrix[i][n] / matrix[i][i]);
    }

    free_matrix(matrix, n);

    return 0;
}


char toPrintChar(char c) {
    if (c == 0) {
        return '0';
    }

    if (c == 1) {
        return '1';
    }

    if (c == 2) {
        return '2';
    }

    if (c == 3) {
        return '3';
    }

    if (c == 4) {
        return '4';
    }

    if (c == 5) {
        return '5';
    }

    if (c == 6) {
        return '6';
    }

    if (c == 7) {
        return '7';
    }

    if (c == 8) {
        return '8';
    }

    if (c == 9) {
        return '9';
    }

    if (c == 10) {
        return 'A';
    }

    if (c == 11) {
        return 'B';
    }

    if (c == 12) {
        return 'C';
    }

    if (c == 13) {
        return 'D';
    }

    if (c == 14) {
        return 'E';
    }

    if (c == 15) {
        return 'F';
    }

    printf("<%d>", c);

    return 'Z';
}

const int MAX_RES_FRACTION = 10;

void baseConversion(int base, double num) {
    int integer = floor(num);
    double fraction = num - integer;

    int i;

    int int_len;
    for (int_len = 0; pow(base, int_len) < integer; int_len++) {}

    char *res_integer = malloc(sizeof(char) * int_len);
    for (i = 0; i < 0; i++) {
        res_integer[i] = 0;
    }

    char res_fraction[MAX_RES_FRACTION];
    for (i = 0; i < MAX_RES_FRACTION; i++) {
        res_fraction[i] = 0;
    }

    for(i = int_len-1; integer >= base; i--) {
        res_integer[i] = integer%base;
        integer = (integer-res_integer[i]) / base;
    }
    res_integer[0] = integer;

    for(i = 0; fraction > 0 && i < MAX_RES_FRACTION; i++){
      fraction *= base;
      integer = floor(fraction);

      res_fraction[i] = integer;

      fraction -= integer;
    }

    printf("base %d = ", base);

    for (i = 0; i < int_len; i++) {
        printf("%c", toPrintChar(res_integer[i]));
    }
    printf(",");
    for (i = 0; i < MAX_RES_FRACTION; i++) {
        printf("%c", toPrintChar(res_fraction[i]));
    }

    printf("\n");

    free(res_integer);
}

void cleanBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/*
main retorna os seguintes códigos de encerramento:
0 - sucesso
1 - memória insuficiente
2 - entrada inválida
3 - falha na leitura da entrada
4 - não implementado
*/
int main() {
    char option;

    printf("Escolha uma opção de operação:\n\n('C') - Conversão\n('S') - Sistema Linear\n('E') - Equação Algébrica\n('F') - Finalizar\n\nOpção > ");

    option = getc(stdin);

    cleanBuffer();

    if (option == 'C' || option ==  'c') {
        printf("numero > ");

        double num;
        scanf("%lf", &num);

        cleanBuffer();

        baseConversion(2, num);
        baseConversion(8, num);
        baseConversion(16, num);

        return 3;
    } else if (option == 'S' || option ==  's') {
        return linear_system();
    } else if (option == 'E' || option ==  'e') {
        printf("Não implementado");

        return 3;
    } else if (option == 'F' || option ==  'f') {
        printf("Finalizando");
    } else {
        printf("Opção desconhecia.");

        return 2;
    }

    return 0;
}
