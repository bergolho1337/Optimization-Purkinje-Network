# ======================================================================================================================================================
# Author: Bernardo Rocha
# This script will read the vertices and faces, which represent the Purkinje networks mesh, and write the corresponding data in VTK_Legacy file format.
# OBS:  First you should run the 'extract_purkinje_and_pmjs.m' script to generate the necessary MAT files.
# 		In order to run the script you need to ask the authors for permission to use the meshes first.
# ======================================================================================================================================================

import scipy.io as sio

def read_data_from_mat_file (mat_filename):
	mat_content = sio.loadmat(mat_filename)
	
	key_name = sorted(mat_content.keys())[0]

	array_data = mat_content[key_name]
			
	return array_data

def write_surface_to_vtk (points,faces,filename):
	file = open(filename,"w")
	
	file.write("# vtk DataFile Version 3.0\n")
	file.write("Terminals\n")
	file.write("ASCII\n")
	file.write("DATASET POLYDATA\n")
	
	file.write("POINTS %u float\n" % (len(points)))
	for i in range(len(points)):
		file.write("%g %g %g\n" % (points[i][0],points[i][1],points[i][2]))

	file.write("POLYGONS %lu %lu\n" % (len(faces),len(faces)*4))
	for i in range(len(faces)):
		file.write("3 %u %u %u\n" % (faces[i][0]-1,faces[i][1]-1,faces[i][2]-1))

	file.close()

def main():
	p3dp1_purkinje_vertices = read_data_from_mat_file("P3DP1ph_vertices.mat")
	p3dp1_purkinje_faces = read_data_from_mat_file("P3DP1ph_faces.mat")
	write_surface_to_vtk(p3dp1_purkinje_vertices,p3dp1_purkinje_faces,"output/purkinje_1.vtk")

	p3dp2_purkinje_vertices = read_data_from_mat_file("P3DP2ph_vertices.mat")
	p3dp2_purkinje_faces = read_data_from_mat_file("P3DP2ph_faces.mat")
	write_surface_to_vtk(p3dp2_purkinje_vertices,p3dp2_purkinje_faces,"output/purkinje_2.vtk")

if __name__ == "__main__":
	main()
