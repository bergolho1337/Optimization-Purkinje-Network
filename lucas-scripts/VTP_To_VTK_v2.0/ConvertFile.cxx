#include <iostream>
#include <cstdio>
#include <cstring>
#include <vector>
 
#include "vtkCellArray.h"
#include "vtkPoints.h"
#include "vtkPointData.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkPolyData.h"
 
struct Point
{
	double x,y,z;
	Point(const double xin, const double yin, const double zin) : x(xin), y(yin), z(zin) {}
};
 
int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        printf("============================================================\n");
        printf("Usage:> %s <vtp_file_to_convert>\n",argv[0]);
        printf("Example: ./ConvertFile /home/lucas/Documentos/Mestrado/Dissertacao/Purkinje_Miocardium/geodesicsonmeshes_purkinje/res/vent_ou.vtp\n");
        printf("============================================================\n");
        exit(1);
    }
    else
    {   
        //get all data from the file
        vtkXMLPolyDataReader* reader = vtkXMLPolyDataReader::New();
        reader->SetFileName(argv[1]);
        reader->Update();
        vtkPolyData* polydata = reader->GetOutput();
    
        //get the number of points the file contains
        vtkIdType NumPoints = polydata->GetNumberOfPoints();
    
        //if there are no points, quit
        if(!(NumPoints > 0) )
        {
            exit(-1);
        }
    
        // Read in all of the points
        std::vector<Point> Points;
        double point[3];
        for(vtkIdType i = 0; i < NumPoints; i++)
        {
            polydata->GetPoint(i, point);
            Points.push_back(Point(point[0], point[1], point[2]));
        }

        // Get the scalars of all the points
        std::vector<double> Scalars;
        for(int i = 0; i < NumPoints; i++)
        {
            double w = polydata->GetPointData()->GetScalars()->GetComponent(0,i);
            Scalars.push_back(w);
        }
    
        // Get the triangles (if there are any)
        std::vector<std::vector<int> > VertexLists;
        vtkIdType NumPolys = polydata->GetNumberOfPolys();
        if(NumPolys > 0)
        {
            vtkCellArray* TriangleCells = polydata->GetPolys();
            vtkIdType npts;
            vtkIdType *pts;
    
            while(TriangleCells->GetNextCell(npts, pts))
            {
                std::vector<int> List(3);
                List[0] = pts[0];
                List[1] = pts[1];
                List[2] = pts[2];
    
                VertexLists.push_back(List);
            }	
        }

        int n = strlen(argv[1]);
        argv[1][n-1] = 'k';
        // Write the VTK file
        FILE *fileVTK = fopen(argv[1],"w+");
        fprintf(fileVTK,"# vtk DataFile Version 3.0\n");
        fprintf(fileVTK,"vtk output\n");
        fprintf(fileVTK,"ASCII\n");
        fprintf(fileVTK,"DATASET POLYDATA\n");
        fprintf(fileVTK,"POINTS %lld float\n",NumPoints);
        for (unsigned int i = 0; i < Points.size(); i++)
            fprintf(fileVTK,"%e %e %e\n",Points[i].x,Points[i].y,Points[i].z);
        fprintf(fileVTK,"POLYGONS %lld %lld\n",NumPolys,NumPolys*4);
        for (unsigned int i = 0; i < VertexLists.size(); i++)
            fprintf(fileVTK,"3 %d %d %d\n",VertexLists[i][0],VertexLists[i][1],VertexLists[i][2]);
        fprintf(fileVTK,"POINT_DATA %lld\n",NumPoints);
        fprintf(fileVTK,"SCALARS FMMDist float\n");
        fprintf(fileVTK,"LOOKUP_TABLE default\n");
        for(int i = 0; i < NumPoints; i++)
            fprintf(fileVTK,"%e\n",Scalars[i]);
        fclose(fileVTK);
    }

	
    
	return 0;
}