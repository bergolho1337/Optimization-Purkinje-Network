#include "../include/queue.h"

// Construtor da fila
Queue* newQueue ()
{
	Queue *queue = (Queue*)malloc(sizeof(Queue));
	queue->head = NULL;
	queue->last = NULL;
	queue->in_the_queue = 0;
	return queue;
}

// Construtor de um QNode da fila
QNode* newQNode (QNode *next, QNode *prev, Node *node)
{
	QNode *qNode = (QNode*)malloc(sizeof(QNode));
	qNode->next = next;
	qNode->prev = prev;
	qNode->node = node;
	return qNode;
}

// Funcao de enfileirar
void Enqueue (Queue **q, Node *p)
{
	// Fila vazia
	if ((*q)->head == NULL)
	{
		QNode *qNode = newQNode(NULL,NULL,p);
		(*q)->head = qNode;
		(*q)->last = qNode;
	}
	// Tem alguem na fila
	else
	{
		QNode *qNode, *aux;
		aux = (*q)->last;
		qNode = newQNode(NULL,aux,p);
		aux->next = qNode;
		(*q)->last = qNode;
	}
	(*q)->in_the_queue++;
}

// Funcao de desinfileirar
Node* Dequeue (Queue **q)
{
	Node *ptr = (*q)->head->node;
	if ((*q)->head == (*q)->last)
	{	
		(*q)->head = NULL;
		(*q)->last = NULL;
	}
	else
	{
		(*q)->head = (*q)->head->next;
		(*q)->head->prev = NULL;
	}
	(*q)->in_the_queue--;
	return ptr;
}

bool isEmpty (Queue *q)
{
	return (q->in_the_queue == 0) ? true : false;
}

// Imprime os elementos na fila
void printQueue (Queue *q)
{
	QNode *qNode = q->head;
	cout << "============== GROWING QUEUE =========================================" << endl;
	cout << "head --> last" << endl;
	cout << "In the queue = " << q->in_the_queue << endl;
	while (qNode != NULL)
	{
		cout << qNode->node->id << " ";
		qNode = qNode->next;
	}
	cout << "\n======================================================================" << endl;
	cout << endl;
}