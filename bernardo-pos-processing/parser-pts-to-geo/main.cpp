// Author: Lucas Berg
// This program reads a PTS file and write a GEO file so it can be open in GMSH 

#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include "timer.h"

#define PRINT_LINE "============================================================================================================="

class Point
{
public:
	uint32_t id;
	double x, y, z;
public:
	Point (const uint32_t id, const double x, const double y, const double z)
	{
		this->id = id;
		this->x = x;
		this->y = y;
		this->z = z;
	}
};

void usage (const char pname[])
{
	fprintf(stderr,"%s\n",PRINT_LINE);
	fprintf(stderr,"Usage:> %s <input_filename>\n",pname);
	fprintf(stderr,"%s\n",PRINT_LINE);
	fprintf(stderr,"<input_filename> = Input filename with the mesh in PTS format.\n");
	fprintf(stderr,"%s\n",PRINT_LINE);
	fprintf(stderr,"Example:> %s inputs/mesh_points_LV.pts\n",pname);
	fprintf(stderr,"%s\n",PRINT_LINE);
}

void print_progress (int iter, int max_iter)
{
    double progress = iter / (double)max_iter;
    
    std::cout << "Progress: " << int(progress * 100.0) << " %\r";
    std::cout.flush();
}

void read_points_from_pts (const char filename[], std::vector<Point> &points)
{
	FILE *file = fopen(filename,"r");
	if (!file)
	{
		fprintf(stderr,"[-] ERROR! Could not open file '%s'\n",filename);
		exit(EXIT_FAILURE);
	}

	uint32_t num_points;
	fscanf(file,"%u",&num_points);
	for (uint32_t i = 0; i < num_points; i++)
	{
		double pos[3];
		fscanf(file,"%lf %lf %lf",&pos[0],&pos[1],&pos[2]);

		Point p(i,pos[0],pos[1],pos[2]);

		points.push_back(p);
	}

	fclose(file);
}

void write_points_in_geo (std::vector<Point> points)
{
	FILE *file = fopen("outputs/mesh_points.geo","w+");

	fprintf(file,"cl__1 = 1;\n");
	for (uint32_t i = 0; i < points.size(); i++)
	{
		fprintf(file,"Point(%u) = {%g, %g, %g, 1};\n",i+1,points[i].x,points[i].y,points[i].z);
	}

	fclose(file);
}

int main(int argc, char *argv[])
{
	if (argc-1 != 1)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	char *input_filename = argv[1];

	std::vector<Point> points;
	read_points_from_pts(input_filename,points);

	write_points_in_geo(points);	

	return 0;

}
