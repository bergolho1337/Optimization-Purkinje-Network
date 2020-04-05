#ifndef MACROSCOPIC_H
#define MACROSCOPIC_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "../include/purkinje.h"
#include "../include/error.h"
//#include "monodomain.h"
#include "../include/plot.h"
// Deve incluir um modelo celular
//#include "../include/mitchell.h"
//#include "noble.h"

using namespace std;

// Define o tipo Func, ponteiro para função que recebe t, um vetor y e o intervalo de tempo k como parâmetro e retorna a avaliação da função
typedef
	double (*Func) (double t, double *y);

// =============================================================================================================
// Estrutura para o resolvedor do modelo
struct MacroscopicModel
{
  Graph *g;                   // Referencia para o grafo contendo a arvore de Purkinje
  double *y0;                 // Vetor contendo as condicoes inicias do modelo celular
  double dx;                  // Tamanho do subintervalo no espaco
  double dt;                  // Tamanho do subintervalo no tempo
  double t_max;               // Tempo maximo de simulacao
  Func *functions;            // Vetor de funcoes do modelo celular
  Node **plot_nodes;          // Referencia para os nos que serao plotados
  char cloudPointFile[100];   // Nome do arquivo contendo os pontos do miocardio
  char terminalPointFile[100];// Nome do arquivo contendo os pontos do terminais de Purkinje

}typedef MacroscopicModel;

// Funcoes do resolvedor
MacroscopicModel* newMacroscopicModel (int argc, char *argv[]);
void createDirectories ();
void copyFilesToFMMDirectory ();
void executeFMM ();
void solve ();
// =============================================================================================================

#endif