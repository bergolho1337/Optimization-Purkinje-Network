#include "../include/miocardium.h"

// Realiza a leitura dos pontos do miocardio e armazena isso em um vetor de Point
// Retorna uam referencia para o ponto que representa o centroide da estrutura
double* readCloudPoints (Graph *g, char *filename)
{
	printf("[!] Reading cloud of points ...\n");
	FILE *file;
	char str[50];
	int i, n;
	double x, y, z, scalar;
	double center_x, center_y, center_z;
	double *rootPoint;
	file = fopen(filename,"r");
	if (file == NULL) error("In readCloudPoints()");

	center_x = 0.0;
	center_y = 0.0;
	center_z = 0.0;

	// Aloca memoria para o ponto da raiz da arvore
	rootPoint = (double*)calloc(3,sizeof(double));

	// Acha o inicio da lista de pontos no arquivo
	while (fscanf(file,"%s",str) != EOF)
		if (strcmp(str,"POINTS") == 0) break;
	
	// Le o numero de pontos e aloca memoria para o vetor de pontos
	if (!fscanf(file,"%d",&n) ) error("In readCloudPoints(), reading cloud points");
	if (!fscanf(file,"%s",str)) error("In readCloudPoints(), reading cloud points");
	g->cloudPoint = (Point*)malloc(sizeof(Point)*n);
	g->numCloudPoint = n;

	for (i = 0; i < n; i++)
	{
		if (!fscanf(file,"%lf %lf %lf",&x,&y,&z)) error("In readCloudPoints(), reading cloud points");
		g->cloudPoint[i].x = x;
		g->cloudPoint[i].y = y;
		g->cloudPoint[i].z = z;
		g->cloudPoint[i].taken = false;
		center_x += x;
		center_y += y;
		center_z += z;
	}

	// Calcular o ponto medio
	rootPoint[0] = center_x / n;
	rootPoint[1] = center_y / n;
	rootPoint[2] = center_z / n;

	// Acha o inicio da lista de escalares dos pontos
	while (fscanf(file,"%s",str) != EOF)
		if (strcmp(str,"default") == 0) break;

	// Ler os escalares dos pontos
	for (i = 0; i < n; i++)
	{
		if (!fscanf(file,"%lf",&scalar)) error("In readCloudPoints(), reading cloud points");
		g->cloudPoint[i].scalar = scalar;
	}
	fclose(file);

	// Ordena os pontos em crescente em relacao ao scalar (tempo de ativacao no ventriculo)
	qsort(g->cloudPoint,n,sizeof(Point),compare);

	//rootPoint[0] = g->cloudPoint[0].x;
	//rootPoint[1] = g->cloudPoint[0].y;
	//rootPoint[2] = g->cloudPoint[0].z;

	printf("Number of cloud points = %d\n",n);
	printf("\tRoot point at:\n");
	printf("\tx = %lf\n",rootPoint[0]);
	printf("\ty = %lf\n",rootPoint[1]);
	printf("\tz = %lf\n",rootPoint[2]);

    // Imprime os pontos de ativacao
    //printActivationPoints(g->cloudPoint,20);

	return rootPoint;

}

// Ler os pontos terminais da arvore a partir do arquivo
void readTerminalPoints (Graph *g, char *filename)
{
	int N, M;
	double x, y, z;
	FILE *file = fopen(filename,"r");
	if (file == NULL) error("In readTerminalPoints(), opening file");

	if (!fscanf(file,"%d %d",&N,&M)) error("In readTerminalPoints(), reading terminal points");
	g->numTerminal = N;
	g->terminalPoint = (Point*)malloc(sizeof(Point)*N);
	for (int i = 0; i < N; i++)
	{
		if (!fscanf(file,"%lf %lf %lf",&x,&y,&z)) error("In readTerminalPoints(), reading terminal points");
		g->terminalPoint[i].x = x;
		g->terminalPoint[i].y = y;
		g->terminalPoint[i].z = z;
		g->terminalPoint[i].taken = false;
	}
	fclose(file);
	//for (int i = 0; i < N; i++)
	//	printf("Terminal %d = (%lf,%lf,%lf,%d)\n",i,g->terminalPoint[i].x,g->terminalPoint[i].y,g->terminalPoint[i].z,(int)g->terminalPoint[i].taken);
}

// Funcao de comparacao do QuickSort
int compare (const void *a, const void *b)
{
	Point *pA = (Point*)a;
	Point *pB = (Point*)b;

	if (pA->scalar < pB->scalar)
      return -1;
   	else if (pA->scalar > pB->scalar)
      return 1;
   	else
      return 0;
}

// Imprime os N primeiros pontos de ativacao do tecido
void printActivationPoints (Point *points, int N)
{
    printf("[!] Pontos de ativacao (N = %d)\n",N);
    for (int i = 0; i < N; i++)
        printf("\t(%lf,%lf,%lf) = %lf\n",points[i].x,points[i].y,points[i].z,points[i].scalar);
}