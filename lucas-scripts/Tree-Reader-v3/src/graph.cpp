#include "../include/graph.h"

FILE *infoFile;                             // Arquivo de informacao da arvore
FILE *pmjFile;                              // Arquivo com os PMJs

vector<double> size_segments;               // Vetor com o tamanho de todos os segmentos                        
vector<bool> visit;                         // Vetor de visitados da DFS
int num_segments;                           // Numero de segmentos da arvore
double seg_size;                            

// Inicializa os atributos do grafo
void initGraph (Graph **g)
{
    (*g)->total_nodes = 0;
    (*g)->total_edges = 0;
    (*g)->listNodes = NULL;
    (*g)->lastNode = NULL;
}

// Le um arquivo .vtk contendo a rede de Purkinje e transforma para uma malha de Elementos Finitos
Graph* readPurkinjeNetworkFromFile (char *filename)
{
    FILE *inFile = fopen(filename,"r");
    if (inFile == NULL) error("Cannot open VTK file!");
    Graph *g = (Graph*)malloc(sizeof(Graph));
    initGraph(&g);
    // Ler os nodos
    int N;
    char str[100];
    while (fscanf(inFile,"%s",str) != EOF)
        if (strcmp(str,"POINTS") == 0) break;
    if (!fscanf(inFile,"%d",&N)) error("Reading file");
    if (!fscanf(inFile,"%s",str)) error("Reading file");
    for (int i = 0; i < N; i++)
    {
        double p[3];
        if (!fscanf(inFile,"%lf %lf %lf",&p[0],&p[1],&p[2])) error("Reading file");
        insertNodeGraph(&g,p);
    }
    // Ler as arestas
    int A, trash;
    while (fscanf(inFile,"%s",str) != EOF)
        if (strcmp(str,"LINES") == 0) break;
    if (!fscanf(inFile,"%d %d",&A,&trash)) error("Reading file");
    for (int i = 0; i < A; i++)
    {
        int e[2];
        if (!fscanf(inFile,"%d %d %d",&trash,&e[0],&e[1])) error("Reading file");
        insertEdgeGraph(&g,e[0],e[1]);
        //insertEdgeGraph(&g,e[1],e[0]);
    }
    // Ler os PMJs
    /*
    while (fscanf(inFile,"%s",str) != EOF)
        if (strcmp(str,"default") == 0) break;
    Node *ptr = g->listNodes;
    for (int i = 0; i < N; i++)
    {
        if (!fscanf(inFile,"%d",&A)) error("Reading file");
        ptr->isPMJ = A;
        ptr = ptr->next;
    }
    fclose(inFile);
    */
    return g;
}

// Construtor de um nodo
Node* newNode (int id, double x, double y, double z)
{
    Node *node = (Node*)malloc(sizeof(Node));
    node->id = id;
    node->x = x;
    node->y = y;
    node->z = z;
    node->num_edges = 0;
    node->next = NULL;
    node->edges = NULL;
    return node;
}

// Cosntrutor de uma aresta
Edge* newEdge (int id, double w, Node *dest)
{
	Edge *edge = (Edge*)malloc(sizeof(Edge));
	edge->id = id;
	edge->w = w;
	edge->dest = dest;
	edge->next = NULL;
	return edge;
}


// Insere um nodo com coordenadas (px,py,pz) no grafo
Node* insertNodeGraph (Graph **g, double p[])
{
    Node *ptr = (*g)->listNodes;
    Node *ptrNew = newNode((*g)->total_nodes++,p[0],p[1],p[2]);
    // Primeiro nodo do grafo
    if (ptr == NULL)
        (*g)->listNodes = ptrNew;
    // Percorrer a lista de nodos
    else
    {
        while (ptr->next != NULL)
            ptr = ptr->next;
        ptr->next = ptrNew;
    }
    return ptrNew;
}

// Insere uma aresta no grafo
void insertEdgeGraph (Graph **g, int id_1, int id_2)
{
	Node *ptr1, *ptr2;
	Edge *edge;
	double norm;
	// Checar se a aresta eh invalida
	if (id_1 == id_2) return;

	ptr1 = searchNode(*g,id_1);
	ptr2 = searchNode(*g,id_2);
	
    norm = calcNorm(ptr1->x,ptr1->y,ptr1->z,ptr2->x,ptr2->y,ptr2->z);
    edge = newEdge(id_2,norm,ptr2);
    // Primeira aresta
    if (ptr1->edges == NULL)
        ptr1->edges = edge;
    // Percorrer ate o final 
    else
    {
        Edge *ptrl = ptr1->edges;
        while (ptrl->next != NULL)
            ptrl = ptrl->next;
        ptrl->next = edge;
    }
    // Incrementar o contador de arestas do Node origem
    ptr1->num_edges++;
    // Incrementar o numero total de arestas no grafo
    (*g)->total_edges++;
}

// Busca por um Node no grafo
Node* searchNode (Graph *g, int id)
{
	Node *ptr = g->listNodes;
	while (ptr != NULL)
	{
		if (ptr->id == id)
			return ptr;
		ptr = ptr->next;
	}
    printf("[-] ERROR! Node %d was not found!\n",id);
    error("Node not found!");
    return NULL;
}


void error (char *msg)
{
    printf("[-] ERROR! %s\n",msg);
    exit(-1);   
}

// Calcula a norma euclidiana entre dois pontos
double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2)
{
    return sqrt(pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2));
}

void printGraph (Graph *g)
{
	Node *ptr;
	Edge *ptrl;
	ptr = g->listNodes;
	printf("======================= PRINTING GRAPH ================================\n");
	while (ptr != NULL)
	{
	    printf("|| %d (%2.lf %2.lf %2.lf) %d %d ||",ptr->id,ptr->x,ptr->y,ptr->z,ptr->num_edges,ptr->isPMJ);
		ptrl = ptr->edges;
		while (ptrl != NULL)
		{
			printf(" --> || %d %2.lf (%2.lf %2.lf %2.lf) ||",ptrl->id,ptrl->w,ptrl->dest->x,ptrl->dest->y, \
					ptrl->dest->z);
			ptrl = ptrl->next;
		}
		printf("\n");
		ptr = ptr->next;
	}
	printf("=======================================================================\n");
}

// Busca em profundidade
void dfs (Node *u)
{
    int contDFS = 0;
    visit[u->id] = true;
    Edge *v = u->edges;

    while (v != NULL)
    {
        if (visit[v->id] == false)
        {
            contDFS++;
            seg_size += v->w;
            dfs(v->dest);
        }
        v = v->next;
    }
    // Eh um terminal, falta diferenciar se eh PMJ ou nao
    if (u->num_edges == 0)
    {
        //if (u->isPMJ == 0)
        //    fprintf(terminalFile,"%.5lf %.5lf %.5lf\n",u->x,u->y,u->z);
        num_segments++;
        size_segments.push_back(seg_size);
        seg_size = 0;
    }
    if (contDFS == 2)
    {
        num_segments++;
    }
        
}

// Constroi um vetor normalizado a partir de uma aresta
void buildVector (Node *ptr, Edge *ptrl, double v[])
{
    double v_norm;
    v[0] = ptrl->dest->x - ptr->x; v[1] = ptrl->dest->y - ptr->y; v[2] = ptrl->dest->z - ptr->z;
    v_norm = sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
    v[0] /= v_norm; v[1] /= v_norm; v[2] /= v_norm;
}

// Calcula o angulo entre dois vetores
double calcAngle (double u[], double v[])
{
    double angle;
	double p_scalar = 0;
    for (int i = 0; i < 3; i++) p_scalar += u[i]*v[i];
    double u_norm, v_norm;
	u_norm = sqrt(pow(u[0],2) + pow(u[1],2) + pow(u[2],2));
	v_norm = sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
    angle = acos(p_scalar / (u_norm*v_norm)) * 180.0 / M_PI;
    if (angle != angle)
        angle = M_PI;
    return angle;
}

// Calcular media e desvio padrao de um vetor e armazenar o resultado na estrutura StdMean
void calcMeanStd (vector<double> v, StdMean &s)
{
    s.mean = 0;
    for (int i = 0; i < (int)v.size(); i++)
        s.mean += v[i];
    s.mean /= (double)v.size();
    s.std = 0;
    for (int i = 0; i < (int)v.size(); i++)
        s.std += pow(v[i] - s.mean,2);
    s.std = sqrt(s.std / (double)v.size());
}

// Calcular media e desvio padrao das informacoes medidas
void calcStatistics (InfoTree *info)
{
    // Calcular a media e o desvio padrao do angulo das bifurcacoes
    calcMeanStd(info->ang_biff,info->angle);
    // Calcular a media e o desvio padrao das arestas
    calcMeanStd(info->edges,info->edge);
    // Calcular a media e o desvio padrao dos segmentos
    calcMeanStd(info->size_segments,info->segment);
}

// Escreve em arquivo as informacoes a respeito da arvore lida
void calcInfoTree (InfoTree *info)
{
    FILE *infoFile = info->infoFile;
    Graph *g = info->g;
    infoFile = fopen("Output/info.txt","w+");

    fprintf(infoFile,"================= INFORMATION FROM TREE ==============================\n");
    fprintf(infoFile,"Number of nodes = %d\n",g->total_nodes);
    fprintf(infoFile,"Number of edges = %d\n",g->total_edges);
    fprintf(infoFile,"Total size of the tree = %.5lf\n",info->total_size);
    fprintf(infoFile,"Number of segments = %d\n",num_segments);
    fprintf(infoFile,"Number of bifurcations = %d\n",(int)info->ang_biff.size());
    fprintf(infoFile,"Mean size of the edges = %.5lf +/- %.5lf\n",info->edge.mean,info->edge.std);
    fprintf(infoFile,"Mean size of the segments = %.5lf +/- %.5lf\n",info->segment.mean,info->segment.std);
    fprintf(infoFile,"Mean angle of the bifurcations = %.5lf +/- %.5lf\n",info->angle.mean,info->angle.std);
    
    //fprintf(infoFile,"Number of active PMJs = %d\n",activePMJ);
    //fprintf(infoFile,"Number of inactive PMJs = %d\n",inactivesPMJ);
    fprintf(infoFile,"======================================================================\n");

    fclose(infoFile);

}

// Percorre todos as arestas do grafo e calcula o tamanho total da arvore
// Armazena o tamanho de todas as arestas no vetor 'edges'
void calcEdges (InfoTree *info)
{
    Graph *g = info->g;
    FILE *edgeFile = info->edgeFile;
    edgeFile = fopen("Output/edges.txt","a");
    double total_size = 0;
    Node *ptr = g->listNodes;

    while (ptr != NULL)
    {
        Edge *ptrl = ptr->edges;
        while (ptrl != NULL)
        {
            fprintf(edgeFile,"%.60lf\n",ptrl->w);
            info->edges.push_back(ptrl->w);
            total_size += ptrl->w;
            ptrl = ptrl->next;
        }
        ptr = ptr->next;
    }
    fclose(edgeFile);
    info->total_size = total_size;
    printf("Edges = %d\n",(int)info->edges.size());
}

// Calcula o angulo de cada bifurcacao da arvore colocando no vetor 'angle_biff'
// Armazena o valor medio na variavel 'mean_angle_bif'
void calcAngles (InfoTree *info)
{
    Graph *g = info->g;
    FILE *angleFile = info->angleFile;
    angleFile = fopen("Output/angles.txt","a");
    Node *ptr = g->listNodes;

    while (ptr != NULL)
    {
        // Eh uma bifurcacao ? Calcular o angulo
        if (ptr->num_edges == 2)
        {
            double u[3], v[3];
            Edge *edge1 = ptr->edges;
            Edge *edge2 = ptr->edges->next;
            buildVector(ptr,edge1,u);
            buildVector(ptr,edge2,v);
            double angle = calcAngle(u,v);
            info->ang_biff.push_back(angle);
            fprintf(angleFile,"%.60lf\n",angle);    
        }
        ptr = ptr->next;
    }
    fclose(angleFile);
}

// Calcula o numero de segmentos na arvore (segmento nao eh uma aresta aqui !)
// O vetor size_segments contem todos os segmentos da arvore
void calcSegments (InfoTree *info)
{
    Graph *g = info->g;
    visit.assign(g->total_nodes,false);
    seg_size = 0;
    dfs(g->listNodes);
    info->size_segments = size_segments;

    // Escrever todos os segmentos em arquivo
    FILE *segmentFile = info->segmentFile;
    segmentFile = fopen("Output/segments.txt","a");
    for (int i = 0; i < (int)info->size_segments.size(); i++)
        fprintf(segmentFile,"%.60lf\n",info->size_segments[i]);
    fclose(segmentFile);
}

// Imprime em arquivo a posicao (x,y,z) de cada terminal da arvore
void calcTerminals (InfoTree *info)
{
    Graph *g = info->g;
    FILE *terminalFile = info->terminalFile;
    terminalFile = fopen("Output/terminals.txt","w+");
    
    Node *ptr = g->listNodes;
    while (ptr != NULL)
    {
        // Eh terminal se nao possuir nenhum vizinho
        if (ptr->num_edges == 0)
            fprintf(terminalFile,"%.10lf %.10lf %.10lf\n",ptr->x,ptr->y,ptr->z);
        ptr = ptr->next;
    }

    fclose(terminalFile);
}