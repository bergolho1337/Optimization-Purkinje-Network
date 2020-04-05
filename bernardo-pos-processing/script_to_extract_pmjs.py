# ======================================================================================================================================================
# Author: Bernardo Rocha
# This script will read the positions where the Purkinje-Muscle-Junctions are located on both the left and right ventricle and write the corresponding data in VTK_Legacy file format.
# OBS:  First you should run the 'extract_purkinje_and_pmjs.m' script to generate the necessary MAT files.
# 		In order to run the script you need to ask the authors for permission to use the meshes first.
# ======================================================================================================================================================

import scipy.io as sio

def read_data_from_mat_file (mat_filename):
	mat_content = sio.loadmat(mat_filename)
	
	key_name = sorted(mat_content.keys())[0]

	array_data = mat_content[key_name]
			
	return array_data

def write_points_to_vtk (points,filename):
	file = open(filename,"w")
	
	file.write("# vtk DataFile Version 3.0\n")
	file.write("Terminals\n")
	file.write("ASCII\n")
	file.write("DATASET POLYDATA\n")
	file.write("POINTS %u float\n" % (len(points)))
	for i in range(len(points)):
		file.write("%g %g %g\n" % (points[i][1],points[i][0],points[i][2]))
	file.write("VERTICES %lu %lu\n" % (len(points),len(points)*2))
	for i in range(len(points)):
		file.write("1 %u\n" % (i))

	file.close()

def main():
	p3dp1_points = read_data_from_mat_file("couplist_p3dp1.mat")
	p3dp2_points = read_data_from_mat_file("couplist_p3dp2.mat")

	write_points_to_vtk(p3dp1_points,"output/couplist_p3dp1.vtk")
	write_points_to_vtk(p3dp2_points,"output/couplist_p3dp2.vtk")
	

if __name__ == "__main__":
	main()
