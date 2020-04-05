% =====================================================================================================================================
% Author: Benjamim Liu
% This MATLAB script is used to extract both the isosurface and activation time from the left and right ventricles.
% -------------------------------------------------------------------------------------------------------------------------------------
% OBS: In order to run the script you need to ask the authors for permission to use the meshes first.
% =====================================================================================================================================

clear
load('ventphase');
load('P3Darrivals_iso');
%% Clean data for plotting
arrivals = P3Darrivals;
% for visualization; originally negative entries correspond to outside of
% domain, so we reassign as maximum activation time for plotting.
arrivals(arrivals<0) = max(arrivals(:));

% shift so everything begins with respect to first activation
arrivals = arrivals-min(arrivals(:));

%% Minimalist plot script
clf
axes('zdir','reverse', 'xdir','reverse')
[ventf, ventv] = isosurface(vph,.5);
ventp1 = patch('faces',ventf, 'vertices',ventv, 'edgecolor','none', 'facecolor','interp');
isocolors(arrivals,ventp1);
camlight
daspect([1 1 1]);

%% Cutaway plotting script
ax1pos = [-.02 -.075 .47 .97];
ax2pos = [.47 -.075 .47 .97];

clf
set(gcf, 'position',[10 50 990 425], 'paperpositionmode','auto');

mycmap = hsv(20); mycmap = mycmap(1:end-7,:);
colormap(mycmap);

opts = {'visible','off', 'xtick',[], 'ytick',[], 'ztick',[], 'zdir','reverse', 'xdir','reverse'};

% set to 1 to display 'front' of the two halves, set to 0 to display back.
front = 1;

cbarax = axes('position',[.05 .05 .935 .8], 'visible','off');
cbar=colorbar;
set(cbar, 'fontsize',16);
set(get(cbar,'title'), 'string','ms', 'fontsize',24)
text('string','3D 3D canine isochronal maps', 'position',[.5 1.1], 'fontsize',30, 'horizontalalignment','center');

ventax1 = axes('outerposition',ax1pos, opts{:});
daspect([1 1 1]);
axis vis3d
if front
    view([-100 20]);
else
    view([100 -10]);
end

ventax2 = axes('outerposition',ax2pos, opts{:});
daspect([1 1 1]);
axis vis3d
if front
    view([100 20]);
else
    view([-100 -10]);
end

% cut the ventricles in two for cutaway view
split = 140;
% define restrictions on the indices giving the two cuts
res1 = {1:size(vph,1), 1:split, 1:size(vph,3)};
res2 = {1:size(vph,1), split:size(vph,2), 1:size(vph,3)};

% generate isosurface for the 3d surface
[ventf, ventv] = isosurface(vph(res1{:}),.5);
ventp1 = patch('faces',ventf, 'vertices',ventv, 'edgecolor','none', 'facecolor','interp', 'parent',ventax1);
isocolors(arrivals(res1{:}),ventp1);

% generate isosurface plot for the 2d surface of the cut
[ventf, ventv] = isocaps(vph(res1{:}),.5);
ventp1caps = patch('faces',ventf, 'vertices',ventv, 'edgecolor','none', 'facecolor','interp', 'parent',ventax1);
isocolors(arrivals(res1{:}),ventp1caps);

% do the same for the second half of the ventricles

[ventf, ventv] = isosurface(vph(res2{:}),.5);
ventp2 = patch('faces',ventf, 'vertices',ventv, 'edgecolor','none', 'facecolor','interp', 'parent',ventax2);
isocolors(arrivals(res2{:}),ventp2);

[ventf, ventv] = isocaps(vph(res2{:}),.5);
ventp2caps = patch('faces',ventf, 'vertices',ventv, 'edgecolor','none', 'facecolor','interp', 'parent',ventax2);
isocolors(arrivals(res2{:}),ventp2caps);


% lighting for 3d patch surfaces
light1 = light('parent',ventax1);
camlight(light1);

light2 = light('parent',ventax2);
camlight(light2);

axis(ventax1,'tight')
axis(ventax2,'tight')

% grab automatic colorbar from one of the two plots
cax = caxis(ventax2);

% assign colorbar to all the plots
caxis(ventax1,cax)
caxis(ventax2,cax)
caxis(cbarax,cax)