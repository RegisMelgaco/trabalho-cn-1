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

/*
* s_retro retorna um int que prese
* m é uma matriz aumentada de um SL com n variáveis.
* se compatível, a ou uma resposta será gravada em x.
* se determinado será retornado 0.
* se indeterminado será retornado 1.
* se incompatível será retornado 2.
*/
int s_retro(double **m, int n, double x[]){
	int i, j, kind = 0;
	double sum = 0;
	
	for (i = n-1; i >= 0; i--) {
		sum = 0;
		for (j = i+1; j < n; j++) {
			sum += m[i][j]*x[j];
		}
		
		if (m[i][i] == 0) {
			if (fabs(m[i][n] - sum) < EPSILON) {
				kind = 1; // variável lívre
				x[i] = 0;
			} else {
				return 2;
			}
		} else {
			x[i] = m[i][n] - sum / m[i][i];
		}
	}
	
	return kind;
}

void print_matrix(double **m, int l, int c) {
	int i, j;
	for (i = 0; i < l; i++) {
	  	for (j = 0; j < c; j++) {
			printf("m[%d][%d] %10.3lf \n", i, j, m[i][j]);
		}
		
		printf(";\n");
	}
}

int linear_system() {
    char c;
    int n, i, j, k;
    double **matrix;
    char file_name[100];
    FILE *file;


    printf("Qual o nome do arquivo que contem o sistema linear? ");

    // clean buff
    while ((c = getchar()) != '\n' && c != EOF) {}

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
        double p = matrix[i][i];

        // se pivo nulo, tentar trocar coluna de coeficientes i a direita que tenha um valor não nulo na linha i
        if (p == 0) {
            // busca pela coluna
            j = i+1;
            while (j < n && matrix[i][j] == 0) j++;

            // caso coluna não seja encontrada, verificar se o sistema linear é incompatível
            if (j == n) {
                double b = matrix[i][n];

                if (b != 0) {
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

        for (j = 0; j < n; j++) {
            if (j == i) {
                continue;
            }

            double c = -matrix[j][i]/p;
            for (k = 0; k <= n; k++) {
                matrix[j][k] += matrix[i][k] * c;
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

    return 0;
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

    printf("Escolha uma opção de operação:\n\n('C') - Conversão\n('S') - Sistema Linear\n('E') - Equação Algébrica\n('F') - Finalizar\n\nOpção> ");

    option = getc(stdin);

    if (option == 'C' || option ==  'c') {
        printf("Não implementado");

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
