#include <cstdio>
#include <cstdlib>
#include <vector>

using namespace std;

struct Point;
struct Line;
struct FileReader;

struct Point
{
    double x, y, z;

}typedef Point;

struct Line
{
    int esq;
    int dir;
}typedef Line;

struct FileReader
{
    vector<Point> points;
    vector<Line> lines;
}typedef FileReader;

FileReader* newFileReader (int argc, char *argv[]);
void readFile (FileReader *reader, char filename[]);
void printInfo (FileReader *reader);
void writeVTK (FileReader *reader);
void checkConnectivity (FileReader *reader);
void eliminateVertex (FileReader *reader, int id);
void errorMsg ();
void freeMemory (FileReader *reader);
