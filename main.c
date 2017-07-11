/*
 * Trabalho Island
 * Civil Engineering Case Study
 *
 * Carolina Oltramari Nery
 *
 * matrizLidaTxt é a matriz que é lida do arquivo de texto.
 * matrizPoluente é a matriz que vai conter os valores da matrizlidatxt transformados em 200, 0 e 100
 * matrizCalculo é em função dela que será realizado o cálculo
 *
 * Equação
 * C(i;j) = c(i-1;j)+C(i+1;j) + C(i; j-1) + C(i; j+1) / 4
 *
 * Matriz só pode ter 0 1 ou 2
 *
 * */

#include <stdio.h>
#include <stdlib.h>

int** alocaMatrizInt(int nrows, int ncols);
float** alocaMatrizFloat(int nrows, int ncols);
float** preencheMatrizPoluentes(int nrows, int ncols, int** matTxt, float** matPoluentes);
float** calculoGausSeidel(int nrows, int ncols, float** matPol, float** matCalc);

/*Variaveis globais*/
int i, j;
int **matrizLidaTxt;
float **matrizPoluente;
float **matrizCalculo;
float valorDeParada; //valor de parada, que deverá ser comparado a gapValueMatriz.

int main() {
    int nrows, ncols;

    printf("Informe o valor de parada:\n ");
    scanf("%f", &valorDeParada);

    FILE *data;
    data = fopen("/home/carolinanery/CLionProjects/T1IslandCarolinaNery/MatrizTest.txt", "r");

    if (data == NULL) {
        printf("Não foi possível ler o arquivo");
    }

    fscanf(data, "%d %d\n", &nrows, &ncols);

    alocaMatrizInt(nrows, ncols);
    alocaMatrizFloat(nrows, ncols);

    if (matrizLidaTxt == NULL || matrizPoluente == NULL || matrizCalculo == NULL) {
        printf("Erro ao realizar a alocação!\n");
        exit(1);
    }

    /*Leio a matriz txt*/

    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            fscanf(data, "%d", &matrizLidaTxt[i][j]);
            printf("%d ", matrizLidaTxt[i][j]);
        }
        fscanf(data, "\n");
        printf("\n");
    }

    preencheMatrizPoluentes(nrows, ncols, matrizLidaTxt, matrizPoluente);

    fclose(data);
    printf("%s\n", "Arquivo fechado");
    printf("-----------------------\n");

    calculoGausSeidel(nrows, ncols, matrizPoluente, matrizCalculo);

    /*desaloca as matrizes*/
    for (i = 0; i < nrows; i++) {
        free(matrizLidaTxt[i]);
        free(matrizPoluente[i]);
        free(matrizCalculo[i]);
    }
    free(matrizLidaTxt);
    free(matrizPoluente);
    free(matrizCalculo);

    return 0;

}

float** alocaMatrizFloat(int nrows, int ncols){

    matrizPoluente = (float **) malloc(nrows * (sizeof(float *)));
    matrizCalculo = (float **) malloc(nrows * (sizeof(float *)));

    for(i = 0; i < nrows; i++){
        matrizPoluente[i] = (float *) malloc(ncols * (sizeof(float)));
        matrizCalculo[i] = (float *) malloc(ncols * (sizeof(float)));
    }

}

int** alocaMatrizInt(int nrows, int ncols){

    matrizLidaTxt = (int **) malloc(nrows * (sizeof(int *)));
    for(i = 0; i < nrows; i++){
        matrizLidaTxt[i] = (int *) malloc(ncols * (sizeof(int)));
    }
}

/* Metodo preencheMatrizPoluentes
 * A matrizPoluente recebe os valores de acordo com a matrizLidaTxt.
 * Se matrizLidaTxt tiver valores:
 * 0, a matrizPoluente recebe o valor 0;
 * 1, a matrizṔoluente recebe o valor 0;
 * 2, a matrizPoluente recebe o valor 200.
 * */
float** preencheMatrizPoluentes(int nrows, int ncols, int** matrizLidaTxt, float** matrizPoluente){
    for (i = 0; i < nrows; i++) {
        for (j = 0; j < ncols; j++) {
            if (matrizLidaTxt[i][j] == 0) {
                matrizPoluente[i][j] = 0;
            }
            if (matrizLidaTxt[i][j] == 1) {
                matrizPoluente[i][j] = 0;
            }
            if (matrizLidaTxt[i][j] == 2) {
                matrizPoluente[i][j] = 200;
            }
            printf("%8.5f ", matrizPoluente[i][j]);
        }
        printf("\n");
    }
}

/* Método que irá realizar o calculo em cada iteração, até chegar no valor de parada.*/
float** calculoGausSeidel(int nrows, int ncols, float** matrizPoluente, float** matrizCalculo){
    float gapValueMatriz; // maior diferença dentro do loop, que será comparado ao valor de parada.
    float gapValuePoint;

    do {
        gapValueMatriz = 0;

        for (i = 0; i < nrows; i++) {
            for (j = 0; j < ncols; j++) {

                if (matrizLidaTxt[i][j] == 2) {
                    matrizCalculo[i][j] = matrizPoluente[i][j];
                } else {
                    if (matrizLidaTxt[i][j] == 1) {
                        //canto superior esquerdo
                        if (i == 0 && j == 0) {
                            matrizCalculo[i][j] = (matrizPoluente[i+1][j] + matrizPoluente[i][j+1])/ 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        //canto inferior esquerdo
                        if (i == nrows-1 && j == 0){
                            matrizCalculo[i][j] = (matrizPoluente[i-1][j] + matrizPoluente[i][j+1]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        //canto superior direito
                        if (i == 0 && j == ncols-1){
                            matrizCalculo[i][j] = (matrizPoluente[i][j-1] + matrizPoluente[i+1][j]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        //canto inferior direito
                        if (i == nrows-1 && j == ncols-1) {
                            matrizCalculo[i][j] = (matrizPoluente[i - 1][j] + matrizPoluente[i][j - 1]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        //toda a linha superior quando i = 0
                        if ((j > 0 && j < ncols - 1) && i == 0) {
                            matrizCalculo[i][j] = (matrizPoluente[i][j-1] + matrizPoluente[i+1][j] + matrizPoluente[i][j+1]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        // toda a coluna do lado esquerdo quando j = 0
                        if ((i > 0 && i < nrows - 1) && j == 0) {
                            matrizCalculo[i][j] =
                                    (matrizPoluente[i - 1][j] + matrizPoluente[i + 1][j] + matrizPoluente[i][j + 1]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        //toda a linha inferior
                        if(i == nrows - 1 && (j > 0 && j < ncols - 1)) {
                            matrizCalculo[i][j] = (matrizPoluente[i][j-1] + matrizPoluente[i - 1][j] + matrizPoluente[i][j+1]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        //toda a coluna direita
                        if (j == ncols - 1 && (i > 0 && i < nrows - 1)){
                            matrizCalculo[i][j] = (matrizPoluente[i - 1][j] + matrizPoluente[i][j-1] + matrizPoluente[i + 1][j]) / 4;
                            gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                            if (gapValuePoint > gapValueMatriz) {
                                gapValueMatriz = gapValuePoint;
                            }
                            continue;
                        }
                        /*Equação
                         * Método de gaussSeidel
                         * c(i;j) = C(i-1;j) + C(i+1;j) + C(i;j-1) + C(i;j+1) / 4 */
                        matrizCalculo[i][j] = (matrizPoluente[i - 1][j] + matrizPoluente[i + 1][j] + matrizPoluente[i][j - 1] + matrizPoluente[i][j + 1]) / 4;
                        gapValuePoint = matrizCalculo[i][j] - matrizPoluente[i][j];
                        if (gapValuePoint > gapValueMatriz) {
                            gapValueMatriz = gapValuePoint;
                        }
                    }
                }

            }
        }
        for (i = 0; i < nrows; i++) {
            for (j = 0; j < ncols; j++) {
                matrizPoluente[i][j] = matrizCalculo[i][j];
                printf("%8.5f ", matrizPoluente[i][j]);

            }
            printf("\n");
        }

        printf("----------------\n");


    } while (gapValueMatriz > valorDeParada);

}

