// Programa que le o .vtk de uma arvore de Purkinje e retorna as suas informacoes.
//  - Numero de vertices
//  - Numero de arestas
//  - Numero de segmentos
//  - Numero de PMJ ativos
//  - Numero de PMJ inativos
//  - Tamanho do angulo das bifurcacoes

#include <cstdio>
#include <cstdlib>
#include "../include/graph.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        printf("============================================================\n");
        printf("Usage:> %s <in_VTK_file>\n",argv[0]);
        printf("<in_VTK_file> = Arquivo contendo a arvore em '.vtk'\n");
        printf("Try for example:> %s Tree_Nterm.vtk\n",argv[0]);
        printf("============================================================\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        InfoTree *info = new InfoTree();
        info->g = readPurkinjeNetworkFromFile(argv[1]);
        calcEdges(info);
        calcAngles(info);
        calcSegments(info);
        calcTerminals(info);
        calcStatistics(info);
        calcInfoTree(info);
        printf("[!] Arquivos salvos na pasta Output !\n");
        return 0;
    }
}