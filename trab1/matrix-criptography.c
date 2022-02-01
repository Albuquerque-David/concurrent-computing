#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

void readMatrixKey();
void readMatrixInverse();
void convertMessageToNumberMatrix(char** message);
void convertNumberMatrixToText(char** matrixMessage);
char* readMessage(char *filename);
void cryptographMessage(char* message);
void decryptographMessage();
void *cryptographMessageConcurrently(void *arg);
void *decryptographMessageConcurrently(void *arg);
void checkResults();
void printMessage();


int *cryptoSequencial;
int *decryptoSequencial;
int *matConcurrent;
int nthreads;
int size_message;
int *matrixKey;
int *matrixInverse;
int dim;

typedef struct
{
    int id;
    int start, end;
} tArgs;

int main(int argc, char *argv[])
{
    

    pthread_t *tid;
    tArgs *args;
    double start, end, delta;
    char* filename;
    char* message;
    char* textMessage;
    int* matrixMessage;


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

    // --------------------------
    // === Leitura do Arquivo ===
    // --------------------------
    
    GET_TIME(start);

    message = readMessage(filename);

    GET_TIME(end);
    delta = end - start;
    printf("A leitura do arquivo levou: %lf ms\n", delta);

    // -------------------------------------
    // === Conversão do texto em números ===
    // -------------------------------------

    GET_TIME(start);

    convertMessageToNumberMatrix(&message);

    GET_TIME(end);
    delta = end - start;
    printf("A conversao de texto em numeros levou: %lf ms\n", delta);
    for (int i = 0; i < size_message; i++)
    {
        printf("%d ", message[i]);
    }
    
    // --------------------------------
    // === Input de dados da matriz ===
    // --------------------------------
    
    readMatrixKey();
    readMatrixInverse();
    // print para teste
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d ", matrixKey[i*dim+j]);
        }
        printf("\n");
    }

    cryptographMessage(message);
    for (int i = 0; i < size_message; i++)
    {
        printf("%d ", cryptoSequencial[i]);
    }

    printf("\n");

    decryptographMessage();
    for (int i = 0; i < size_message; i++)
    {
        printf("%d ", decryptoSequencial[i]);
    }
    
    // --------------------------
    // === Leitura do arquivo ===
    // --------------------------

    // --------------------------------------
    // === Conversão dos números em texto ===
    // --------------------------------------

    GET_TIME(start);

    convertNumberMatrixToText(&message);

    GET_TIME(end);
    delta = end - start;
    printf("A conversao dos numeros em texto levou: %lf ms\n", delta);

    return 0;
}

char* readMessage(char *filename)
{

    char *message = NULL;
    int string_size, read_size;
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        fprintf(stderr, "Falha ao abrir o arquivo: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    if (file) {

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

void convertMessageToNumberMatrix(char** message) {

    int i = 0;
    while((*message)[i] != '\0') {
        if ((*message)[i] >= 65 && ((*message)[i]) <= 90)
        {
            (*message)[i] -= 64;
            i++;
            continue;
        }

        if((*message)[i] >= 97 && (*message)[i] <= 122) {
            (*message)[i] -= 96;
            i++;
            continue;
        }

        (*message)[i] = 35;
        i++;
        printf("M%c", (*message)[i]);
    }

}

void convertNumberMatrixToText(char** matrixMessage) {

    int i = 0;

    while ((char)(*matrixMessage)[i] != (char)'\0') {

        if ((*matrixMessage)[i] == 35)
        {
            i++;
            continue;
        }

        (*matrixMessage)[i] += 64;
        i++;
    }
}

void readMatrixKey() {
    int i = 0, j = 0;

    printf("Para criptografar sua mensagem, eh preciso de uma matriz chave. Entre com a dimensao da sua matriz quadrada:\n");
    scanf("%d", &dim);
    
    printf("Agora, digite um a um cada elemento inteiro da matriz:\n");
    matrixKey = (int *) malloc(sizeof(int) * dim * dim);
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            scanf("%d", &matrixKey[i*dim+j]);
        }
    }
}


void readMatrixInverse() {
    int i = 0, j = 0;

    printf("Agora, digite um a um cada elemento inteiro da matriz inversa de sua chave:\n");
    matrixInverse = (int *) malloc(sizeof(int) * dim * dim);
    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            scanf("%d", &matrixInverse[i*dim+j]);
        }
    }
}

void cryptographMessage(char* message) 
{
    int rowsMessage = size_message/dim;     // esse calculo eh referente a matriz de mensagens dever ter o numero de linhas igual ao da matriz lida, para poder realizar a multiplicacao
    cryptoSequencial = (int *) malloc(sizeof(int) * dim * rowsMessage);

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < rowsMessage; j++){
            for (int x = 0; x < dim; x++){
                cryptoSequencial[i*rowsMessage+j] += matrixKey[i*dim+x] * message[x*rowsMessage+j];    // logica para multiplicar a linha da primeira matriz com cada uma das colunas da segunda matriz
            }
        }
    }
}

void decryptographMessage() 
{
    int rowsMessage = size_message/dim;     // esse calculo eh referente a matriz de mensagens dever ter o numero de linhas igual ao da matriz lida, para poder realizar a multiplicacao
    decryptoSequencial = (int *) malloc(sizeof(int) * dim * rowsMessage);

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < rowsMessage; j++){
            for (int x = 0; x < dim; x++){
                decryptoSequencial[i*rowsMessage+j] += matrixInverse[i*dim+x] * cryptoSequencial[x*rowsMessage+j];    // logica para multiplicar a linha da primeira matriz com cada uma das colunas da segunda matriz
            }
        }
    }
}

   