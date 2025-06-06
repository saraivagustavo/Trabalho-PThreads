#include <stdio.h>
#include <stdlib.h>
#include <time.h> //usar rand() pros n�meros aleat�rios e 
#include <math.h> //para usar sqrt e ver se � primo

//VARI�VEIS GLOBAIS
// Defini��es de tamanho da matriz
#define MATRIZ_LINHAS 10000 //n�mero de linhas
#define MATRIZ_COLUNAS 10000 //n�mero de colunas
int** matriz; //ponteiro para ponteiro, ou seja, matriz de ponteiros, que vai apontar pras linhas e colunas da matriz
long contadorPrimos = 0; //autoexplicativo

int ehPrimo(int n);
void gerarMatriz(int l, int c);
void freeMatriz(int l);
void buscaSerial(int l, int c);

int main() {
    int linhas = MATRIZ_LINHAS;
    int colunas = MATRIZ_COLUNAS;

    printf("Iniciando a geracao da matriz de %dx%d...\n", linhas, colunas);
    gerarMatriz(linhas, colunas);
	buscaSerial(linhas, colunas); //chama a fun��o que vai percorrer a matriz e ver se os n�meros s�o primos
    freeMatriz(linhas);
    printf("Programa finalizado. beijos!\n");

    return 0;
}

void gerarMatriz(int l, int c) {
    matriz = (int**)malloc(l * sizeof(int*)); //aloca mem�ria pras linhas
    if (matriz == NULL) {
        printf("Erro na alocacao de memoria pras linhas\n");
    }

    for (int i = 0; i < l; i++) {
		matriz[i] = (int*)malloc(c * sizeof(int)); //aloca mem�ria pras colunas
		if (matriz[i] == NULL) { //controlar se a aloca��o falhou ou n�o
            printf("Erro na alocacao de memoria pras colunas\n");
            for (int j = 0; j < i; j++) {
                free(matriz[j]); //se a aloca��o pras colunas falhar, libera as linhas que tinham sido alocaadas antes
            }
            free(matriz); //libera tudo que tinha sido alocado
        }
        for (int j = 0; j < c; j++) {
            matriz[i][j] = rand() % 32000; //preenche a matriz com n�meros aleat�rios at� 31999 igual no ava
        }
    }
    printf("Matriz de %dx%d gerada e preenchida com numero aleatorio com sucesso.\n", l, c); //verifica��o
}

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

// Implementa��o da fun��o ehPrimo
int ehPrimo(int n) {
    if (n <= 1) { // 0 e 1 n�o s�o primos!!!!!!
        return 0;
    }
	for (int i = 2; i <= sqrt(n); i++) { //come�a do 2 e vai at� a raiz quadrada de n vendo se tem algum divisor
        if (n % i == 0) {
            return 0; //se achar algum divisor, ele n�o � primo
        }
    }
	return 1; //se N�O achar nenhum divisor, ele � primo
}

void buscaSerial(int l, int c) {
	clock_t inicioSerial, fimSerial; //clock_t � da biblioteca, serve pra medir os ticks da cpu
    double tempoSerial;

    contadorPrimos = 0; //s� de garantia coloca 0 aqui

	inicioSerial = clock(); //o clock() funciona como um marcador, marcando o tempo de in�cio da execu��o do c�digo nessa parte
 
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            if (ehPrimo(matriz[i][j])) { //chama a fun��o pra ver se � primo
				contadorPrimos++; //se for, acrescenta 
            }
        }
    }

	fimSerial = clock(); //tem que marcar depois que terminou de percorrer a matriz, pra pegar o tempo total de execu��o e depois calcular o tempo total 
	printf("Total de numeros primos encontrados: %ld\n", contadorPrimos); //imprime o total de primos encontrados na matriz

	//c�lculo do tempo total de execu��o
	tempoSerial = (double)(fimSerial - inicioSerial) / CLOCKS_PER_SEC; //o CLOCKS_PER_SEC pega o valor que o clock() retornou e converte pra segundos
    printf("Tempo de exeucao de forma serial: %f segundos\n", tempoSerial);
}