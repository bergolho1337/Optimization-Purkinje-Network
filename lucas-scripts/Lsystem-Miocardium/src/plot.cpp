#include "../include/plot.h"

// Imprime o grafo em arquivo
void printGraph (Graph *g)
{
	FILE *file;
	Node *ptr;
	Edge *ptrl;
	char filename[50];
	sprintf(filename,"Run/graph.txt");
	file = fopen(filename,"w+");
	ptr = g->listNodes;
	fprintf(file,"======================= PRINTING GRAPH ================================\n");
	while (ptr != NULL)
	{
	fprintf(file,"|| %d %2.lf %2.lf %2.lf %d %d (%lf %lf %lf) ||",ptr->id,ptr->x,ptr->y,ptr->z,ptr->num_edges,ptr->grow, \
			ptr->d_ori[0],ptr->d_ori[1],ptr->d_ori[2]);
		ptrl = ptr->edges;
		while (ptrl != NULL)
		{
			fprintf(file," --> || %d %2.lf %2.lf %2.lf %2.lf ||",ptrl->id,ptrl->size,ptrl->dest->x,ptrl->dest->y, \
					ptrl->dest->z);
			ptrl = ptrl->next;
		}
		fprintf(file,"\n");
		ptr = ptr->next;
	}
	fprintf(file,"=======================================================================\n");
}

// Escreve o grafo como um arquivo .vtk
void writeGraphVTK (Graph *g)
{
	char filename[50];
	FILE *fileVTK;
	Node *ptr;
	Edge *ptrl;

	ptr = g->listNodes;
	sprintf(filename,"Run/Tree_Nterm.vtk");
	fileVTK = fopen(filename,"w+");
	fprintf(fileVTK,"# vtk DataFile Version 3.0\n");
	fprintf(fileVTK,"Monodomain\n");
	fprintf(fileVTK,"ASCII\n");
	fprintf(fileVTK,"DATASET POLYDATA\n");
	// Nos
	fprintf(fileVTK,"POINTS %d float\n",g->total_nodes);
	while (ptr != NULL)
	{
		fprintf(fileVTK,"%e %e %e\n",ptr->x,ptr->y,ptr->z);
		ptr = ptr->next;
	}
	// Arestas
	fprintf(fileVTK,"LINES %d %d\n",g->total_edges,g->total_edges*3);
	ptr = g->listNodes;
	while (ptr != NULL)
	{
		ptrl = ptr->edges;
		while (ptrl != NULL)
		{
			fprintf(fileVTK,"2 %d %d\n",ptr->id,ptrl->id);
			ptrl = ptrl->next;
		}
		ptr = ptr->next;
	}
	// Imprimir os escalares dos Nodes
	fprintf(fileVTK,"POINT_DATA %d\n",g->total_nodes);
	fprintf(fileVTK,"SCALARS vm float 1\n");
	fprintf(fileVTK,"LOOKUP_TABLE default\n");
	ptr = g->listNodes;
	while (ptr != NULL)
	{
		if (!ptr->isTerminal)
			fprintf(fileVTK,"0\n");
		else
			fprintf(fileVTK,"1\n");
		ptr = ptr->next;
	}
}

// Escreve os terminais em um .vtk
void writeTerminalsVTK (Graph *g)
{
	char filename[50];
	FILE *fileVTK;
	Node *ptr;
	
	ptr = g->listNodes;
	sprintf(filename,"Run/dadosTerminais.txt");
	fileVTK = fopen(filename,"w+");
	while (ptr != NULL)
	{
		if (ptr->isTerminal)
			fprintf(fileVTK,"%d %e %e %e\n",ptr->id,ptr->x,ptr->y,ptr->z);
		ptr = ptr->next;
	}
	fclose(fileVTK);
}

/*
void writeVTK (Graph *g, int k, int exec_number)
{
  char filename[50];
	FILE *fileVTK;
	Node *ptr;
	Edge *ptrl;

	ptr = g->getListNodes();
	sprintf(filename,"../Runs/Run%d/VTK/monodomain%d.vtk",exec_number,k);
	fileVTK = fopen(filename,"w+");
	fprintf(fileVTK,"# vtk DataFile Version 3.0\n");
	fprintf(fileVTK,"Monodomain\n");
	fprintf(fileVTK,"ASCII\n");
	fprintf(fileVTK,"DATASET POLYDATA\n");
	// Nos
	fprintf(fileVTK,"POINTS %d float\n",g->getTotalNodes());
	while (ptr != NULL)
	{
		fprintf(fileVTK,"%e %e %e\n",ptr->getX(),ptr->getY(),ptr->getZ());
		ptr = ptr->getNext();
	}
	// Arestas
	fprintf(fileVTK,"LINES %d %d\n",g->getTotalEdges(),g->getTotalEdges()*3);
	ptr = g->getListNodes();
	while (ptr != NULL)
	{
		ptrl = ptr->getEdges();
		while (ptrl != NULL)
		{
			fprintf(fileVTK,"2 %d %d\n",ptr->getId()-1,ptrl->getId()-1);
			ptrl = ptrl->getNext();
		}
		ptr = ptr->getNext();
	}
	// Solucao da equacao do monodominio
	fprintf(fileVTK,"POINT_DATA %d\n",g->getTotalNodes());
	fprintf(fileVTK,"SCALARS vm float 1\n");
	fprintf(fileVTK,"LOOKUP_TABLE default\n");
	ptr = g->getListNodes();
	while (ptr != NULL)
	{
		fprintf(fileVTK,"%e\n",ptr->getCell()->y[0]);
		ptr = ptr->getNext();
	}
	fclose(fileVTK);
}

Node** setPlotPointers (Graph *g)
{
  // Set the plot to be at the end of the central fiber
	Node **plot = new Node*[2];
	cout << "[+] Setting plot pointers to volumes:" << endl;
	plot[0] = g->searchNode(2);
	plot[1] = g->searchNode(10);
	cout << "\tVolume 1: " << plot[0]->getId() << endl;
	cout << "\tVolume 2: " << plot[1]->getId() << endl;
	return (plot);
}

void writeGraphic (Graph *g, double t, Node **plot, int num_eq, int exec_number)
{
  int k;
	char filename[50];
	FILE *file;
	for (k = 0; k < 2; k++)
	{
		sprintf(filename,"../Runs/Run%d/data%d.dat",exec_number,plot[k]->getId());
		file = fopen(filename,"a");
		fprintf(file,"%e ",t);
		for (int i = 0; i < num_eq; i++)
			fprintf(file,"%e ", plot[k]->getCell()->y[i]);
		fprintf(file,"\n");
		fclose(file);
	}
}

void makePlot (Node **plot, int exec_number)
{
  FILE *file;
  char filename[50];
  int k;
  for (k = 0; k < 2; k++)
  {
    sprintf(filename,"../Runs/Run%d/data%d.dat",exec_number,plot[k]->getId());
		// Imprime o gráfico do potencial transmembrânico (V)
		file = fopen("graph.plt","w+");
		fprintf(file,"set grid\n");
		fprintf(file,"set terminal png\n");
		fprintf(file,"set output \"../Runs/Run%d/Graphics/dvdt%d.png\"\n",exec_number,k);
		fprintf(file,"set title \"Vol = %d\"\n",plot[k]->getId());
		fprintf(file,"plot \"%s\" using 1:2 title \"Vm\" w l\n",filename);
		fclose(file);
		if (!system("gnuplot graph.plt"))
			cout << "[+] Grafico plotado com sucesso!" << endl;
		else
			cout << "[-] ERRO! Plotando grafico!" << endl;
		// Imprime o gráfico da variável gate (m)
		file = fopen("graph.plt","w+");
		fprintf(file,"set grid\n");
		fprintf(file,"set terminal png\n");
		fprintf(file,"set output \"../Runs/Run%d/Graphics/dmdt%d.png\"\n",exec_number,k);
		fprintf(file,"set title \"Vol = %d\"\n",plot[k]->getId());
		fprintf(file,"plot \"%s\" using 1:3 title \"m\" w l\n",filename);
		fclose(file);
		if (!system("gnuplot graph.plt"))
			cout << "[+] Grafico plotado com sucesso!" << endl;
		else
			cout << "[-] ERRO! Plotando grafico!" << endl;
  }
}

void printInfoModel (Graph *g, double *y0, double t_max, double Dt, double Dx, int exec_number)
{
  char filename[50];
  sprintf(filename,"../Runs/Run%d/info_simulation",exec_number);
  FILE *out = fopen(filename,"w+");
  fprintf(out,"=================== PARAMETERS OF THE MODEL ===================\n");
	fprintf(out,"Number of segments = %d\n",g->getTotalEdges());
	fprintf(out,"Number of nodes = %d\n",g->getTotalNodes());
	fprintf(out,"V_stim = %.2lf mV\n",y0[0]);
	fprintf(out,"t_max = %.2lf s\n",t_max);
	fprintf(out,"N = %d\n",(int)(g->getXmax()/Dx));
	fprintf(out,"M = %d\n",(int)(t_max/Dt));
	fprintf(out,"Dx = %e um\n",Dx);
	fprintf(out,"Dt = %e s\n",Dt);
  fclose(out);
}


*/