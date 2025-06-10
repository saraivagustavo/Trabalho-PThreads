# Introdução à Programação Multithread com PThreads

![C Language](https://img.shields.io/badge/Language-C-blue.svg)
![PThreads](https://img.shields.io/badge/Concurrency-PThreads-green.svg)
![Build Status](https://img.shields.io/badge/Status-Completed-brightgreen.svg)

## 📝 Descrição do Projeto

Este projeto tem como objetivo principal explorar os conceitos de programação concorrente utilizando a biblioteca POSIX PThreads em C. Através da implementação de um algoritmo para contar números primos em grandes matrizes de números aleatórios, o trabalho compara o desempenho entre uma abordagem serial e uma paralela. O foco está na análise do impacto do paralelismo, da sincronização de threads via mutexes e da granularidade das tarefas (macroblocos) no tempo de execução e na correção dos resultados.

## 🚀 Funcionalidades

* **Geração de Matriz:** Criação dinâmica de uma matriz de grandes dimensões (`[20000]x[20000]`) preenchida com números naturais aleatórios (0 a 31999).
* **Busca Serial de Primos:** Implementação de um algoritmo sequencial para identificar e contar números primos na matriz, servindo como tempo de referência.
* **Busca Paralela de Primos (PThreads):** Utilização de múltiplas threads para processar a matriz em "macroblocos", distribuindo a carga de trabalho.
* **Sincronização com Mutexes:** Proteção de variáveis globais compartilhadas (`contadorPrimos`, `proximoMacrobloco`) para garantir a integridade dos dados e a correção do resultado em ambiente concorrente.
* **Medição de Desempenho:** Cálculo e exibição do tempo de execução para as buscas serial e paralela.
* **Análise de Condições de Corrida:** Inclusão de uma versão da busca paralela sem mutexes para demonstrar o problema da inconsistência de dados em ambiente não sincronizado.

## ⚙️ Como Compilar e Rodar

Este projeto foi desenvolvido e testado no **Microsoft Visual Studio 2022** com o ambiente de **C++ Desktop Development** e a biblioteca **PThreads-w32**.

### Pré-requisitos

* **Microsoft Visual Studio 2022:** Com a workload "Desenvolvimento para desktop com C++" instalada.
* **PThreads-w32:** A biblioteca foi configurada manualmente conforme as instruções do professor, copiando `pthreadVC2.lib` e `pthreadVC2.dll` para os diretórios apropriados do Visual Studio.
    * Certifique-se de que a macro `#define HAVE_STRUCT_TIMESPEC` está definida no topo do seu `main.c` para evitar conflitos de tipo.

### Passos para Compilação e Execução

1.  **Clone o Repositório:**
    ```bash
    git clone [https://docs.github.com/articles/referencing-and-citing-content](https://docs.github.com/articles/referencing-and-citing-content)
    cd [nome-do-seu-repositorio]
    ```
2.  **Abra no Visual Studio:** Abra o arquivo de solução (`.sln`) do projeto no Visual Studio 2022.
3.  **Configurar Plataforma:** No Visual Studio, selecione a plataforma de solução para **x64** (ou x86 se preferir, mas x64 é recomendado para matrizes grandes).
4.  **Compilar:** No menu do Visual Studio, vá em `Build > Build Solution` (ou `Ctrl+Shift+B`).
5.  **Executar:** Para rodar o programa, vá em `Debug > Start Without Debugging` (ou `Ctrl+F5`). O console exibirá os tempos de execução e o total de primos para as buscas serial, paralela com mutex e paralela sem mutex.

### Configurações de Teste

Você pode ajustar as seguintes macros no arquivo `main.c` para testar diferentes cenários:

* `#define MATRIZ_LINHAS [X]`
* `#define MATRIZ_COLUNAS [Y]`
* `#define NUM_THREADS [N]` (Número de threads a serem criadas)
* `#define MACROBLOCO_LINHAS [ML]`
* `#define MACROBLOCO_COLUNAS [MC]`

## 📊 Análise de Desempenho (Baseado em Testes)

Os testes foram conduzidos em [AMD Ryzen 7 5300X3D (8 Cores Físicos, 16 Threads Lógicas) e AMD Ryzen 5 5600 (6 Cores Físicos, 12 Threads Lógicas)], utilizando matrizes de até 30000 x 30000 elementos.

### Principais Observações:

* **Aceleração pelo Paralelismo:** A busca paralela consistentemente superou a serial em matrizes grandes quando otimizada.
    * Ex: Matriz 20000 x 20000: Serial: 15.776s (Ryzen 7); Paralela (1000 x 1000 macrobloco, 16 Core): 1.650s.
* **Impacto da Granularidade (Tamanho do Macrobloco):**
    * **Macroblocos Pequenos (ex: 1 x 1):** Causam *overhead* excessivo de sincronização, resultando em desempenho pior que o serial (ex: 68.616s para 1 x 1 no Ryzen 7). A alta frequência de aquisição/liberação de mutexes (mesmo que ausentes no teste sem mutex) e a gestão de muitas pequenas tarefas anulam os ganhos.
    * **Macroblocos Ótimos:** Uma "faixa ideal" (aprox. 1000 x 1000 a 5000 x 5000) proporcionou os melhores tempos, balanceando a divisão do trabalho com o *overhead* (ex: 1.650s no Ryzen 7 para 1000 x 1000).
    * **Macroblocos Grandes (ex: matriz inteira):** Reduzem o paralelismo efetivo, aproximando o tempo do serial (ex: 15.81$s para 20000 x 20000 no Ryzen 7), pois o trabalho não é distribuído de forma eficiente entre as threads.
* **Influência do Número de Threads/Cores:**
    * O desempenho melhora com o aumento do número de threads até o limite de núcleos físicos/lógicos, conforme a Lei de Amdahl.
    * Ex: Matriz 20000 x 20000 (Ryzen 7, 1000 x 1000 macrobloco): de 8.003s (2 Core) para 1.650s (16 Core).
    * O uso de threads lógicas (Hyper-Threading) pode oferecer ganhos, mas com menor eficiência do que os núcleos físicos.
* **Necessidade da Sincronização (Mutexes):**
    * Testes sem mutexes, embora não tenham manifestado inconsistência na contagem de primos em algumas execuções (devido à natureza não determinística das condições de corrida e otimizações), são teórica e pragmaticamente cruciais para demonstrar que a proteção de variáveis compartilhadas é vital para a **correção e consistência** dos resultados em um ambiente multithread. Sem mutexes, o risco de resultados incorretos devido a condições de corrida é sempre presente.

## **Integrantes do Projeto**
| Aluno | GitHub | LinkedIn |
|-------|--------|----------|
| Gustavo Saraiva Mariano | [![GitHub](https://img.shields.io/badge/github-black?style=for-the-badge&logo=github)](https://github.com/saraivagustavo) | [![LinkedIn](https://img.shields.io/badge/linkedin-blue?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/gustavo-saraiva-mariano/) |
| Pedro Henrique Albani Nunes | [![GitHub](https://img.shields.io/badge/github-black?style=for-the-badge&logo=github)](https://github.com/PedroAlbaniNunes) | [![LinkedIn](https://img.shields.io/badge/linkedin-blue?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/pedro-henrique-albani-nunes-33a729270/) |
| Tertuliano dos Santos | [![GitHub](https://img.shields.io/badge/github-black?style=for-the-badge&logo=github)](https://github.com/TertSJ) | [![LinkedIn](https://img.shields.io/badge/linkedin-blue?style=for-the-badge&logo=linkedin)]() |

## 🎓 Disciplina e Professor

* **Disciplina:** Sistemas Operacionais
* **Professor:** Flávio Giraldeli
* **Instituição:** INSTITUTO FEDERAL DO ESPÍRITO SANTO - Campus Serra

---
