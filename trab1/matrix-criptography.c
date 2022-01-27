#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include "./libs/timer.h"

void readMatrix();
void convertMessageToNumberMatrix();
void convertNumberMatrixToText();
void calculateReverseMatrix(); 
void readMessage();
void cryptographMessage();
void decryptographMessage();
void *cryptographMessageConcurrently(void *arg); 
void *decryptographMessageConcurrently(void *arg);
void checkResults();
void printMessage();

int main(int argc, char *argv[]) {}