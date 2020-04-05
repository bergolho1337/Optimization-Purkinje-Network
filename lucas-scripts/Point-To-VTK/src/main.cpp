#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

struct Point
{
    double x, y, z;
}typedef Point;

vector<Point> readPoints (char *filename)
{
    FILE *inFile = fopen(filename,"r");
    Point p;
    vector<Point> points;
    while (fscanf(inFile,"%lf %lf %lf",&p.x,&p.y,&p.z) != EOF)
        points.push_back(p);
    fclose(inFile);
    return points;
}

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        printf("============================================================================================================\n");
        printf("Programa que converte um arquivo texto com coordenads (x,y,z) em um VTK que representa esses pontos\n\n");
        printf("Usage:> %s <in_file>\n",argv[0]);
        printf("<in_file> = Arquivo de entrada com as coordenadas dos pontos\n");
        printf("<out_file> = 'points.vtk'\n");
        printf("============================================================================================================\n");
        exit(-1);
    }
    else
    {
        vector<Point> points = readPoints(argv[1]);
        FILE *vtkFile = fopen("points.vtk","w+");

        fprintf(vtkFile,"# vtk DataFile Version 3.0\n");
        fprintf(vtkFile,"vtk output\n");
        fprintf(vtkFile,"ASCII\n");
        fprintf(vtkFile,"DATASET POLYDATA\n");
        fprintf(vtkFile,"POINTS %d float\n",(int)points.size());
        for (int i = 0; i < (int)points.size(); i++)
            fprintf(vtkFile,"%.5lf %.5lf %.5lf\n",points[i].x,points[i].y,points[i].z);
        fprintf(vtkFile,"VERTICES %d %d\n",(int)points.size(),(int)points.size()*2);
        for (int i = 0; i < (int)points.size(); i++)
            fprintf(vtkFile,"1 %d\n",i);

        fclose(vtkFile);

    }
    return 0;    
}