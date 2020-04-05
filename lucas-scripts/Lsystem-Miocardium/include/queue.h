#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "../include/purkinje.h"

using namespace std;

// =============================================================================================================
// Estrutura de um Node da fila
struct QNode
{
	QNode *next;				// Ponteiro para o proximo elemento da fila
	QNode *prev;				// Ponteiro para o elemento anterior da fila
	Node *node;					// Ponteiro para o no em crescimento da arvore
}typedef QNode;

QNode* newQNode (QNode *next, QNode *prev, Node *node);
// =============================================================================================================
// =============================================================================================================
// Estrutura da fila
struct Queue
{
	QNode *head;				// Ponteiro para a cabeca da fila
	QNode *last;				// Ponteiro para o ultimo da fila
	int in_the_queue;			// Contador de nos em crescimento na fila
}typedef Queue; 

Queue* newQueue ();
void Enqueue (Queue **q, Node *p);
bool isEmpty (Queue *q);
Node* Dequeue (Queue **q);
void printQueue (Queue *q);
// =============================================================================================================
// =============================================================================================================

#endif