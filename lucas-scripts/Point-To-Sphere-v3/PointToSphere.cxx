// =====================================================================================================
// Realiza a leitura dos pontos em um arquivo .txt e gera um outro arquivo .vtp contendo esferas
// centradas nestes pontos.
// =====================================================================================================


#include <iostream>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
 
using namespace std;

#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
#include <vtkVersion.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkAppendPolyData.h>
#include <vtkFloatArray.h>
#include <vtkPolyDataMapper.h>

struct Point;
void readPoints (int argc, char *argv[], vector< vector<Point> > &points);
void createSpheres (vector< vector<Point> > points);

struct Point
{
	double x,y,z;
	Point(const double xin, const double yin, const double zin) : x(xin), y(yin), z(zin) {}
};

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        printf("================================================================================\n");
        printf("Realiza a leitura dos pontos em um arquivo .txt e gera um outro\n"); 
        printf("arquivo '.vtp' contendo esferas centradas nestes pontos.\n");
        printf("================================================================================\n");
        printf("Usage:> %s <txt_file_with_points> [txt_file_with_points]\n",argv[0]);
        printf("<txt_file_with_points> = Arquivo '.txt' com as coordenadas dos pontos\n");
        printf("[txt_file_with_points] = Arquivo opcional com outro conjunto de pontos\n");
        printf("-------------------------------------------------------------------------------\n");
        printf("OBS: Passando dois arquivos '.txt' na entrada sera gerado um arquivo '.vtp'\n");
        printf("   com cada conjunto de pontos sendo colorido de um escalar diferente\n");
        printf("Example: %s Points/terminalsORI.txt Points/terminalsRAND.txt\n",argv[0]);
        printf("===============================================================================\n");
        exit(EXIT_FAILURE);
    }
    else
    {   
        vector< vector<Point> > points; 
        readPoints(argc,argv,points);
        createSpheres(points);
        return 0;
    }
}


void readPoints (int argc, char *argv[], vector< vector<Point> > &points)
{
    /* Numero de arquivos */
    int nf = argc-1;
    double x, y, z;
    points.assign(nf,vector<Point>());
    
    for (int i = 0; i < nf; i++)
    {
        ifstream in(argv[i+1]);
        while (in >> x >> y >> z)
        {
            Point point(x,y,z);
            points[i].push_back(point);
        }
        in.close();
    }
}

void createSpheres (vector< vector<Point> > points)
{
    int nf = (int)points.size();

    vtkSmartPointer<vtkAppendPolyData> appendFilter =
        vtkSmartPointer<vtkAppendPolyData>::New();
    // Para cada arquivo de pontos
    for (int i = 0; i < nf; i++)
    {
        // Para cada ponto do arquivo gerar uma esfera
        int np = (int)points[i].size();
        for (int j = 0; j < np; j++)
        {
            vtkSmartPointer<vtkSphereSource> sphereSource = 
                vtkSmartPointer<vtkSphereSource>::New();
            sphereSource->SetCenter(points[i][j].x,points[i][j].y,points[i][j].z);
            sphereSource->SetRadius(1.0);
            sphereSource->Update();

            // Colorir cada ponto da esfera com o valor do escala referente ao arquivo atual
            int nps = sphereSource->GetOutput()->GetPoints()->GetNumberOfPoints();
            vtkSmartPointer<vtkFloatArray> scalars =
                vtkSmartPointer<vtkFloatArray>::New();
            scalars->SetNumberOfValues(nps);
            for( int k = 0; k < nps; k++ )
                scalars->SetValue(k,i);
            sphereSource->GetOutput()->GetPointData()->SetScalars(scalars);
            sphereSource->Update();

            appendFilter->AddInputConnection(sphereSource->GetOutputPort());
        }
        appendFilter->Update();

        // Escrever arquivo
        vtkSmartPointer<vtkXMLPolyDataWriter> writer =  
            vtkSmartPointer<vtkXMLPolyDataWriter>::New();
        writer->SetFileName("../spheres.vtp");
        writer->SetInputConnection(appendFilter->GetOutputPort());
        writer->Write();
    }
}
