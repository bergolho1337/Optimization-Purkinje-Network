#include "../include/error.h"

// Escreve uma mensagem de erro e sai do programa
void error (const char *message)
{
    fprintf(stderr,"[-] ERROR ! %s !\n",message);
    exit(EXIT_FAILURE);
}