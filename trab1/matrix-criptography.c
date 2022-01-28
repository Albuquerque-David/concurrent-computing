#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

void readMatrix();
int* convertMessageToNumberMatrix(char* message, int size);
char* convertNumberMatrixToText(int* matrixMessage, int size);
void calculateReverseMatrix();
char* readMessage(char *filename);
void cryptographMessage();
void decryptographMessage();
void *cryptographMessageConcurrently(void *arg);
void *decryptographMessageConcurrently(void *arg);
void checkResults();
void printMessage();

float *mat1;
float *mat2;
float *matSequential;
float *matConcurrent;
int nthreads;
int size_message;

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
        printf("Digite: %s <numero de threads> <arquivo a ser criptografado>\n", argv[0]);
        return 1;
    }

    nthreads = atoi(argv[1]);
    filename = argv[2];

    if (nthreads <= 0)
    {
        printf("Entre com um numero positivo de threads\n");
        exit(1);
    }

    message = readMessage(filename);
    matrixMessage = convertMessageToNumberMatrix(message, size_message);
    printf("MENSAGEM EM NUMEROS: \n\n");
    for(int i = 0; i < 25; i++) {
        printf("%d ", matrixMessage[i]);
    }
    printf("\n\n");
    printf("MENSAGEM EM TEXTO: \n\n");
    textMessage = convertNumberMatrixToText(matrixMessage, size_message);
    printf("%s \n\n", textMessage);

    return 0;
}

char* readMessage(char *filename)
{

    char *message = NULL;
    int string_size, read_size;
    FILE *file = fopen(filename, "r");

    if(file == NULL){
        perror("fopen");
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

int* convertMessageToNumberMatrix(char* message, int size) {

    int i;
    int *numberMessage;

    numberMessage = (int *)malloc(sizeof(int) * size);
    i = 0;

    while(message[i] != '\0') {

        if(message[i] >= 65 && message[i] <= 90) {
            numberMessage[i] = message[i] - 65;
            i++;
            continue;
        }

        if(message[i] >= 97 && message[i] <= 122) {
            numberMessage[i] = message[i] - 97;
            i++;
            continue;
        }

        numberMessage[i] = 92;
        i++;

    }

    return numberMessage;
}

char* convertNumberMatrixToText(int* matrixMessage, int size) {
    char *textMessage;

    textMessage = (char *)malloc(sizeof(char) * size);

    for(int i = 0; i < size; i++) {
        textMessage[i] = matrixMessage[i] + 65;
    }

    return textMessage;
}