#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

void readMatrix();
void convertMessageToNumberMatrix();
void convertNumberMatrixToText();
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

    readMessage(filename);

    return 0;
}

char* readMessage(char *filename)
{

    char *buffer = NULL;
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
        buffer = (char *)malloc(sizeof(char) * (string_size + 1));
        read_size = fread(buffer, sizeof(char), string_size, file);

        buffer[string_size] = '\0';

        if (string_size != read_size)
        {
            free(buffer);
            buffer = NULL;
        }

        fclose(file);
    }

    printf("%s", buffer);

    return buffer;
}