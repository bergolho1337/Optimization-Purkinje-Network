#ifndef PURKINJE_H
#define PURKINJE_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <ctime>
#include <cstring>
#include <random>
#include "../include/error.h"

// =============================================================================================================
// Parametros do modelo
const double scale_factor = 4;							// Fator de escala para as dimensoes (Especifico para o Ventriculo da Jesuliana)
const int num_terminal = 130;							// Numero de terminais a serem utilizados
const int Iter = 50;									// Numero de iteracoes de crescimento
const double l_bra = 8.0 / scale_factor;				// Tamanho do ramo (mm) (default = 5mm) (artigo = 8mm)
const double w_1 = 0.9;									// Peso especificado pelo usuário, ligado ao quanto a fibra deve desviar
const double t_cube = 2.0;								// Dimensão do mini-cubo de Sobel
const double L = 200.0;									// Lado do cubo que envolve a estrutura da árvore (domínio)
const double sigma_c = 0.2;								// Condutividade padrao citoplasmatica (mS/mm)
const double Toler_tree = 7.0e-00 / scale_factor;		// Tolerancia para colisao na arvore (default = 4) (artigo = 7)
const double Toler_miocardium = 7.0e-00 / scale_factor;	// Tolerancia para colisao no miocardio (default = 7)
const double Toler_terminal = 3.0e-00 / scale_factor;	// Tolerancia para colisao com um terminal (default = 3)
// =============================================================================================================

using namespace std;

struct Edge;
struct Node;

// =============================================================================================================
// Estrutura de uma célula na fibra. É utilizada para armazenar dados da solução da equação do monodomínio.
struct Cell
{
	int type;				// Tipo de célula.
	double *y;				// Resolução da equação no instante n.
	double *y_new;			// Resolução da equação no instante n+1
	double sigma;			// Condutividade
	double V_star;			// V*
}typedef Cell;
// =============================================================================================================
// =============================================================================================================
// Estrutura de um ponto 3D
struct Point
{
	double x, y, z;		// Coordenadas (x,y,z)
	double scalar;		// Valor escalar do ponto
	bool taken;			// Flag para ver se o terminal jah foi verificado
}typedef Point;
// =============================================================================================================
// =============================================================================================================
// Estrutura de uma aresta no grafo
struct Edge
{
	int id;				// Identificador do no destino
	double size;		// Tamanho da aresta, distancia euclidiana
	Edge *next;			// Ponteiro para a proxima aresta
	Node *dest;			// Ponteiro para o no destino
}typedef Edge;

// Funcoes de Edge
Edge* newEdge (int id, double size, Node *dest);
// =============================================================================================================
// =============================================================================================================
// Estrutura de nó do grafo
struct Node
{
	int id;					// Identificador do nodo
	double x, y, z;			// Coordenadas (x,y,z)
	double d_ori[3];		// Vetor da direcao original de crescimento
	int num_edges;			// Contador do número de arestas
	bool grow;				// Controlador se o no esta ou nao em crescimento
	bool isTerminal;		// Flag de um nodo terminal
	Cell *cell;				// Estrutura células contém dados sobre a solução da equação do monodomínio
	Node *next;				// Ponteiro para o próximo nó na lista de nós
	Edge *edges;			// Ponteiro para a lista de arestas
}typedef Node;

// Funcoes de Node
Node* newNode (int id, double x, double y, double z, bool grow, double d_ori[]);
bool checkAlreadyExist (Node *node1, Node *node2);

// =============================================================================================================
// Estrutura do grafo
struct Graph
{
	Node *listNodes;			// Ponteiro para a lista de nós
	Node *lastNode;				// Ponteiro para último nó da lista de nós
	int total_nodes;			// Contador de nós
	int total_edges;			// Contador de arestas
	double Xmax;			    // Valor máximo do domínio no eixo x
	int numCloudPoint;		    // Numero de pontos da nuvem
	int numTerminal;			// Numero de pontos de terminais
	Point *cloudPoint;		    // Vetor contendo a nuvem de pontos do miocardio
	Point *terminalPoint;		// Vetor contendo os pontos terminais
	double *limits;				// Vetor contendo os limites do dominio

}typedef Graph;

// Funcoes do grafo
Graph* newGraph (double dx, char *cloudFileName, char *terminalFileName);
Node* searchNode (Graph *g, int id);
void makeRoot (Graph **g, double *rootPoint, double dx);
Node* insertNodeGraph (Graph **g, Node *prev, double x, double y, double z);
void insertEdgeGraph (Graph **g, int id_1, int id_2);
void growBranch (Graph **g, Node *p);
void Lsystem (Graph **g, Node *p, int type);
void calculateGradient (Graph *g, Node *p, double d_gra[]);
void calculateGrowDirection (Graph **g, Node *p, double d_gra[], double teta);
void generateBranch (Graph **g, Node *p, double d[]);
void joinTerminalPoints (Graph **g);
void countTerminals (Point *terminals, int N);
void linkToMiocardium (Graph **g, Node *last);
int searchMostNear (Point *points, int N, double x, double y, double z);
void setLimits (Graph *g);
void setRootPoint (Point *terminals, int id, double *rootPoint);

// =============================================================================================================
// =============================================================================================================
// Funcoes auxiliares
double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2);
bool isInsideCube (Node *ptr, double x, double y, double z, double width, double lenght, double height);
void initializeRandomGenerator ();
void calcOriginalDirection (double d_ori[], Node *prev, double x, double y, double z);
double calcSizeBranch ();
bool checkCollisionTree (Graph **g, Node *prev, double x, double y, double z);
bool checkCollisionMiocardium (Graph **g, Node *prev, double x, double y, double z);
bool checkLimits (Graph **g, Node *prev, double x, double y, double z);
bool checkTerminals (Graph **g, Node *prev, double x, double y, double z);
bool checkEdge (Node *ptr1, Node *ptr2);
double calcAngle (double u[], double v[]);
void rotate (double u[], double d[], double teta);
// =============================================================================================================

#endif