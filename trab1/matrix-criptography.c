#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

void readMatrix();
void convertMessageToNumberMatrix(char** message);
void convertNumberMatrixToText(char** matrixMessage);
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
    printf("%s\n\n",message);
    convertMessageToNumberMatrix(&message);
    printf("MENSAGEM EM NUMEROS: \n\n");
    for(int i = 0; i < 25; i++) {
        printf("%d ", message[i]);
    }
    printf("\n\n");
    printf("MENSAGEM EM TEXTO: \n\n");
    convertNumberMatrixToText(&message);
    printf("%s \n\n", message);

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