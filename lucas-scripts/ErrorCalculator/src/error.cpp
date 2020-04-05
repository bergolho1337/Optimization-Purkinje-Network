#include "../include/error.h"

ErrorCalculator* newErrorCalculator (int argc, char *argv[])
{
    printf("=============================================================================\n");
    ErrorCalculator *error = (ErrorCalculator*)malloc(sizeof(ErrorCalculator));
    readInput(error,argv);
    printf("=============================================================================\n");
    return error;
}

void readInput (ErrorCalculator *e, char *argv[])
{
    printf("[!] Reading input files ... ");
    fflush(stdout);
    e->analit = readFile(argv[1],argv[3]); 
    e->aprox = readFile(argv[2],argv[4]);
    printf("ok\n");
}

vtkFile* readFile (char atFile[], char termFile[])
{
    // Ler o arquivo dos tempos de ativacao
    char str[500], str2[500];
    int trash;
    FILE *in = fopen(atFile,"r");
    vtkFile *vtk = (vtkFile*)malloc(sizeof(vtkFile));

    // Ler as coordenadas dos pontos
    while (fscanf(in,"%s",str) != EOF)
        if (strcmp(str,"POINTS") == 0) break;
    if (!fscanf(in,"%d",&vtk->N)) errorMsg();
    vtk->points = (Point*)malloc(sizeof(Point)*vtk->N);
    if (!fscanf(in,"%s",str)) errorMsg();
    for (int i = 0; i < vtk->N; i++)
        if (!fscanf(in,"%lf %lf %lf",&vtk->points[i].x,&vtk->points[i].y,&vtk->points[i].z)) errorMsg();

    // Ler os poligonos
    if (!fscanf(in,"%s %d %s",str,&vtk->M,str2)) errorMsg();
    vtk->polygons = (Polygons*)malloc(sizeof(Polygons)*vtk->M);
    for (int i = 0; i < vtk->M; i++)
        if (!fscanf(in,"%d %d %d %d",&trash,&vtk->polygons[i].id1,&vtk->polygons[i].id2,&vtk->polygons[i].id3)) errorMsg();
    
    // Ler os tempos de ativacao dos pontos
    while (fscanf(in,"%s",str) != EOF)
        if (strcmp(str,"default") == 0) break;
    for (int i = 0; i < vtk->N; i++)
        if (!fscanf(in,"%lf",&vtk->points[i].aTime)) errorMsg();
    fclose(in);
    
    // Ler o arquivo com os pontos terminais
    /*
    printf("[!] Reading file %s ... \n",atFile);
    in = fopen(termFile,"r");
    if (!fscanf(in,"%d",&vtk->T)) errorMsg();
    vtk->terminals = (Point*)malloc(sizeof(Point)*vtk->T);
    for (int i = 0; i < vtk->T; i++)
        if (!fscanf(in,"%lf %lf %lf",&vtk->terminals[i].x,&vtk->terminals[i].y,&vtk->terminals[i].z)) errorMsg();
    fclose(in);
    */
    // Retornar a estrutura montada 
    return vtk;
}

void errorMsg ()
{
    printf("[-] Error found!\n");
    exit(-1);
}

// Calcula o erro de todo o miocardio
void calculateError (ErrorCalculator *e)
{   
    // Alocar as estruturas para o arquivo de saida
    int N = e->analit->N;
    int M = e->analit->M;
    double mean_error = 0; 
    double error[N];
    e->error = (vtkFile*)malloc(sizeof(vtkFile));
    e->error->points = (Point*)malloc(sizeof(Point)*N);
    e->error->N = N;
    e->error->polygons = (Polygons*)malloc(sizeof(Polygons)*M);
    e->error->M = M;

    // Escrever o arquivo de saida (erro)
    FILE *out = fopen("Output/error.vtk","w+");
    fprintf(out,"# vtk DataFile Version 3.0\n");
    fprintf(out,"vtk output\n");
    fprintf(out,"ASCII\n");
    fprintf(out,"DATASET POLYDATA\n");
    fprintf(out,"POINTS %d float\n",N);
    // Pontos
    for (int i = 0; i < N; i++)
        fprintf(out,"%e %e %e\n",e->analit->points[i].x,e->analit->points[i].y,e->analit->points[i].z);
    // Poligonos 
    fprintf(out,"POLYGONS %d %d\n",M,M*4);
    for (int i = 0; i < M; i++)
        fprintf(out,"3 %d %d %d\n",e->analit->polygons[i].id1,e->analit->polygons[i].id2,e->analit->polygons[i].id3);
    // Tempo de ativacao: erro = fabs(analit - aprox)
    fprintf(out,"POINT_DATA %d\n",N);
    fprintf(out,"SCALARS error float\n");
    fprintf(out,"LOOKUP_TABLE default\n");
    for (int i = 0; i < N; i++)
    {
        error[i] = fabs(e->analit->points[i].aTime - e->aprox->points[i].aTime);
        mean_error += error[i];
        fprintf(out,"%.60lf\n",error[i]);
    }
    // Computar a media e o desvio padrao 
    mean_error /= (double)N;
    double std_error = 0;
    for (int i = 0; i < N; i++)
        std_error += pow(error[i] - mean_error,2);
    std_error = sqrt(std_error / (double)N);

    printf("=============== GLOBAL ERROR =====================\n");
    printf("Media = %.6lf\n",mean_error);
    printf("Desvio padrao = %.6lf\n",std_error);
    printf("==================================================\n");
        
    fclose(out);

    // Imprimir o erro de todos os pontos de todas as arvores
    FILE *out2 = fopen("Output/globalError.txt","a");
    for (int i = 0; i < N; i++)
        fprintf(out2,"%.60lf\n",error[i]);
    fclose(out2);
}

void freeMemory (ErrorCalculator *e)
{
    printf("[!] Freeing memory ...");
    fflush(stdout);
    free(e->analit->points);
    free(e->analit->polygons);
    //free(e->analit->terminals);
    free(e->aprox->points);
    free(e->aprox->polygons);
    //free(e->aprox->terminals);
    free(e->error->points);
    free(e->error->polygons);
    free(e->analit);
    free(e->aprox);
    free(e->error);
    free(e);
    printf("ok\n");
}

/*
void calculateErrorTerminals (ErrorCalculator *e)
{
    // Localizar os ids dos pontos terminais no ventriculo
    int *idTermAnalit = searchTerminalIds(e->analit);
    int *idTermAprox = searchTerminalIds(e->aprox);

    vtkFile *vtkAnalit = e->analit;
    vtkFile *vtkAprox = e->aprox;
    int T = e->aprox->T;
    double erro[T];
    double M = 0;

    // Escrever o arquivo de saida (erro)
    FILE *out = fopen("Output/errorTerminals.vtk","w+");
    fprintf(out,"# vtk DataFile Version 3.0\n");
    fprintf(out,"vtk error terminals\n");
    fprintf(out,"ASCII\n");
    fprintf(out,"DATASET POLYDATA\n");
    // Pontos
    fprintf(out,"POINTS %d float\n",T);
    for (int i = 0; i < T; i++)
        fprintf(out,"%e %e %e\n",vtkAprox->terminals[i].x,vtkAprox->terminals[i].y,vtkAprox->terminals[i].z);
    // Vertices
    fprintf(out,"VERTICES %d %d\n",T,T*2);
    for (int i = 0; i < T; i++)
        fprintf(out,"1 %d\n",i);
    // Tempo de ativacao: erro = fabs(analit - aprox)
    fprintf(out,"POINT_DATA %d\n",T);
    fprintf(out,"SCALARS error float\n");
    fprintf(out,"LOOKUP_TABLE default\n");
    for (int i = 0; i < T; i++)
    {
        erro[i] = fabs(vtkAnalit->points[idTermAnalit[i]].aTime - vtkAprox->points[idTermAprox[i]].aTime);
        M += erro[i];
        fprintf(out,"%.60lf\n",erro[i]);
    }
    
    fclose(out);    
    
    // Computar a media e o desvio padrao 
    M /= (double)T;
    double D = 0;
    for (int i = 0; i < T; i++)
        D += pow(erro[i] - M,2);
    D = sqrt(D / (double)T);

    printf("============= TERMINAL ERROR =====================\n");
    printf("Media = %.6lf\n",M);
    printf("Desvio padrao = %.6lf\n",D);
    printf("==================================================\n");
    
    free(idTermAprox);
    free(idTermAnalit);
}

// Mapear os ids dos terminais no miocardio
int* searchTerminalIds (vtkFile *vtk)
{
    int i, j;
    int *idTerm = (int*)malloc(sizeof(int)*vtk->T);
    // Para cada terminal
    for (i = 0; i < vtk->T; i++)
    {
        int minId = -1;
        double minDist = 1.0e+100;
        // Percorrer todos os pontos do miocardio comparando com a coordenada de cada terminal
        for (j = 0; j < vtk->N; j++)
        {
            double dist = calcDistance(vtk->terminals[i],vtk->points[j]);
            if (dist < minDist)
            {
                minDist = dist;
                minId = j;
            }
        }
        idTerm[i] = minId; 
    }

    return idTerm;
}

double calcDistance (Point p1, Point p2)
{
    return sqrt( pow(p1.x - p2.x,2) + pow(p1.y - p2.y,2) + pow(p1.z - p2.z,2) );
}
*/