#include "../include/purkinje.h"
#include "../include/miocardium.h"
#include "../include/queue.h"

int cont_segments = 1;					// Contador de segmentos jah feitos para o ramo
double rand_number[1000];				// Vetor de numeros aleatorios, seguindo uma distribuicao normal
Queue *queue;							// Ponteiro para a fila de nos em crescimento
double sobel[3][3][3];					// Filtro de sobel em 3D

// Construtor de um no
Node* newNode (int id, double x, double y, double z, bool grow, double d_ori[])
{
	Node *node = (Node*)malloc(sizeof(Node));
	node->id = id;
	node->x = x;
	node->y = y;
	node->z = z;
	node->grow = grow;
	node->isTerminal = false;
	node->num_edges = 0;
	memcpy(node->d_ori,d_ori,sizeof(double)*3);
	node->cell = NULL;
	node->edges = NULL;
	node->next = NULL;
	return node;
}

// Cosntrutor de uma aresta
Edge* newEdge (int id, double size, Node *dest)
{
	Edge *edge = (Edge*)malloc(sizeof(Edge));
	edge->id = id;
	edge->size = size;
	edge->dest = dest;
	edge->next = NULL;
	return edge;
}

// Construtor do grafo. Constroi uma arvore de Purkinje automaticamente pelo L-System
Graph* newGraph (double dx, char *cloudFileName, char *terminalFileName)
{
	int cont;
	Node *ptr;
	Graph *g = (Graph*)malloc(sizeof(Graph));
	//double *rootPoint = readCloudPoints(g,"Miocardium/vent_original_tempo.vtk");
	double *rootPoint = readCloudPoints(g,cloudFileName);
	//readTerminalPoints(g,"Miocardium/terminaisBenja_1.dat");
	readTerminalPoints(g,terminalFileName);
	setLimits(g);
	setRootPoint(g->terminalPoint,5,rootPoint);
	initializeRandomGenerator();
	makeRoot(&g,rootPoint,dx);
	for (int k = 0; k < Iter; k++)
	{	
		cont = queue->in_the_queue;
		printf("[!] Iteration %d! Growing branches ...\n",k+1);
		// Retira um no em crescimento da fila e aplica a regra de crescimento nele
		while (cont > 0)
		{
			ptr = Dequeue(&queue);
			growBranch(&g,ptr);
			cont--; 
		}
	}
	printf("[+] Purkinje structure was build with sucess!\n");
	printf("[+] There are %d nodes\n",g->total_nodes);
	printf("[+] There are %d edges\n",g->total_edges);

	joinTerminalPoints(&g);
	return g;
}

// Cresce um ramo a partir do L-System
void growBranch (Graph **g, Node *p)
{
	Lsystem(g,p,0);
}

// Aplica a regra do L-System sobre um Node em crescimento
void Lsystem (Graph **g, Node *p, int type)
{
	double d_gra[3];
	// Calcula o gradiente de distancia pela aplicacao do filtro de Sobel
	calculateGradient(*g,p,d_gra);
	// Verifica qual o tipo de ramo
	switch (type)
	{
		// Inicio do ramo, gera dois filhos
		case 0: {
					calculateGrowDirection(g,p,d_gra,1);
					calculateGradient(*g,p,d_gra);
					calculateGrowDirection(g,p,d_gra,-1);
					break;
				}
		// Meio do ramo, gera um filho
		case 1: {
					calculateGrowDirection(g,p,d_gra,0);
					break;
				}
		default: break;
	}
}

// Calcula a direcao de crescimento do ramo: d = d_ori + w1*d_gra
void calculateGrowDirection (Graph **g, Node *p, double d_gra[], double teta)
{
	double norm;
	double d[3], d_rot[3];

	// O vetor d_ori jah esta calculado para o Node
	// O vetor d_gra jah foi calculada na ultima funcao

	// Substituir na formula (pra direita)
	if (teta >= 0)
	{
		rotate(p->d_ori,d_rot,M_PI/4.0);
		for (int i = 0; i < 3; i++)
			//d[i] = d_rot[i] + w_1*d_gra[i];
			d[i] = p->d_ori[i] + w_1*d_gra[i];	
	}
	else
	{
		rotate(p->d_ori,d_rot,-M_PI/4.0);
		for (int i = 0; i < 3; i++)
			//d[i] = d_rot[i] - w_1*d_gra[i];
			d[i] = p->d_ori[i] - w_1*d_gra[i];
	}
	// Vetor unitario da direcao de crescimento
	norm = sqrt(pow(d[0],2) + pow(d[1],2) + pow(d[2],2));
	for (int i = 0; i < 3; i++) d[i] /= norm;

	// Criar o ramo na direcao calculada, repete-se a geracao do ramo 5 vezes dentro desse procedimento
	generateBranch(g,p,d);
	// Resetar o contador de segmentos
	cont_segments = 1;
}

// Gera o ramo na direcao calculada
void generateBranch (Graph **g, Node *p, double d[])
{
	// Coordenadas do novo Node
	d[0] = p->x + calcSizeBranch()*d[0];
	d[1] = p->y + calcSizeBranch()*d[1];
	d[2] = p->z + calcSizeBranch()*d[2];

	// !!!! Verificar se o ponto NAO passa em alguma restricao  !!!!
	if (!checkTerminals(g,p,d[0],d[1],d[2]) && !checkCollisionTree(g,p,d[0],d[1],d[2]) \
	  && !checkCollisionMiocardium(g,p,d[0],d[1],d[2]) && !checkLimits(g,p,d[0],d[1],d[2]))
	{
		Node *ptrNew = insertNodeGraph(g,p,d[0],d[1],d[2]);
		if (ptrNew != NULL)
		{
			insertEdgeGraph(g,p->id,ptrNew->id);
			cont_segments++;

			// Deve-se repetir 4 vezes para completar o crescimento
			if (cont_segments <= 1) 
				Lsystem(g,ptrNew,1);
			// Enfileirar o ultimo
			else
				Enqueue(&queue,ptrNew);
		}
		
	}
}

// Constroi a raiz da arvore
void makeRoot (Graph **g, double *rootPoint, double dx)
{
	Node *ptr1, *ptr2;
	queue = newQueue();
	(*g)->total_nodes = (*g)->total_edges = 0;
	(*g)->listNodes = NULL; 
	(*g)->lastNode = NULL;
	ptr1 = insertNodeGraph(g,NULL,rootPoint[0],rootPoint[1],rootPoint[2]);
	ptr2 = insertNodeGraph(g,(*g)->listNodes,rootPoint[0],rootPoint[1],rootPoint[2]+l_bra);
	insertEdgeGraph(g,0,1);
	// Liga ao ponto mais proximo do miocardio
	if (ptr2 != NULL && ptr1 != NULL) linkToMiocardium(g,ptr2);
}

// Insere um noh no grafo
Node* insertNodeGraph (Graph **g, Node *prev, double x, double y, double z)
{
	// Verificar se o nodo jah existe
	bool alreadyExist = false;
	Node *ptr = (*g)->listNodes;
	while (ptr != NULL)
	{
		if (ptr->x == x && ptr->y == y && ptr->z == z)
		{
			alreadyExist = true;
			//printf("Id = %d\n",ptr->id);
			//printf("(%e,%e,%e) - (%e,%e,%e)\n",ptr->x,ptr->y,ptr->z,x,y,z);
			//printf("Node already exist!\n");
			break;
		}
		ptr = ptr->next;
	}
	
	ptr = (*g)->listNodes;
	if (alreadyExist)
		return NULL;
	else
	{
		double d_ori[3];
		calcOriginalDirection(d_ori,prev,x,y,z);
		Node *ptrNew = newNode((*g)->total_nodes++,x,y,z,false,d_ori);
		// Primeiro no da lista
		if (ptr == NULL)
			(*g)->listNodes = ptrNew;
		// Percorrer a lista de nos ate o final
		else
		{
			while (ptr->next != NULL)
				ptr = ptr->next;
			ptr->next = ptrNew;
		}
		return ptrNew;
	}
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
	
	if (checkEdge(ptr1,ptr2))
	{
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
	else
		return;
	
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
	exit(-1);
	return NULL;
}

// Calcula a norma euclidiana entre dois pontos
double calcNorm (double x1, double y1, double z1, double x2, double y2, double z2)
{
	double norm = sqrt(pow((x1-x2),2) + pow((y1-y2),2) + pow((z1-z2),2));
	if (norm > 1.0e-20)
		return (norm);
	else
		return (1.0e-20);
}

// Verifica se um ponto esta dentro do voxel dado pelo cubo de tamanho (width,lenght,height)
bool isInsideCube (Node *ptr, double x, double y, double z, double width, double lenght, double height)
{
	return (ptr->x <= x + lenght && ptr->x >= x && ptr->y >= y - width && ptr->y <= y \
	 && ptr->z >= z - height && ptr->z <= z);
}

// Checa se 2 nos sao iguais. Usa norma euclidiana
bool checkAlreadyExist (Node *node1, Node *node2)
{
	double norm = calcNorm(node1->x,node1->y,node1->z,node2->x,node2->y,node2->z);
	if (norm == 1.0e-20)
		return true;
	else
		return false;
}

// Inicializa o vetor de numeros aleatorios
void initializeRandomGenerator ()
{
    int i;
	srand(time(NULL));
	default_random_engine generator;
	normal_distribution<double> distribution(0.0,l_bra*0.2);			// 0.1*l_bra gerou bons resultados
	for (i = 0; i < 1000; i++)
		rand_number[i] = distribution(generator);
}

// Juntar os pontos terminais que sobraram
void joinTerminalPoints (Graph **g)
{
	int idMostNear;
	double dist, norm;
	double x, y, z;
	Point *terminalPoint = (*g)->terminalPoint;
	Node *ptr, *prev;
	// Percorrer os primeiros pontos de ativacao e verificar qual o Node do grafo mais proximo
	for (int i = 0; i < num_terminal; i++)
	{
		// Se ainda nao encontrou algum ponto da arvore proximo a algum terminal ...
		if (!terminalPoint[i].taken)
		{
			terminalPoint[i].taken = true;
			dist = 1.0e+10;
			idMostNear = -1;
			ptr = (*g)->listNodes;
			while (ptr != NULL)
			{
				norm = calcNorm(terminalPoint[i].x,terminalPoint[i].y,terminalPoint[i].z,ptr->x,ptr->y,ptr->z);
				if (norm < dist && ptr->num_edges < 2)
				{
					dist = norm;
					idMostNear = ptr->id;
					x = ptr->x;
					y = ptr->y;
					z = ptr->z;
				}
				ptr = ptr->next;
			}
			// Pegar referencia para Node mais proximo do ponto terminal e calcular a direcao de crescimento original
			prev = searchNode(*g,idMostNear);
			Node *ptr = insertNodeGraph(g,prev,terminalPoint[i].x,terminalPoint[i].y,terminalPoint[i].z);
			if (ptr != NULL)
			{
				insertEdgeGraph(g,prev->id,ptr->id);
				ptr->isTerminal = true;
			}
			printf("=====================================================================\n");
			printf("Most near Node = %d\n",idMostNear);
			printf("Terminal point = (%lf,%lf,%lf) - Most near Node = (%lf,%lf,%lf)\n", \
				terminalPoint[i].x,terminalPoint[i].y,terminalPoint[i].z,x,y,z);
			printf("Distance = %lf\n",dist);
			printf("=====================================================================\n");
		}
		
	}
}

// Calcula a direcao original de crescimento do Node que estamos criando
void calcOriginalDirection (double d_ori[], Node *prev, double x, double y, double z)
{
	if (prev == NULL)
	{
		d_ori[0] = 0;
		d_ori[1] = 0;
		d_ori[2] = 0;	
	}
	else
	{
		double norm = calcNorm(prev->x,prev->y,prev->z,x,y,z);
		d_ori[0] = (x - prev->x) / norm;
		d_ori[1] = (y - prev->y) / norm;
		d_ori[2] = (z - prev->z) / norm;
	}
}

// Calcula o tamanho do segmento usando o vetor de numeros aleatorios global
double calcSizeBranch ()
{
	int i = rand() % 1000;
	double number = rand_number[i];
	return (l_bra + number);
	//return l_bra;
}

void calculateGradient (Graph *g, Node *p, double d_gra[])
{
	bool tick;
	double x, y, z;
	int i, j, k;
	double width, lenght, height;
	double norm = 0.0;
	Node *ptr;
	// Dimensoes do mini-cubo
	width = t_cube; lenght = t_cube; height = t_cube;
	// Aumentar o tamanho do mini-cubo caso o gradiente resulte em nulo
	while (norm == 0.0)
	{
		width++; lenght++; height++;
		// Seta as posicoes do primeiro mini-cubo a ser testado
		x = p->x - ((width*3.0)/2.0); y = p->y - ((lenght*3.0)/2.0); z = p->z - ((height*3.0)/2.0);
		// Iterar em relacao a todos os mini-cubos
		for (i = 0; i < 3; i++)
		{
			z = p->z + ((height*3.0)/2.0);
			for (j = 0; j < 3; j++)
			{
				y = p->y + ((lenght*3.0)/2.0);
				for (k = 0; k < 3; k++)
				{
					ptr = g->listNodes;
					tick = false;
					// Nao checar o cubo central
					if (i == 1 && j == 1 && k == 1) sobel[i][j][k] = 0;
					// Para os outros
					else
					{
						// Testar para todos os nos do grafo se algum deles esta dentro do mini-cubo atual
						while (ptr != NULL)
						{
							// Se acharmos pelo um no, marcamos o mini-cubo
							if (isInsideCube(ptr,x,y,z,width,lenght,height))
							{
								tick = true;
								break;
							}
							ptr = ptr->next;
						}
						(tick) ? sobel[i][j][k] = 1 : sobel[i][j][k] = 0;
					}
					y -= lenght;
				}
				z -= height;	
			}
			x += width;
		}
		// Convolucao
		d_gra[0] = (-sobel[0][0][0] + sobel[0][0][2] - 2*sobel[0][1][0] + 2*sobel[0][1][2] - sobel[0][2][0] + sobel[0][2][2]) +
			(-2*sobel[1][0][0] + 2*sobel[1][0][2] - 4*sobel[1][1][0] + 4*sobel[1][1][2] - 2*sobel[1][2][0] + 2*sobel[1][2][2]) +
				(-sobel[2][0][0] + sobel[2][0][2] - 2*sobel[2][1][0] + 2*sobel[2][1][2] - sobel[2][2][0] + sobel[2][2][2]);

		d_gra[1] = (sobel[0][0][2] + 2*sobel[1][0][2] + sobel[2][0][2] - sobel[0][2][2] - 2*sobel[1][2][2] - sobel[2][2][2]) +
			(2*sobel[0][0][1] + 4*sobel[1][0][1] + 2*sobel[2][0][1] - 2*sobel[0][2][1] - 4*sobel[1][2][1] - 2*sobel[2][2][1]) +
				(sobel[0][0][0] + 2*sobel[1][0][0] + sobel[2][0][0] - sobel[0][2][0] - 2*sobel[1][2][0] - sobel[2][2][0]);

		d_gra[2] = (-sobel[0][2][2] - 2*sobel[0][2][1] - sobel[0][2][0] - 2*sobel[1][2][2] - 4*sobel[1][2][1] - 2*sobel[1][2][0] - sobel[2][2][2] - 2*sobel[2][2][1] - sobel[2][2][0]) + (sobel[0][0][2] + 2*sobel[0][0][1] + sobel[0][0][0] + 2*sobel[1][0][2] + 4*sobel[1][0][1] + 2*sobel[1][0][0] + 		sobel[2][0][2] + 2*sobel[2][0][1] + sobel[2][0][0]);

		norm = sqrt(pow(d_gra[0],2)+pow(d_gra[1],2)+pow(d_gra[2],2)); 
	}
	// Invertendo o valor do gradiente de distância e calculando sua direção pelo vetor unitário
	d_gra[0] = -d_gra[0]/norm;
	d_gra[1] = -d_gra[1]/norm;
	d_gra[2] = -d_gra[2]/norm;
}

// Checa se o Node em crescimento ira colidir com algum outro Node jah na arvore
bool checkCollisionTree (Graph **g, Node *prev, double x, double y, double z)
{
	Node *ptr = (*g)->listNodes;
	while (ptr->next != NULL)
	{
		if (calcNorm(ptr->x,ptr->y,ptr->z,x,y,z) < Toler_tree && ptr->isTerminal == false)
		{
			//printf("\t[!] Tree collision between nodes: %d - %d\n",prev->id,ptr->id);
			//insertEdgeGraph(g,prev->id,ptr->id);
			return true;
		}
		ptr = ptr->next;
	}
	return false;
}

// Checar se o Node em crescimento ultrapassara os limites do dominio
bool checkLimits (Graph **g, Node *prev, double x, double y, double z)
{
	if (x >= (*g)->limits[0]-1 && x >= (*g)->limits[3]+1 && y >= (*g)->limits[1]-1 && y >= (*g)->limits[4]+1 \
	    && z >= (*g)->limits[2]-1 && z >= (*g)->limits[5]+1 )
		return false;
	else
		return true;
}

// Checa se o Node em crescimento ira colidir com algum ponto da nuvem de pontos do miocardio
bool checkCollisionMiocardium (Graph **g, Node *prev, double x, double y, double z)
{
	int id;
	double dist;
	int N = (*g)->numCloudPoint;
	Point *cloudPoint = (*g)->cloudPoint;
	// Achar qual o ponto da nuvem mais proximo do Node em crescimento
	id = searchMostNear(cloudPoint,N,x,y,z);
	dist = calcNorm(cloudPoint[id].x,cloudPoint[id].y,cloudPoint[id].z,x,y,z);
	// Checar se a distancia desse ponto eh menor que a tolerancia para colisao
	if (dist < Toler_miocardium)
	{
		// Inserir o Node do miocardio no grafo e gerar uma aresta de ligacao
		Node *nodeMio = insertNodeGraph(g,prev,cloudPoint[id].x,cloudPoint[id].y,cloudPoint[id].z);
		if (nodeMio != NULL)
		{
			insertEdgeGraph(g,prev->id,nodeMio->id);
			// Enfileirar ele como Node de crescimento
			Enqueue(&queue,nodeMio);
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

// Checa se o ponto em crescimento vai estar muito proximo de algum ponto terminal do miocardio
bool checkTerminals (Graph **g, Node *prev, double x, double y, double z)
{
	// Checar se o anterior for terminal retornar erro
	if (prev->isTerminal) return true;

	double dist;
	int N = (*g)->numTerminal;
	Point *terminalPoint = (*g)->terminalPoint;
	// Achar qual o ponto terminal mais proximo do Node candidato ao crescimento
	int id = searchMostNear(terminalPoint,N,x,y,z);
	dist = calcNorm(terminalPoint[id].x,terminalPoint[id].y,terminalPoint[id].z,x,y,z);
	// Checar se a distancia desse ponto eh menor que a tolerancia para colisao
	if (dist < Toler_terminal)
	{
		//printf("\t[!] Terminal collision ! Id = %d\n",id);
		// Marcar esse terminal como visto jah
		terminalPoint[id].taken = true;
		
		double d_ori[3];
		calcOriginalDirection(d_ori,prev,terminalPoint[id].x,terminalPoint[id].y,terminalPoint[id].z);
		// Inserir o Node terminal no grafo e gerar uma aresta de ligacao
		Node *nodeTerminal = insertNodeGraph(g,prev,terminalPoint[id].x,terminalPoint[id].y,terminalPoint[id].z);
		if (nodeTerminal != NULL)
		{
			nodeTerminal->isTerminal = true;
			insertEdgeGraph(g,prev->id,nodeTerminal->id);
			// **** NAO enfileirar ele como Node de crescimento *****
			return true;
		}
		else
			return false;
	}
	else
		return false;
}

// Faz ligacao da raiz com o miocardio pelo ponto mais proximo da nuvem de pontos
void linkToMiocardium (Graph **g, Node *last)
{
	int N = (*g)->numCloudPoint;
	Point *cloudPoint = (*g)->cloudPoint;
	double dist = 1.0e+10;
	double norm;
	int idMostNear = -1;
	// Descobre o ponto mais perto do miocardio
	for (int i = 0; i < N; i++)
	{
		norm = calcNorm(last->x,last->y,last->z,cloudPoint[i].x,cloudPoint[i].y,cloudPoint[i].z);
		if (norm < dist && norm > 1.0e-09)
		{
			idMostNear = i;
			dist = norm;
		}
	}
	// Criar o nodo e fazer a ligacao
	Node *ptrNew = insertNodeGraph(g,last,cloudPoint[idMostNear].x,cloudPoint[idMostNear].y,cloudPoint[idMostNear].z);
	if (ptrNew != NULL) insertEdgeGraph(g,last->id,ptrNew->id);

	// Fazer isso mais uma vez para garantir que temos um segmento na parede do miocardio
	last = ptrNew;
	dist = 1.0e+10;
	idMostNear = -1;
	for (int i = 0; i < N; i++)
	{
		norm = calcNorm(last->x,last->y,last->z,cloudPoint[i].x,cloudPoint[i].y,cloudPoint[i].z);
		if (norm < dist && norm > 1.0e-09)
		{
			idMostNear = i;
			dist = norm;
		}
	}
	ptrNew = insertNodeGraph(g,last,cloudPoint[idMostNear].x,cloudPoint[idMostNear].y,cloudPoint[idMostNear].z);
	if (ptrNew != NULL) insertEdgeGraph(g,last->id,ptrNew->id);

	Enqueue(&queue,ptrNew);
}

// Procura em um vetor de pontos pelo ponto mais proximo a coordenada (x,y,z)
// Evita de pegar pontos jah marcados
int searchMostNear (Point *points, int N, double x, double y, double z)
{
	int i;
	double norm;
	int id = -1;
	double dist = 1.0e+10;
	for (i = 0; i < N; i++)
	{
		if (!points[i].taken)
		{
			norm = calcNorm(points[i].x,points[i].y,points[i].z,x,y,z);
			if (norm < dist)
			{
				dist = norm;
				id = i;
			}
		}
	}
	return id;
}

// Descobre os valores maximos e minimos nos 3 eixos coordenados (x,y,z) e salva no vetor limits[] do Graph
void setLimits (Graph *g)
{
	double x, y, z;
	int N = g->numCloudPoint;
	// limits[0] = (minX,minY,minZ);
	// limits[1] = (maxX,maxY,maxZ);
	g->limits = (double*)calloc(6,sizeof(double));
	for (int i = 0; i < 3; i++)	g->limits[i] = 1.0e+10;
	for (int i = 3; i < 6; i++)	g->limits[i] = -1.0e+10;
	// Percorrer todos os pontos e verificar os valores (x,y,z)
	for (int i = 0; i < N; i++)
	{
		x = g->cloudPoint[i].x; y = g->cloudPoint[i].y; z = g->cloudPoint[i].z;

		// Checar os menores valores em cada eixo
		if (x < g->limits[0]) g->limits[0] = x;
		if (y < g->limits[1]) g->limits[1] = y;
		if (z < g->limits[2]) g->limits[2] = z;

		// Checar os maiores valores em cada eixo
		if (x > g->limits[3]) g->limits[3] = x;
		if (y > g->limits[4]) g->limits[4] = y;
		if (z > g->limits[5]) g->limits[5] = z;
	}
	printf("=============== LIMITS ================\n");
	printf("x - [%lf,%lf]\n",g->limits[0],g->limits[3]);
	printf("y - [%lf,%lf]\n",g->limits[1],g->limits[4]);
	printf("z - [%lf,%lf]\n",g->limits[2],g->limits[5]);
	printf("=======================================\n");
}

// Checa se na lista de aresta do nodo 1 se jah existe alguma ligacao com o nodo 2
bool checkEdge (Node *ptr1, Node *ptr2)
{
	Edge *ptrl = ptr1->edges;
	while (ptrl != NULL)
	{
		if (ptrl->id == ptr2->id)
			return false;
		ptrl = ptrl->next;
	}
	return true;
}

// Atribui o valor do ponto da raiz como sendo a coordenada do nodo terminal indice "id"
void setRootPoint (Point *terminals, int id, double *rootPoint)
{
	if (terminals == NULL || rootPoint == NULL) error("In setRootPoint()!");
	// Ponto da raiz sendo o primeiro terminal
	rootPoint[0] = terminals[id].x;
	rootPoint[1] = terminals[id].y;
	rootPoint[2] = terminals[id].z;
	
	// Ponto da raiz do Benjamin : (158.5,240.28,54.6504)
	rootPoint[0] = 158.5 / scale_factor;
	rootPoint[1] = 240.28 / scale_factor;
	rootPoint[2] = 54.6504 / scale_factor;

}

// Conta quantos terminais jah existem na arvore
void countTerminals (Point *terminals, int N)
{
	int cont = 0;
	for (int i = 0; i < N; i++)
	{
		if (terminals[i].taken) cont++;
	}
	printf("[!] There are %d terminals\n",cont);
}

// Calcula o angulo entre dois vetores
double calcAngle (double u[], double v[])
{
	double p_scalar = 0;
    for (int i = 0; i < 3; i++) p_scalar += u[i]*v[i];
    double u_norm, v_norm;
	u_norm = sqrt(pow(u[0],2) + pow(u[1],2) + pow(u[2],2));
	v_norm = sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
	return acos(p_scalar / (u_norm*v_norm)) * 180.0 / M_PI;
}

void rotate (double u[], double d[], double teta)
{
	d[0] = u[0]*(cos(teta) + pow(u[0],2)*(1-cos(teta))) + u[1]*(u[0]*u[1]*(1-cos(teta)) - u[2]*sin(teta)) \
	 + u[2]*(u[0]*u[2]*(1-cos(teta)) + u[1]*sin(teta));
	d[1] = u[0]*(u[0]*u[1]*(1-cos(teta)) + u[2]*sin(teta)) + u[1]*(cos(teta) + pow(u[1],2)*(1-cos(teta))) \
	 + u[2]*(u[1]*u[2]*(1-cos(teta)) - u[0]*sin(teta));
	d[2] = u[0]*(u[0]*u[2]*(1-cos(teta)) - u[1]*sin(teta)) + u[1]*(u[1]*u[2]*(1-cos(teta)) + u[0]*sin(teta)) \
	 + u[2]*(cos(teta) + pow(u[2],2)*(1-cos(teta)));
}