/* ----------------------------------------------------------------------------------------------------
	MODELO MACROSCOPICO DAS FIBRAS DE PURKINJE ACOPLADO A UM MODELO DE MIOCARDIO (BENJAMIM LIU)
   ----------------------------------------------------------------------------------------------------
	1) Geracao automatica das fibras eh feita usando o L-System modificado;
	2) Modelo celular utilizado eh o de Mitchell Shaeffer (Two Current Model);
	3) Resolucao da equacao do monodominio nas celulas eh feita por volumes finitos;
   ----------------------------------------------------------------------------------------------------
	Try example: ./purkinjeMiocardium 500 2.0 0.05 
*/

/*
 ***********************************************************************************
	FALTA IMPLEMENTAR A PARTE RELACIONADA A SOLUCAO DA EQUACAO DO MONODOMINIO
	POR ENQUANTO SO ESTOU GERANDO A ARVORE DE PURKINJE SOBRE UM MIOCARDIO
 ***********************************************************************************
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "../include/macroscopic.h"

using namespace std;

int main (int argc, char *argv[])
{

    printf("================= MACROSCOPIC PURKINJE MODEL COUPLED MIOCARDIUM ====================\n");
	if (argc-1 < 5)
	{
		printf("Usage:> ./purkinjeMiocardium <t_max> <Dx> <Dt> <cloudPointFile> <terminalPointFile>\n");
        printf("------------------------------------------------------------------------------------\n");
		printf("t_max = Maximum time of the simulation. (ms)\n");
		printf("Dx = Size of the discretization on space. (mm)\n");
		printf("Dt = Size of the discretization on time. (ms)\n");
		printf("cloudPointFile = caminho para o .vtk contendo a malha do miocardio.\n");
		printf("terminalPointFile = caminho para o .txt contendo os pontos terminais da rede de Purkinje.\n");
		printf("Example: ./purkinjeMiocardium 500 2.0 0.05 Miocardium/miocardium.vtk Terminals/term-ORI.txt \n");
	}
	else
	{
		MacroscopicModel *model = newMacroscopicModel(argc,argv);
		//copyFilesToFMMDirectory();
	}
    printf("====================================================================================\n");
	return 0;
}