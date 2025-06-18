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

/*GRUPO: GUSTAVO SARAIVA, PEDRO HENRIQUE ALBANI, TERTULIANO DOS SANTOS
    trabalho PThreads - Sistemas Operacionais*/

// *******************************************************
// **************** VARI�VEIS GLOBAIS ********************
// *******************************************************
int** matriz;
long contadorPrimos = 0;
// Defini��es de tamanho da matriz
#define MATRIZ_LINHAS 20000                 // <<<< MUDAR PARA O TAMANHO DESEJADO DA MATRIZ (LINHAS) >>>>
#define MATRIZ_COLUNAS 20000                // <<<< MUDAR PARA O TAMANHO DESEJADO DA MATRIZ (COLUNAS) >>>>

int proximoMacrobloco = 0;
pthread_mutex_t mutexContador;
// Defini��es de tamanho dos macroblocos e n�mero de threads
#define NUM_THREADS 6                       // <<<< MUDAR PARA O TAMANHO DESEJADO DA MATRIZ >>>>
#define MACROBLOCO_LINHAS  1000             // <<<< MUDAR PARA O TAMANHO DESEJADO DO MACROBLOCO (LINHAS) >>>>
#define MACROBLOCO_COLUNAS 1000             // <<<< MUDAR PARA O TAMANHO DESEJADO DO MACROBLOCO (COLUNAS) >>>>


// *******************************************************
// ******************* PROT�TIPOS ************************
// *******************************************************
int ehPrimo(int n);
void gerarMatriz(int l, int c);
void freeMatriz(int l);
void buscaSerial(int l, int c);
void buscaParalela(int l, int c);
void* percorrerCadaThread(void* arg);
void exibeMenu();

/*fun��es sem mutex para comparar com a com mutex, descomentar se quiser*/
//void buscaParalelaSemMutex(int l, int c);
//void* percorrerCadaThreadSemMutex(void* arg);


// *******************************************************
// ********************** MAIN ***************************
// *******************************************************
int main() {
    int linhas = MATRIZ_LINHAS;
    int colunas = MATRIZ_COLUNAS;

    printf("Iniciando a geracao da matriz de %dx%d...\n", linhas, colunas);
    gerarMatriz(linhas, colunas);

    pthread_mutex_init(&mutexContador, NULL);

    exibeMenu();

    freeMatriz(linhas);
    pthread_mutex_destroy(&mutexContador);
    printf("Programa finalizado. beijos!\n");

    return 0;
}

// *******************************************************
// **************** FUN��O EXIBEMENU *********************
// *******************************************************
void exibeMenu() { //!!!!!
    int escolha;
    char confirmarValores;
    int linhas = MATRIZ_LINHAS;
    int colunas = MATRIZ_COLUNAS;

    do {
        printf("\n\n--- MENU DE BUSCA DE PRIMOS ---\n");
        printf("1. Realizar Busca Serial\n");
        printf("2. Realizar Busca Paralela (COM Mutex)\n");
        // printf("3. Realizar Busca Paralela (SEM Mutex)\n);
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &escolha);
        while (getchar() != '\n');
        switch (escolha) {
        case 1:
            printf("\n--- Teste busca serial ---\n");
            buscaSerial(linhas, colunas);
            break;
        case 2:
            printf("\n--- Configuracao da Busca Paralela ---\n");
            printf("Valores Atuais:\n");
            printf("NUMERO DE THREADS: %d\n", NUM_THREADS);
            printf("TAMANHO DO MACROBLOCO: %dx%d\n", MACROBLOCO_LINHAS, MACROBLOCO_COLUNAS);
            printf("Deseja usar estes valores? (s/n): ");
            scanf(" %c", &confirmarValores);

            if (confirmarValores == 's' || confirmarValores == 'S') {
                printf("\nMACROBLOCOS: %dx%d\n", MACROBLOCO_LINHAS, MACROBLOCO_COLUNAS);
                printf("NUMERO THREADS: %d\n", NUM_THREADS);
                printf("--- Teste paralela COM mutex ---\n");

                proximoMacrobloco = 0;
                contadorPrimos = 0;
                buscaParalela(linhas, colunas);
            }
            else if (confirmarValores == 'n' || confirmarValores == 'N') {
                printf("Altere os valores das threads e tamanho dos macroblocos direto no codigo com o valor desejado e recompile.\n");
                exit(0);
            }
            else {
                printf("Opcao invalida.\n");
            }
            break;
            /*case 3: // Op��o para busca paralela SEM mutex
                printf("\n--- Configuracao da Busca Paralela (SEM Mutex) ---\n");
                printf("Valores Atuais:\n");
                printf("  Numero de Threads: %d\n", NUM_THREADS);
                printf("  Tamanho do Macrobloco: %dx%d\n", MACROBLOCO_LINHAS, MACROBLOCO_COLUNAS);
                printf("Deseja usar estes valores? (s/n): ");
                scanf(" %c", &confirmarValores);
                if (confirmarValores == 's' || confirmarValores == 'S') {
                    printf("\nMACROBLOCOS: %dx%d\n", MACROBLOCO_LINHAS, MACROBLOCO_COLUNAS);
                    printf("NUMERO THREADS: %d\n", NUM_THREADS);
                    printf("--- Teste paralelo SEM mutex ---\n");
                    proximoMacrobloco = 0;
                    contadorPrimos = 0;
                    buscaParalelaSemMutex(linhas, colunas);
                } else if (confirmarValores == 'n' || confirmarValores == 'N') {
                    printf("Altere os valores das threads e tamanho dos macroblocos direto no codigo com o valor desejado e recompile.");
                    exit(0);
                } else {
                    printf("Opcao invalida.\n");
                }
                break;*/
        case 0:
            printf("Saindo do programa.\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
        }
    } while (escolha != 0);
}



// *******************************************************
// **************** FUN��O GERAMATRIZ ********************
// *******************************************************
void gerarMatriz(int l, int c) { //copiei de ED ajeitando pro jeito da apostila do Giraldeli
    matriz = (int**)malloc(l * sizeof(int*));
    if (matriz == NULL) {
        printf("Erro na alocacao de memoria pras linhas\n");
        exit(1);
    }
    for (int i = 0; i < l; i++) {
        matriz[i] = (int*)malloc(c * sizeof(int));
        if (matriz[i] == NULL) {
            printf("Erro na alocacao de memoria pras colunas\n");
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            exit(1);
        }
        for (int j = 0; j < c; j++) {
            matriz[i][j] = rand() % 32000;
        }
    }
    printf("Matriz de %dx%d gerada e preenchida com numero aleatorio com sucesso.\n", l, c);
}


// *******************************************************
// **************** FUN��O FREEMATRIZ ********************
// *******************************************************
void freeMatriz(int l) { //copiei de ED
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
// ***************** FUN��O EHPRIMO **********************
// *******************************************************
int ehPrimo(int n) {
    if (n <= 1) {
        return 0;
    }

	int raizN = (int)sqrt(n);
    for (int i = 2; i <= raizN; i++) {
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

	for (int i = 0; i < l; i++) { //loop para percorrer as linhas da matriz
		for (int j = 0; j < c; j++) { //loop para percorrer as colunas da matriz
            if (ehPrimo(matriz[i][j])) {
                contadorPrimos++; //incrementa se entrar no if
            }
        }
    }

    fimSerial = clock();
    printf("Total de numeros primos encontrados: %ld\n", contadorPrimos);

    tempoSerial = (double)(fimSerial - inicioSerial) / CLOCKS_PER_SEC; //o CLOCKS_PER_SEC pega o valor do clock() e transforma em  segundos
    printf("Tempo de exeucao de forma serial: %.3f segundos\n", tempoSerial);
}


// *******************************************************
// **************** BUSCA PARALELA ***********************
// *******************************************************
void* percorrerCadaThread(void* arg) {
	long primoCadaThread = 0; //n�o pode usar o contadorPrimos aqui, porque cada thread vai ter que contar os primos que ela encontrou e depois somar no contador global (l� na regi�o cr�tica embaixo)
	int numMacroblocosLinha = (int)ceil((double)MATRIZ_LINHAS / MACROBLOCO_LINHAS); //garantir que o n�mero de linhas seja um m�ltiplo de MACROBLOCO_LINHAS
    int numMacroblocosColuna = (int)ceil((double)MATRIZ_COLUNAS / MACROBLOCO_COLUNAS); //mesma coisa para as colunas
    int totalMacroblocos = numMacroblocosLinha * numMacroblocosColuna;

    while (1) {
        int idMacrobloco;
		//tem que travar a regi�o cr�tica pq o proximoMacrobloco compartilha entre as threads
        pthread_mutex_lock(&mutexContador);
        idMacrobloco = proximoMacrobloco;
        proximoMacrobloco++;
        pthread_mutex_unlock(&mutexContador);

        if (idMacrobloco >= totalMacroblocos) { //verifica se j� percorreu todos os macroblocos que tinha na matriz
            break;
        }

        int linhaMacrobloco = idMacrobloco / numMacroblocosLinha; //separa a matriz nos macroblocos olhando a linha
        int colunaMacrobloco = idMacrobloco % numMacroblocosColuna; //separa a matriz nos macroblocos olhando a coluna

        int inicioLinha = linhaMacrobloco * MACROBLOCO_LINHAS; //calcula o in�cio da linha de cada microbloco, pra saber daonde come�a a percorrer
        int fimLinha = inicioLinha + MACROBLOCO_LINHAS;
		if (fimLinha > MATRIZ_LINHAS) { //garante que n�o ultrapassa o tamanho da matriz
            fimLinha = MATRIZ_LINHAS; 
        }

		int inicioColuna = colunaMacrobloco * MACROBLOCO_COLUNAS; //mesma coisa para as colunas
        int fimColuna = inicioColuna + MACROBLOCO_COLUNAS;
		if (fimColuna > MATRIZ_COLUNAS) { //garante que n�o ultrapassa o tamanho da matriz
            fimColuna = MATRIZ_COLUNAS;
        }

        primoCadaThread = 0;
        for (int i = inicioLinha; i < fimLinha; i++) { //loop pra come�ar a percorrer o macrobloco atual
            for (int j = inicioColuna; j < fimColuna; j++) {
                if (ehPrimo(matriz[i][j])) { //aqui que verifica se o n�mero desse lugar de dentro do macrobloco � primo ou n�o
                    primoCadaThread++;
                }
            }
        }

        //como vai incrementar o contador global de n�meros primos, tem que travar tamb�m, regi�o cr�tica!!!!
        pthread_mutex_lock(&mutexContador);
		contadorPrimos += primoCadaThread; //aqui que atualiza o contador global de n�meros primos encontrados em todas as threads
        pthread_mutex_unlock(&mutexContador);
    }
    return NULL;
}


void buscaParalela(int l, int c) {
    clock_t inicioParalela, fimParalela;
    double tempoParalela;
    pthread_t threads[NUM_THREADS];

    inicioParalela = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_create(&threads[i], NULL, percorrerCadaThread, NULL) != 0) { //c�digo do gigi *adaptar pra v�rias threads
            perror("Pthread_create falhou!");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], NULL) != 0) { //c�digo do gigi *adaptar pra v�rias threads
            perror("Pthread_join falhou!");
            exit(1);
        }
    }

    fimParalela = clock();
    printf("Total de numeros primos encontrados: %ld\n", contadorPrimos);

    tempoParalela = (double)(fimParalela - inicioParalela) / CLOCKS_PER_SEC;
    printf("Tempo de execucao de forma paralela: %.3f segundos\n", tempoParalela);
}


// *******************************************************
// ************* BUSCA PARALELA >>SEM<< MUTEX ************
// *******************************************************

/*apenas a t�tulo de curiosidade de compara��o dos resultados, descomentar se quiser*/

//void* percorrerCadaThreadSemMutex(void* arg) {
//    long primoCadaThread = 0;
//    int numMacroblocosLinha = (int)ceil((double)MATRIZ_LINHAS / MACROBLOCO_LINHAS);
//    int numMacroblocosColuna = (int)ceil((double)MATRIZ_COLUNAS / MACROBLOCO_COLUNAS);
//    int totalMacroblocos = numMacroblocosLinha * numMacroblocosColuna;
//
//    while (1) {
//        int idMacrobloco;
//
//        idMacrobloco = proximoMacrobloco;
//        proximoMacrobloco++;
//
//        if (idMacrobloco >= totalMacroblocos) {
//            break;
//        }
//
//        int linhaMacrobloco = idMacrobloco / numMacroblocosColuna;
//        int colunaMacrobloco = idMacrobloco % numMacroblocosColuna;
//
//        int inicioLinha = linhaMacrobloco * MACROBLOCO_LINHAS; 
//        int fimLinha = inicioLinha + MACROBLOCO_LINHAS; 
//        if (fimLinha > MATRIZ_LINHAS) { 
//            fimLinha = MATRIZ_LINHAS; 
//        }
//
//        int inicioColuna = colunaMacrobloco * MACROBLOCO_COLUNAS; 
//        int fimColuna = inicioColuna + MACROBLOCO_COLUNAS; 
//        if (fimColuna > MATRIZ_COLUNAS) { 
//            fimColuna = MATRIZ_COLUNAS;
//        }
//
//        primoCadaThread = 0; 
//        for (int i = inicioLinha; i < fimLinha; i++) {
//            for (int j = inicioColuna; j < fimColuna; j++) {
//                if (ehPrimo(matriz[i][j])) { 
//                    primoCadaThread++; 
//                }
//            }
//        }
//        contadorPrimos += primoCadaThread;
//    }
//    return NULL; 
//}
//
//void buscaParalelaSemMutex(int l, int c) { 
//    clock_t inicioParalela, fimParalela; 
//    double tempoParalela; 
//    pthread_t threads[NUM_THREADS]; 
//
//    inicioParalela = clock(); 
//
//    for (int i = 0; i < NUM_THREADS; i++) { 
//        if (pthread_create(&threads[i], NULL, percorrerCadaThreadSemMutex, NULL) != 0) {
//            perror("Pthread_create falhou!");
//            exit(1); 
//        }
//    }
//
//    for (int i = 0; i < NUM_THREADS; i++) { 
//        if (pthread_join(threads[i], NULL) != 0) { 
//            perror("Pthread_join falhou!");
//            exit(1);
//        }
//    }
//
//    fimParalela = clock();
//    printf("Total de numeros primos encontrados: %ld (SEM MUTEX)\n", contadorPrimos);
//
//    tempoParalela = (double)(fimParalela - inicioParalela) / CLOCKS_PER_SEC;
//    printf("Tempo de execucao de forma paralela: %.3f segundos (SEM MUTEX)\n", tempoParalela);
//}