#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

int *matSequential;
int *matConcurrent;
int nthreads;
int size_message;
int *matrixKey;
int *matrixInverse;
int dim;
int *matrixMessage;
char *message;

typedef struct
{
    int id;
    int start, end, line_size;
} tArgs;

int main(int argc, char *argv[])
{
    pthread_t *tid;
    tArgs *args;
    double start, end, delta;
    char *filename;
    
    char *textMessage;

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
    //print para teste
    printf("%s\n", message);

    // -------------------------------------
    // === Conversão do texto em números ===
    // -------------------------------------

    GET_TIME(start);

    convertMessageToNumberMatrix(&message);

    GET_TIME(end);
    delta = end - start;
    printf("A conversao de texto em numeros levou: %lf ms\n", delta);
    // print para teste
    for (int j = 0; j < size_message; j++)
    {
        printf("%d ", message[j]);
    }
    printf("\n");

    // --------------------------------
    // === Input de dados da matriz ===
    // --------------------------------

    readMatrixKey();
    readMatrixInverse();
    // print para teste
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            printf("i: %d j: %d valor: %d ", i,j,matrixKey[i * dim + j]);
        }
        printf("\n");
    }

    // ------------------------------
    // === Criptografa Sequencial ===
    // ------------------------------

    // -------------------------------
    // === Criptografa Concorrente ===
    // -------------------------------
    printf("ta de sacanagem q n chega aqui\n");
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
        (args + thread)->start = thread * ((size_message) / nthreads);
        (args + thread)->end = (thread * ((size_message) / nthreads)) + ((size_message) / nthreads);
        (args + thread)->line_size = sizeLinePerThread;
        (args + thread)->id = thread;
        printf("thread %d: sizem %d nthreads %d dim %d\n", thread, size_message, nthreads, dim);
        printf("thread %d: start %d end %d \n", thread, (args + thread)->start, (args + thread)->end);

        if (pthread_create(tid + thread, NULL, cryptographMessageConcurrently, (void *)(args + thread)))
        {
            fprintf(stderr, "Erro ao criar threads para a funcao cryptographMessageConcurrently\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf("Thread criada com sucesso. Tid: %ld\n",*(tid + thread));
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
    printf("A multiplicacao concorrente levou: %lf ms\n", delta);
    for (int j = 0; j < size_message; j++)
    {
        printf("%d ", message[j]);
    }
    printf("\n\n");

    // --------------------------------------
    // === Conversão dos números em texto ===
    // --------------------------------------

    GET_TIME(start);

    convertNumberMatrixToText(&message);

    GET_TIME(end);
    delta = end - start;
    printf("A conversao dos numeros em texto levou: %lf ms\n", delta);

    free(tid);

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
    printf("Para criptografar sua mensagem, eh preciso de uma matriz chave. Entre com a dimensao da sua matriz quadrada:\n");
    scanf("%d", &dim);
    printf("Agora, digite um a um cada elemento inteiro da matriz:\n");
    matrixKey = (int *)malloc(sizeof(int) * dim * dim);
    for (i = 0; i < dim; i++)
    {
        for (j = 0; j < dim; j++)
        {
            printf("i: %d j: %d dim %d total %d\n", i, j, dim, i * dim + j);
            scanf("%d", &matrixKey[i * dim + j]);
        }
    }
}

void readMatrixInverse()
{
    int i = 0, j = 0;
    printf("Agora, digite um a um cada elemento inteiro da matriz inversa de sua chave:\n");
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
    start = args->start;
    end = args->end;
    line_size = args->line_size;
    int initialValue;
    int line = 1;

    for (int i = start; i < end; i++)
    {
        if (i >= ((dim / line * dim)))
            initialValue = message[i];
        for (int k = 0; k < dim; k++)
        {
           
            message[i] += message[i] * matrixKey[(line*dim) + k];
        }   
        message[i] -= initialValue;
    }

    pthread_exit(NULL);
}

void *decryptographMessageConcurrently(void *arg)
{
}
