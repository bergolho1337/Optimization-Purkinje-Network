#include <cstdio>
#include <cstdlib>
#include "../include/error.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 4)
    {
        printf("==========================================================================================================================\n");
        printf("Usage:> %s <analitical_result.vtk> <aproximation_result.vtk> <analitical_terminals> <aproximation_terminals>\n",argv[0]);
        printf("<analitical_result.vtk> = Arquivo .vtk contendo a solucao analitica\n");
        printf("<aproximation_result.vtk> = Arquivo .vtk contendo a solucao aproximada\n");
        printf("<analitical_terminals.txt> = Arquivo .txt contendo as coordenadas dos terminais da solucao analitica\n");
        printf("<aproximation_terminals.txt> = Arquivo .txt contendo as coordenadas dos terminais da solucao aproximada\n");
        printf("--------------------------------------------------------------------------------------------------------------------------\n");
        printf("Exemplo: ./errorCalculator Artigo/Benjamin/FMM_130.vtk Artigo/L-System/1/FMM_130.vtk Artigo/Benjamin/term.txt Artigo/L-System/1/terminals.txt\n");
        printf("--------------------------------------------------------------------------------------------------------------------------\n");
        printf("Calcula o erro entre a aproximacao feita pelo GMSH e a arvore original do Benjamin\n");
        printf(" !!!! O arquivo .vtk deve estar representando o numero em ponto flutuante com '.' nao com ',' !!!\n");
        printf("==========================================================================================================================\n");
        exit(-1);
    }
    else
    {
        ErrorCalculator *error = newErrorCalculator(argc,argv);
        calculateError(error);
        //calculateErrorTerminals(error);
        freeMemory(error);
        return 0;
    }
}
