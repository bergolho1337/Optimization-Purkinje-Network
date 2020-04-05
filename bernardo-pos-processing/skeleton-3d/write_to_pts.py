# ============================================================================
# Author: Lucas Berg
# A simple parser from TXT to PTS.
# TODO: Add an option to pass the number of points from the mesh. 
# ============================================================================

input_file = open("dados_1.txt","r")
output_file = open("dados_1.pts","w")
number_of_lines = 3911 		# dados_1
#number_of_lines = 2450 		# dados_2

output_file.write("%u\n" % (number_of_lines))
for line in input_file:
	tokens = line.split()
	x,y,z = float(tokens[0]),float(tokens[1]),float(tokens[2])

	output_file.write("%g %g %g\n" % (y,x,z))

input_file.close()
output_file.close()
