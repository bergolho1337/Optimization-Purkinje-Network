#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

struct Point
{
    double x, y, z;                     // Coordenadas (x,y,z)
    double aTime;                       // Tempo de ativacao
}typedef Point;

struct Polygons
{
    int id1;                            // Ids que formam o poligono
    int id2;
    int id3;
}typedef Polygons;

struct vtkFile
{
    Point *points;                      // Vetor de pontos do .vtk
    int N;                              // Numero de pontos
    Polygons *polygons;                 // Vetor dos poligonos do .vtk
    int M;                              // Numero de poligonos
    Point *terminals;                   // Vetor de pontos terminais do .vtk
    int T;
}typedef vtkFile;

struct ErrorCalculator
{
    vtkFile *analit;                        // Solucao analitica         
    vtkFile *aprox;                         // Solucao aproximada
    vtkFile *error;                         // Erro
}typedef ErrorCalculator;

ErrorCalculator* newErrorCalculator (int argc, char *argv[]);
void readInput (ErrorCalculator *e, char *argv[]);
vtkFile* readFile (char atFile[], char termFile[]);
int* searchTerminalIds (vtkFile *f);
void errorMsg ();
void calculateError (ErrorCalculator *e);
void calculateErrorTerminals (ErrorCalculator *e);
Point* readTerminals (int &numTerminals);
double calcDistance (Point p1, Point p2);
void freeMemory (ErrorCalculator *error);