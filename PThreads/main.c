#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>


// *******************************************************
// **************** VARIÁVEIS GLOBAIS ********************
// *******************************************************
int** matriz;
long contadorPrimos = 0;
#define MATRIZ_LINHAS 10000
#define MATRIZ_COLUNAS 10000

int proximoMacrobloco = 0;
pthread_mutex_t mutexContador;
#define NUM_THREADS 6
#define MACROBLOCO_LINHAS 100
#define MACROBLOCO_COLUNAS 100


// *******************************************************
// ******************* PROTÓTIPOS ************************
// *******************************************************
int ehPrimo(int n);
void gerarMatriz(int l, int c);
void freeMatriz(int l);
void buscaSerial(int l, int c);
void buscaParalela(int l, int c);
void* percorrerCadaThread(void* arg);


// *******************************************************
// ********************** MAIN ***************************
// *******************************************************
int main() {
    int linhas = MATRIZ_LINHAS;
    int colunas = MATRIZ_COLUNAS;

    printf("Iniciando a geracao da matriz de %dx%d...\n", linhas, colunas);
    gerarMatriz(linhas, colunas);


    buscaSerial(linhas, colunas);

    pthread_mutex_init(&mutexContador, NULL); //inicializa o semáforo
    proximoMacrobloco = 0;
    contadorPrimos = 0;
    buscaParalela(linhas, colunas);

    freeMatriz(linhas);
	pthread_mutex_destroy(&mutexContador); //destrói o semáforo
    printf("Programa finalizado. beijos!\n");

    return 0;
}


// *******************************************************
// **************** FUNÇÃO GERAMATRIZ ********************
// *******************************************************
void gerarMatriz(int l, int c) {
    matriz = (int**)malloc(l * sizeof(int*));
    if (matriz == NULL) {
        printf("Erro na alocacao de memoria pras linhas\n");
        exit(EXIT_FAILURE); //!!!!!
    }
    for (int i = 0; i < l; i++) {
        matriz[i] = (int*)malloc(c * sizeof(int));
        if (matriz[i] == NULL) {
            printf("Erro na alocacao de memoria pras colunas\n"); //!!!!!
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            exit(EXIT_FAILURE); //!!!!!
        }
        for (int j = 0; j < c; j++) {
            matriz[i][j] = rand() % 32000;
        }
    }
    printf("Matriz de %dx%d gerada e preenchida com numero aleatorio com sucesso.\n", l, c);
}


// *******************************************************
// **************** FUNÇÃO FREEMATRIZ ********************
// *******************************************************
void freeMatriz(int l) {
    if (matriz != NULL) {
        for (int i = 0; i < l; i++) {
            if (matriz[i] != NULL) {
                free(matriz[i]);
            }
        }
        free(matriz);
        matriz = NULL;
        printf("Memoria da matriz liberada com sucesso.\n");
    }
}


// *******************************************************
// ***************** FUNÇÃO EHPRIMO **********************
// *******************************************************
int ehPrimo(int n) {
    if (n <= 1) {
        return 0;
    }
    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}


// *******************************************************
// ****************** BUSCA SERIAL ***********************
// *******************************************************
void buscaSerial(int l, int c) {
    clock_t inicioSerial, fimSerial;
    double tempoSerial;

    contadorPrimos = 0;

    inicioSerial = clock();

    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            if (ehPrimo(matriz[i][j])) {
                contadorPrimos++;
            }
        }
    }

    fimSerial = clock();
    printf("Total de numeros primos encontrados (Serial): %ld\n", contadorPrimos);

    tempoSerial = (double)(fimSerial - inicioSerial) / CLOCKS_PER_SEC;
    printf("Tempo de exeucao de forma serial: %.f segundos\n", tempoSerial);
}


// *******************************************************
// **************** BUSCA PARALELA ***********************
// *******************************************************
void* percorrerCadaThread(void* arg) {
    long primoCadaThread = 0;
	int numMacroblocosLinha = (int)ceil((double)MATRIZ_LINHAS / MACROBLOCO_LINHAS); //garantir que o número de linhas seja um múltiplo de MACROBLOCO_LINHAS
    int numMacroblocosColuna = (int)ceil((double)MATRIZ_COLUNAS / MACROBLOCO_COLUNAS); //mesma coisa para as colunas
    int totalMacroblocos = numMacroblocosLinha * numMacroblocosColuna;

    while (1) {
        int idMacrobloco;

        pthread_mutex_lock(&mutexContador);
        idMacrobloco = proximoMacrobloco;
        proximoMacrobloco++;
        pthread_mutex_unlock(&mutexContador);

        if (idMacrobloco >= totalMacroblocos) { //verifica se já percorreu todos os macroblocos que tinha na matriz
            break;
        }

        int linhaMacrobloco = idMacrobloco / numMacroblocosLinha; //separa a matriz nos macroblocos olhando a linha
        int colunaMacrobloco = idMacrobloco % numMacroblocosColuna; //separa a matriz nos macroblocos olhando a coluna

        int inicioLinha = linhaMacrobloco * MACROBLOCO_LINHAS; //calcula o início da linha de cada microbloco, pra saber daonde começa a percorrer
        int fimLinha = inicioLinha + MACROBLOCO_LINHAS;
		if (fimLinha > MATRIZ_LINHAS) { //garante que não ultrapassa o tamanho da matriz
            fimLinha = MATRIZ_LINHAS; 
        }

		int inicioColuna = colunaMacrobloco * MACROBLOCO_COLUNAS; //mesma coisa para as colunas
        int fimColuna = inicioColuna + MACROBLOCO_COLUNAS;
		if (fimColuna > MATRIZ_COLUNAS) { //garante que não ultrapassa o tamanho da matriz
            fimColuna = MATRIZ_COLUNAS;
        }

        primoCadaThread = 0;
        for (int i = inicioLinha; i < fimLinha; i++) { //loop pra começar a percorrer o macrobloco atual
            for (int j = inicioColuna; j < fimColuna; j++) {
                if (ehPrimo(matriz[i][j])) { //aqui que verifica se o número desse lugar do macrobloco é primo ou não
                    primoCadaThread++;
                }
            }
        }

        //como vai incrementar o contador global de números primos, tem que travar também, região crítica!!!!
        pthread_mutex_lock(&mutexContador);
        contadorPrimos += primoCadaThread;
        pthread_mutex_unlock(&mutexContador);
    }
    return NULL;
}


void buscaParalela(int l, int c) {
    clock_t inicioParalela, fimParalela;
    double tempoParalela;
    pthread_t threads[NUM_THREADS]; //vetor das threads

    inicioParalela = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_create(&threads[i], NULL, percorrerCadaThread, NULL) != 0) { //código do gigi *adaptar pra várias threads
            perror("Pthread_create falhou!");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], NULL) != 0) { //código do gigi *adaptar pra várias threads
            perror("Pthread_join falhou!");
            exit(1);
        }
    }

    fimParalela = clock();

    tempoParalela = (double)(fimParalela - inicioParalela) / CLOCKS_PER_SEC;
    printf("Tempo de execucao de forma paralela: %f segundos\n", tempoParalela);
}
