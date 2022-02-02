#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include<string.h>
#include <time.h>
#include "./libs/timer.h"

void readMatrixKey();
void readMatrixInverse();
void convertMessageToNumberMatrix(char **message);
void convertNumberMatrixToText(char **matrixMessage);
int determinant(int *matrix, int dim);
char *readMessage(char *filename);
void cryptographMessage();
void decryptographMessage();
void *cryptographMessageConcurrently(void *arg);
void *decryptographMessageConcurrently(void *arg);
void checkResults();
void printMessage();
void barreira(int nthreads);

char *cryptoSequencial;
char *decryptoSequencial;
int *matConcurrent;
int nthreads;
int size_message;
int *matrixKey;
int *matrixInverse;
int dim;
int *matrixMessage;
char *message;
char *messageToCheck;
int bloqueadas = 0;

typedef struct
{
    int id;
    int line_start, line_end, line_size;
} tArgs;

pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

int main(int argc, char *argv[])
{
    pthread_t *tid;
    tArgs *args;
    double start, end, delta;
    char *filename;

    if (argc < 3)
    {
        fprintf(stderr, "Digite: %s <numero de threads> <arquivo a ser criptografado>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    nthreads = atoi(argv[1]);
    filename = argv[2];

    if (nthreads <= 0)
    {
        fprintf(stderr, "Entre com um numero positivo de threads\n");
        exit(EXIT_FAILURE);
    }

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init(&x_cond, NULL);

    // --------------------------
    // === Leitura do Arquivo ===
    // --------------------------

    GET_TIME(start);

    message = readMessage(filename);
    messageToCheck = readMessage(filename);

    GET_TIME(end);
    delta = end - start;
    printf("A leitura do arquivo levou: %lf ms\n", delta);
    //print para teste
    //printf("%s\n", message);

    // -------------------------------------
    // === Conversão do texto em números ===
    // -------------------------------------

    GET_TIME(start);

    convertMessageToNumberMatrix(&message);
    GET_TIME(end);
    delta = end - start;
    printf("A conversao de texto em numeros levou: %lf ms\n", delta);
    // print para teste
    // for (int j = 0; j < size_message; j++)
    // {
    //     //printf("%d ", message[j]);
    // }
    //printf("\n");
    // --------------------------------
    // === Input de dados da matriz ===
    // --------------------------------

    readMatrixKey();
    readMatrixInverse();

    // ------------------------------
    // === Criptografa Sequencial ===
    // ------------------------------
    GET_TIME(start);
    cryptographMessage(message);
    GET_TIME(end);
    delta = end - start;
    printf("A criptografa sequencial levou: %lf ms\n", delta);

    // ----------------------------------
    // === Descriptografia Sequencial ===
    // ----------------------------------
    GET_TIME(start);
    decryptographMessage();
    GET_TIME(end);
    delta = end - start;
    printf("A descriptografia sequencial levou: %lf ms\n", delta);

    convertNumberMatrixToText(&decryptoSequencial);

    // -------------------------------
    // === Criptografa Concorrente ===
    // -------------------------------
    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL)
    {
        puts("Erro ao alocar tid\n");
        return 2;
    }

    args = (tArgs *)malloc(sizeof(tArgs) * nthreads);
    if (args == NULL)
    {
        puts("Erro ao alocar args\n");
        return 2;
    }

    GET_TIME(start);

    int sizeLinePerThread = size_message / dim;
    int quantityLines = dim;
    

    for (int thread = 0; thread < nthreads; thread++)
    {
        (args + thread)->line_start = thread * ((quantityLines) / nthreads);
        (args + thread)->line_end = (thread * ((quantityLines) / nthreads)) + ((quantityLines) / nthreads);
        (args + thread)->line_size = sizeLinePerThread;
        (args + thread)->id = thread;


        if (pthread_create(tid + thread, NULL, cryptographMessageConcurrently, (void *)(args + thread)))
        {
            fprintf(stderr, "Erro ao criar threads para a funcao cryptographMessageConcurrently\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            //printf("Thread criada com sucesso. Tid: %ld\n", *(tid + thread));
        }
    }

    // Aguarda as threads finalizarem
    for (int thread = 0; thread < nthreads; thread++)
    {
        if (pthread_join(*(tid + thread), NULL))
        {
            fprintf(stderr, "Erro ao aguardar as threads terminarem a execução\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            //printf("Thread finalizada com sucesso. Tid: %ld\n", *(tid + thread));
        }
    }

    GET_TIME(end);
    delta = end - start;
    printf("A criptografa concorrente levou: %lf ms\n", delta);
    // for (int j = 0; j < size_message; j++)
    // {
    //     //printf("%d ", message[j]);
    // }

    // ----------------------------------
    // === Descriptografa Concorrente ===
    // ----------------------------------
    tid = (pthread_t *)malloc(sizeof(pthread_t) * nthreads);
    if (tid == NULL)
    {
        puts("Erro ao alocar tid\n");
        return 2;
    }

    args = (tArgs *)malloc(sizeof(tArgs) * nthreads);
    if (args == NULL)
    {
        puts("Erro ao alocar args\n");
        return 2;
    }

    GET_TIME(start);

    sizeLinePerThread = size_message / dim;
    quantityLines = dim;

    for (int thread = 0; thread < nthreads; thread++)
    {
        (args + thread)->line_start = thread * ((quantityLines) / nthreads);
        (args + thread)->line_end = (thread * ((quantityLines) / nthreads)) + ((quantityLines) / nthreads);
        (args + thread)->line_size = sizeLinePerThread;
        (args + thread)->id = thread;

        if (pthread_create(tid + thread, NULL, decryptographMessageConcurrently, (void *)(args + thread)))
        {
            fprintf(stderr, "Erro ao criar threads para a funcao decryptographMessageConcurrently\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            //printf("Thread criada com sucesso. Tid: %ld\n", *(tid + thread));
        }
    }

    // Aguarda as threads finalizarem
    for (int thread = 0; thread < nthreads; thread++)
    {
        if (pthread_join(*(tid + thread), NULL))
        {
            fprintf(stderr, "Erro ao aguardar as threads terminarem a execução\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            //printf("Thread finalizada com sucesso. Tid: %ld\n", *(tid + thread));
        }
    }

    GET_TIME(end);
    delta = end - start;
    printf("A descriptografia concorrente levou: %lf ms\n", delta);
    // for (int j = 0; j < size_message; j++)
    // {
    //     //printf("%d ", message[j]);
    // }

    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);

    

    // --------------------------------------
    // === Conversão dos números em texto ===
    // --------------------------------------

    GET_TIME(start);

    convertNumberMatrixToText(&message);

    GET_TIME(end);
    delta = end - start;
    printf("A conversao dos numeros em texto levou: %lf ms\n", delta);
    //printf("%s\n", message);

    free(tid);

    checkResults(message, messageToCheck);

    return 0;
}

char *readMessage(char *filename)
{

    char *message = NULL;
    int string_size, read_size;
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Falha ao abrir o arquivo: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (file)
    {

        fseek(file, 0, SEEK_END);
        string_size = ftell(file);
        rewind(file);
        message = (char *)malloc(sizeof(char) * (string_size + 1));
        read_size = fread(message, sizeof(char), string_size, file);

        message[string_size] = '\0';

        if (string_size != read_size)
        {
            free(message);
            message = NULL;
        }

        fclose(file);
    }
    size_message = string_size;
    return message;
}

void convertMessageToNumberMatrix(char **message)
{

    int i = 0;
    while ((*message)[i] != '\0')
    {
        if ((*message)[i] >= 65 && ((*message)[i]) <= 90)
        {
            (*message)[i] -= 64;
            i++;
            continue;
        }

        if ((*message)[i] >= 97 && (*message)[i] <= 122)
        {
            (*message)[i] -= 96;
            i++;
            continue;
        }

        (*message)[i] = 35;
        i++;
    }
}

void convertNumberMatrixToText(char **matrixMessage)
{

    int i = 0;

    while ((char)(*matrixMessage)[i] != (char)'\0')
    {

        if ((*matrixMessage)[i] == 35)
        {
            i++;
            continue;
        }

        (*matrixMessage)[i] += 64;
        i++;
    }
}

void readMatrixKey()
{
    int i = 0, j = 0;

    //printf("Para criptografar sua mensagem, eh preciso de uma matriz chave. Entre com a dimensao da sua matriz quadrada:\n");
    scanf("%d", &dim);
    
    //printf("Agora, digite um a um cada elemento inteiro da matriz:\n");
    matrixKey = (int *)malloc(sizeof(int) * dim * dim);
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            scanf("%d", &matrixKey[i * dim + j]);
        }
    }
}

void readMatrixInverse()
{
    int i = 0, j = 0;

    //printf("Agora, digite um a um cada elemento inteiro da matriz inversa de sua chave:\n");
    matrixInverse = (int *)malloc(sizeof(int) * dim * dim);
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            scanf("%d", &matrixInverse[i * dim + j]);
        }
    }
}

void *cryptographMessageConcurrently(void *arg)
{
    int start, end, line_size;
    tArgs *args = (tArgs *)arg;
    start = args->line_start;
    end = args->line_end;
    line_size = args->line_size;
    int value;
    int columnAux;
    char* aux = NULL;

    aux = (char *)malloc(sizeof(char) * (size_message + 1));
    
    for (int i = start; i < end; i++) // Linha da matriz da mensagem
    {
        columnAux = 0;
        for (int j = (line_size * i); j < (line_size * i) + line_size; j++) { //Cada casa da linha
            value = 0;
            for (int k = 0; k < dim; k++) // Linha da matriz chave
            {
                value += message[columnAux + (k * line_size)] * matrixKey[(k * dim) + i];
            }
            aux[j] = value;
            columnAux++;
        }
        
    }

    barreira(nthreads);

    for (int i = start; i < end; i++) {
        for (int j = (line_size * i); j < (line_size * i) + line_size; j++) {
            message[j] = aux[j];        
        }
    }
    
    pthread_exit(NULL);
}

void *decryptographMessageConcurrently(void *arg)
{
    int start, end, line_size;
    tArgs *args = (tArgs *)arg;
    start = args->line_start;
    end = args->line_end;
    line_size = args->line_size;
    int value;
    int columnAux;
    char* aux = NULL;

    aux = (char *)malloc(sizeof(char) * (size_message + 1));
    
    for (int i = start; i < end; i++) // Linha da matriz da mensagem
    {
        columnAux = 0;
        for (int j = (line_size * i); j < (line_size * i) + line_size; j++) { //Cada casa da linha
            
            value = 0;
            for (int k = 0; k < dim; k++) // Linha da matriz inversa
            {
                value += message[columnAux + (k * line_size)] * matrixInverse[(k * dim) + i];
            }
            aux[j] = value;
            columnAux++;
        }
        
    }

    barreira(nthreads);

    for (int i = start; i < end; i++) {
        for (int j = (line_size * i); j < (line_size * i) + line_size; j++) {
            message[j] = aux[j];        
        }
    }

    pthread_exit(NULL);
}

void cryptographMessage(char* message) 
{   
    char* aux = NULL;
    int rowsMessage = size_message/dim;     // esse calculo eh referente a matriz de mensagens dever ter o numero de linhas igual ao da matriz lida, para poder realizar a multiplicacao
    cryptoSequencial = (char *) malloc(sizeof(char) * dim * rowsMessage);
    aux = (char *) malloc(sizeof(char) * dim * rowsMessage);
    int value = 0;

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < rowsMessage; j++){
            value = 0;
            for (int x = 0; x < dim; x++){
                value += matrixKey[i*dim+x] * message[x*rowsMessage+j];    // logica para multiplicar a linha da primeira matriz com cada uma das colunas da segunda matriz
            }
            aux[i*rowsMessage+j] = value;
        }
    }

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < rowsMessage; j++){
            cryptoSequencial[i*rowsMessage+j] = aux[i*rowsMessage+j];
        }
    }
}

void decryptographMessage() 
{
    char* aux = NULL;
    int rowsMessage = size_message/dim;     // esse calculo eh referente a matriz de mensagens dever ter o numero de linhas igual ao da matriz lida, para poder realizar a multiplicacao
    decryptoSequencial = (char *) malloc(sizeof(char) * dim * rowsMessage);
    aux = (char *) malloc(sizeof(char) * dim * rowsMessage);
    int value;

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < rowsMessage; j++){
            value = 0;
            for (int x = 0; x < dim; x++){
                value += matrixInverse[i*dim+x] * cryptoSequencial[x*rowsMessage+j];    // logica para multiplicar a linha da primeira matriz com cada uma das colunas da segunda matriz
            }
            aux[i*rowsMessage+j] = value;
        }
    }

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < rowsMessage; j++){
            decryptoSequencial[i*rowsMessage+j] = aux[i*rowsMessage+j];
        }
    }
}

void barreira(int nthreads)
{
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads - 1))
    {
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&x_cond);
        bloqueadas = 0;
    }
    else
    {
        bloqueadas++;
        pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

void checkResults(char* decryptedMessage, char* messageToCheck) 
{
    if (strcmp(decryptedMessage, messageToCheck) == 0)
    {
        printf("Resultado correto!\n");
    }
    else {
        printf("ERRO! O resultado nao eh o esperado");
    }
    
}
            
