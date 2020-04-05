#ifndef MIOCARDIUM_H
#define MIOCARDIUM_H

using namespace std;

#include "../include/purkinje.h"
#include "../include/error.h"

// Funcoes que trabalham nos dados do miocardio
int compare (const void *a, const void *b);
double* readCloudPoints (Graph *g, char *filename);
void readTerminalPoints (Graph *g, char *filename);
void printActivationPoints (Point *points, int N);

#endif