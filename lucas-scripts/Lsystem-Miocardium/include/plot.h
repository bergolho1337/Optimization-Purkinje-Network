#ifndef PLOT_H
#define PLOT_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../include/purkinje.h"
//#include "../include/monodomain.h"
//#include "queue.h"

using namespace std;

void printGraph (Graph *g);
void writeGraphVTK (Graph *g);
void writeTerminalsVTK (Graph *g);
/*
void printInfoModel (Graph *g, double *y0, double t_max, double Dt, double Dx, int exec_number);
void writeVTK (Graph *g, int k, int exec_number);
Node** setPlotPointers (Graph *g);
void writeGraphic (Graph *g, double t, Node **plot, int num_eq, int exec_number);
void makePlot (Node **plot, int exec_number);
*/

#endif