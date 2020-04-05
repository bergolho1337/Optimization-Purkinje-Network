% =====================================================================================================================================
% Author: Benjamim Liu
% This MATLAB script is used to extract both the Purkinje network and Purkinje-Muscle-Junction positions from the left and right
% ventricles.
% -------------------------------------------------------------------------------------------------------------------------------------
% OBS: In order to run the script you need to ask the authors for permission to use the data first.
% =====================================================================================================================================

clear
%%
fid = fopen('P3DP1couplist.bin','r');
P3DP1couplist = fread(fid,'int32');
fclose(fid);

fid = fopen('P3DP2couplist.bin','r');
P3DP2couplist = fread(fid,'int32');
fclose(fid);

P3DP1couplist = reshape(P3DP1couplist,[],3);
P3DP2couplist = reshape(P3DP2couplist,[],3);
%%
fid = fopen('P3DP1phase.bin','r');
P3DP1ph = fread(fid,'real*8');
fclose(fid);

fid = fopen('P3DP2phase.bin','r');
P3DP2ph = fread(fid,'real*8');
fclose(fid);

P3DP1ph = reshape(P3DP1ph,[400 320 320]);
P3DP2ph = reshape(P3DP2ph,[400 320 320]);

%%

thresh = 0.9;
[P3DP1ph_f,P3DP1ph_v] = isosurface(P3DP1ph,thresh); % precompute
[P3DP2ph_f,P3DP2ph_v] = isosurface(P3DP2ph,thresh);
%%
clf
hold on
myp1 = patch('vertices',P3DP1ph_v,'faces',P3DP1ph_f,'edgecolor','none','facecolor','y');
myp2 = patch('vertices',P3DP2ph_v,'faces',P3DP2ph_f,'edgecolor','none','facecolor','y');

% patch objects can be slow to visualize even with pre-computation
% Enable these commands for faster visualization
reducepatch(myp1,.1); reducepatch(myp2,.1); % .1 -> reduced to 10% of original faces

% these discrepancies in data dimension order are due to MATLAB's meshgrid versus ndgrid.
% isosurface requires meshgrid but the natural dimension ordering is ndgrid
%plot3(P3DP1couplist(:,2),P3DP1couplist(:,1),P3DP1couplist(:,3),'b.','markersize',35);
%plot3(P3DP2couplist(:,2),P3DP2couplist(:,1),P3DP2couplist(:,3),'r.','markersize',35);

%camlight % to add shading to the isosurface patch plots
%axis vis3d % better for click/drag rotating of the plot
%daspect([1 1 1]); % uniform scaling in all axis directions

%set(gca,'zdir','reverse'); % this is one way by which you can flip the axes

% Left ventricle Purkinje network and PMJs
save P3DP1ph_vertices.mat P3DP1ph_v;
save P3DP1ph_faces.mat P3DP1ph_f;
save couplist_p3dp1.mat P3DP1couplist;

% Right ventricle Purkinje network and PMJs
save P3DP2ph_vertices.mat P3DP2ph_v;
save P3DP2ph_faces.mat P3DP2ph_f;
save couplist_p3dp2.mat P3DP2couplist;
