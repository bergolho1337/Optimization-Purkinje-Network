% ==================================================================================================================================================================================================
% Authors: Bernardo Rocha and Lucas Berg
% This MATLAB script will read the Purkinje networks from the original data, parse the corresponding matrix from double to int and pass this data as an input for the Skeleton3D function.
% Finally, we write the coordinates from the points which are given by the Skeleton3D in a TXT file.
% ==================================================================================================================================================================================================

clear
%% PMJ reading section
fid = fopen('P3DP1couplist.bin','r');
P3DP1couplist = fread(fid,'int32');
fclose(fid);

fid = fopen('P3DP2couplist.bin','r');
P3DP2couplist = fread(fid,'int32');
fclose(fid);

P3DP1couplist = reshape(P3DP1couplist,[],3);
P3DP2couplist = reshape(P3DP2couplist,[],3);

%% Purkinje networks reading section
fid = fopen('P3DP1phase.bin','r');
P3DP1ph = fread(fid,'real*8');
fclose(fid);

fid = fopen('P3DP2phase.bin','r');
P3DP2ph = fread(fid,'real*8');
fclose(fid);

P3DP1ph = reshape(P3DP1ph,[400 320 320]);
P3DP2ph = reshape(P3DP2ph,[400 320 320]);

% Parsing from "real*8" to "int"
P3DP1ph_logic = eliminateThresh(P3DP1ph,1.0e-02);
P3DP2ph_logic = eliminateThresh(P3DP2ph,1.0e-02);

%% Purkinje network surface building
thresh = 0.9;
[P3DP1ph_f,P3DP1ph_v] = isosurface(P3DP1ph,thresh); % precompute
[P3DP2ph_f,P3DP2ph_v] = isosurface(P3DP2ph,thresh);

%% Purkinje networks faces and vertices
%clf
%hold on
%myp1 = patch('vertices',P3DP1ph_v,'faces',P3DP1ph_f,'edgecolor','none','facecolor','y');
%myp2 = patch('vertices',P3DP2ph_v,'faces',P3DP2ph_f,'edgecolor','none','facecolor','y');

%% Writing Purkinje networks in STL format
stlwrite('teste_pk_1.stl', P3DP1ph_f, P3DP1ph_v);
stlwrite('teste_pk_2.stl', P3DP2ph_f, P3DP2ph_v);

%% Skeleton3D section
skel_1 = Skeleton3D(P3DP1ph_logic);
w = size(skel_1,1);
l = size(skel_1,2);
h = size(skel_1,3);
[x,y,z] = ind2sub([w,l,h],find(skel_1(:)));
x=x';
y=y';
z=z';
m = [x; y; z]';
save dados_1.txt m -ascii;

skel_2 = Skeleton3D(P3DP2ph_logic);
w = size(skel_2,1);
l = size(skel_2,2);
h = size(skel_2,3);
[x,y,z] = ind2sub([w,l,h],find(skel_1(:)));
x=x';
y=y';
z=z';
m = [x; y; z]';
save dados_2.txt m -ascii;

%% Visualization
% patch objects can be slow to visualize even with pre-computation
% Enable these commands for faster visualization
%reducepatch(myp1,.1); 
%reducepatch(myp2,.1); % .1 -> reduced to 10% of original faces

% these discrepancies in data dimension order are due to MATLAB's meshgrid versus ndgrid.
% isosurface requires meshgrid but the natural dimension ordering is ndgrid
%plot3(P3DP1couplist(1:30,2),P3DP1couplist(1:30,1),P3DP1couplist(1:30,3),'b.','markersize',35);
%plot3(P3DP2couplist(:,2),P3DP2couplist(:,1),P3DP2couplist(:,3),'r.','markersize',35);

%camlight % to add shading to the isosurface patch plots
%axis vis3d % better for click/drag rotating of the plot
%daspect([1 1 1]); % uniform scaling in all axis directions

%set(gca,'zdir','reverse'); % this is one way by which you can flip the axes
