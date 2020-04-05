#include <cstdio>
#include <cstdlib>
#include "../include/reader.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        printf("===========================================================\n");
        printf("Usage:> %s <input_file>\n",argv[0]);
        printf("===========================================================\n");
        exit(-1);
    }
    else
    {
        FileReader *reader = newFileReader(argc,argv);
        freeMemory(reader);
        return 0;
    }
}