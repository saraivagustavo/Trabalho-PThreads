#include <stdio.h>
#include <stdlib.h>
#include <time.h> //usar rand() pros números aleatórios e 
#include <math.h> //para usar sqrt e ver se é primo

//VARIÁVEIS GLOBAIS
// Definições de tamanho da matriz
#define MATRIZ_LINHAS 10000 //número de linhas
#define MATRIZ_COLUNAS 10000 //número de colunas
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
	buscaSerial(linhas, colunas); //chama a função que vai percorrer a matriz e ver se os números são primos
    freeMatriz(linhas);
    printf("Programa finalizado. beijos!\n");

    return 0;
}

void gerarMatriz(int l, int c) {
    matriz = (int**)malloc(l * sizeof(int*)); //aloca memória pras linhas
    if (matriz == NULL) {
        printf("Erro na alocacao de memoria pras linhas\n");
    }

    for (int i = 0; i < l; i++) {
		matriz[i] = (int*)malloc(c * sizeof(int)); //aloca memória pras colunas
		if (matriz[i] == NULL) { //controlar se a alocação falhou ou não
            printf("Erro na alocacao de memoria pras colunas\n");
            for (int j = 0; j < i; j++) {
                free(matriz[j]); //se a alocação pras colunas falhar, libera as linhas que tinham sido alocaadas antes
            }
            free(matriz); //libera tudo que tinha sido alocado
        }
        for (int j = 0; j < c; j++) {
            matriz[i][j] = rand() % 32000; //preenche a matriz com números aleatórios até 31999 igual no ava
        }
    }
    printf("Matriz de %dx%d gerada e preenchida com numero aleatorio com sucesso.\n", l, c); //verificação
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

// Implementação da função ehPrimo
int ehPrimo(int n) {
    if (n <= 1) { // 0 e 1 não são primos!!!!!!
        return 0;
    }
	for (int i = 2; i <= sqrt(n); i++) { //começa do 2 e vai até a raiz quadrada de n vendo se tem algum divisor
        if (n % i == 0) {
            return 0; //se achar algum divisor, ele não é primo
        }
    }
	return 1; //se NÃO achar nenhum divisor, ele é primo
}

void buscaSerial(int l, int c) {
	clock_t inicioSerial, fimSerial; //clock_t é da biblioteca, serve pra medir os ticks da cpu
    double tempoSerial;

    contadorPrimos = 0; //só de garantia coloca 0 aqui

	inicioSerial = clock(); //o clock() funciona como um marcador, marcando o tempo de início da execução do código nessa parte
 
    for (int i = 0; i < l; i++) {
        for (int j = 0; j < c; j++) {
            if (ehPrimo(matriz[i][j])) { //chama a função pra ver se é primo
				contadorPrimos++; //se for, acrescenta 
            }
        }
    }

	fimSerial = clock(); //tem que marcar depois que terminou de percorrer a matriz, pra pegar o tempo total de execução e depois calcular o tempo total 
	printf("Total de numeros primos encontrados: %ld\n", contadorPrimos); //imprime o total de primos encontrados na matriz

	//cálculo do tempo total de execução
	tempoSerial = (double)(fimSerial - inicioSerial) / CLOCKS_PER_SEC; //o CLOCKS_PER_SEC pega o valor que o clock() retornou e converte pra segundos
    printf("Tempo de exeucao de forma serial: %f segundos\n", tempoSerial);
}