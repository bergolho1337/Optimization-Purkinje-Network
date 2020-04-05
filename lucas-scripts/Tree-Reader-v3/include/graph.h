#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <vector>

using namespace std;

struct Graph;
struct Edge;
struct Node;
struct InfoTree;
struct StdMean;

// =============================================================================================================
// Estrutura de media e desvio padrao de uma medida
struct StdMean
{
	double mean;								// Media
	double std;									// Desvio padrao
}typedef StdMean;

// =============================================================================================================
// =============================================================================================================
// Estrutura sobre as informacoes da arvore
struct InfoTree
{
	Graph *g;									// Grafo contendo a arvore

	FILE *infoFile;                             // Arquivo de informacao da arvore
	FILE *pmjFile;                              // Arquivo com os PMJs
	FILE *terminalFile;                         // Arquivo com os terminais      
	FILE *segmentFile;                          // Arquivo com todos os segmentos das 10 arvores
	FILE *angleFile;                            // Arquivo com todos os angulos de bifurcacao das 10 arvores
	FILE *edgeFile;                             // Arquivo com todas as aresta das 10 arvores

	vector<double> edges;                       // Vetor com todas arestas
	vector<double> ang_biff;                    // Vetor de todos os angulos de bifurcacoes
	vector<double> size_segments;               // Vetor com o tamanho de todos os segmentos                        

	double total_size;							// Tamanho total da arvore
	StdMean angle;								// Media e desvio padrao dos angulos
	StdMean edge;								// Media e desvio padrao das arestas
	StdMean segment;							// Media e desvio padrao dos segmentos 
	

}typedef InfoTree;

// =============================================================================================================
// =============================================================================================================
// Estrutura de uma aresta no grafo
struct Edge
{
	int id;				// Identificador do no destino
	double w;		    // Tamanho da aresta, distancia euclidiana
	Edge *next;			// Ponteiro para a proxima aresta
	Node *dest;			// Ponteiro para o no destino
}typedef Edge;

// Funcoes de Edge
Edge* newEdge (int id, double w, Node *dest);
// =============================================================================================================
// =============================================================================================================
// Estrutura de nó do grafo
struct Node
{
	int isPMJ;				// Flag para um PMJ
	int id;					// Identificador do nodo
	double x, y, z;			// Coordenadas (x,y,z)
	int num_edges;			// Contador do número de arestas
	Node *next;				// Ponteiro para o próximo nó na lista de nós
	Edge *edges;			// Ponteiro para a lista de arestas
}typedef Node;
// =============================================================================================================
// Funcoes de Node
Node* newNode (int id, double x, double y, double z);
// =============================================================================================================
// Estrutura do grafo
struct Graph
{
	Node *listNodes;			// Ponteiro para a lista de nós
	Node *lastNode;				// Ponteiro para último nó da lista de nós
	int total_nodes;			// Contador de nós
	int total_edges;			// Contador de arestas
}typedef Graph;

// Funcoes do grafo
void initGraph (Graph **g);
Graph* readPurkinjeNetworkFromFile (char *filename);
Node* searchNode (Graph *g, int id);
Node* insertNodeGraph (Graph **g, double p[]);
void insertEdgeGraph (Graph **g, int id_1, int id_2);
void printGraph (Graph *g);
// =============================================================================================================
// =============================================================================================================
// Funcoes auxiliares
void calcEdges (InfoTree *info);
void calcAngles (InfoTree *info);
void calcSegments (InfoTree *info);
void calcTerminals (InfoTree *info);
void calcStatistics (InfoTree *info);
void calcInfoTree (InfoTree *info);
void calcMeanStd (vector<double> v, StdMean &s);

double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2);
void error (char *msg);
//void calcInfoTree (Graph *g);
void calcNumberSegments (Graph *g);
double calcTotalSize (Graph *g);
double calcMeanAngleBif (Graph *g);
double calcAngle (double u[], double v[]);
void calcActivationPMJ (Graph *g, int *num_actives, int *num_inactives);
void buildVector (Node *ptr, Edge *ptrl, double v[]);
void dfs (Node *u);
void countTerminal (Graph *g);
// =============================================================================================================