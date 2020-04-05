#include "../include/reader.h"

FileReader* newFileReader (int argc, char *argv[])
{
    FileReader* reader = (FileReader*)malloc(sizeof(FileReader));
    readFile(reader,argv[1]);
    printInfo(reader);

    checkConnectivity(reader);

    eliminateVertex(reader,5);
    eliminateVertex(reader,5);
    
    writeVTK(reader);
    //printInfo(reader);
    return reader;   
}

void readFile (FileReader *reader, char filename[])
{
    int N, M;
    FILE *file = fopen(filename,"r");
    // Read the points
    if (!fscanf(file,"%d",&N)) errorMsg();
    for (int i = 0; i < N; i++)
    {
        Point point;
        if (!fscanf(file,"%lf %lf %lf",&point.x,&point.y,&point.z)) errorMsg();
        reader->points.push_back(point);
    }
    // Read the lines
    if (!fscanf(file,"%d",&M)) errorMsg();
    for (int i = 0; i < M; i++)
    {
        Line line;
        if (!fscanf(file,"%d %d",&line.esq,&line.dir)) errorMsg();
        line.esq--; line.dir--;
        reader->lines.push_back(line);
    }   
    fclose(file);
}

void eliminateVertex (FileReader *reader, int id)
{
    reader->points.erase(reader->points.begin()+id);
    for (unsigned int i = 0; i < reader->lines.size(); i++)
    {
        if (reader->lines[i].esq > id)
            reader->lines[i].esq--;
        if (reader->lines[i].dir > id)
            reader->lines[i].dir--;
    }
}

void checkConnectivity (FileReader *reader)
{
    FILE *file = fopen("problem.txt","w+");
    bool inTree;
    for (int i = 0; i < (int)reader->points.size(); i++)
    {
        inTree = false;
        for (int j = 0; j < (int)reader->lines.size(); j++)
        {
            if (reader->lines[j].esq == i || reader->lines[j].dir == i)
            {
                inTree = true;
                break;
            }
        }
        if (!inTree)
            fprintf(file,"%d\n",i);
    }
    fclose(file);
}

void printInfo (FileReader *reader)
{
    printf("========== POINTS ============\n");
    printf("Num points = %d\n",(int)reader->points.size());
    for (int i = 0; i < (int)reader->points.size(); i++)
        printf("(%lf,%lf,%lf)\n",reader->points[i].x,reader->points[i].y,reader->points[i].z);
    printf("==============================\n");
    printf("========== LINES =============\n");
    printf("Num lines = %d\n",(int)reader->lines.size());
    for (int i = 0; i < (int)reader->lines.size(); i++)
        printf("(%d,%d)\n",reader->lines[i].esq,reader->lines[i].dir);
    printf("==============================\n");

}

void writeVTK (FileReader *reader)
{
    FILE *fileVTK = fopen("saida.vtk","w+");
    fprintf(fileVTK,"# vtk DataFile Version 3.0\n");
    fprintf(fileVTK,"vtk output\n");
    fprintf(fileVTK,"ASCII\n");
    fprintf(fileVTK,"DATASET POLYDATA\n");
    fprintf(fileVTK,"POINTS %d float\n",(int)reader->points.size());
    for (unsigned int i = 0; i < reader->points.size(); i++)
        fprintf(fileVTK,"%e %e %e\n",reader->points[i].x,reader->points[i].y,reader->points[i].z);
    fprintf(fileVTK,"LINES %d %d\n",(int)reader->lines.size(),(int)reader->lines.size()*3);
    for (unsigned int i = 0; i < reader->lines.size(); i++)
        fprintf(fileVTK,"2 %d %d\n",reader->lines[i].esq,reader->lines[i].dir);
    fclose(fileVTK);
}

void errorMsg ()
{
    printf("[-] ERROR!\n");
    exit(-1);
}

void freeMemory (FileReader *reader)
{
    reader->points.clear();
    reader->lines.clear();
    free(reader);
}