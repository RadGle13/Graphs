#ifndef GRAPHADJLIST_H
#define GRAPHADJLIST_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include "EdgeList.h"

// klasa reprezentuj¹ca element listy sasiedztwa
class AdjListNode {
public:
	EdgeNode* edge;
	AdjListNode* next;
	AdjListNode() {
		next = nullptr;
	}
};

//Klasa reprezentuj¹ca listê sasiedztwa
class AdjList
{
public:
	AdjListNode* head;

	AdjList()
	{
		head = nullptr;
	}
};

class GraphAdjList {
private:
	AdjList** adjList;                       //Tablica adresów list s¹siedztwa dla wierzcho³ków
	int vertexQ;                             //Liczba wierzcho³ków
	AdjListNode* position;                   //bie¿¹ca pozycja dla metod first, next

public:
	GraphAdjList(int);
	~GraphAdjList();

	void GraphFill(const EdgeList*);
	int NumberOfVertices();                 //zwraca liczbê wierzcho³ków grafu
	//EdgeList* IncydentEdgies(int);          //zwraca listê krawêdzi incydentnych dla wierzcho³ka
	EdgeNode* first(int);                   //Zwraca pierwsz¹ krawêdŸ incydentn¹ dla wierzchó³ka
	EdgeNode* next(int);                    //Zwraca nastêpn¹ krawêdŸ incydentn¹ dla wierzchó³ka

	void AddEdge(EdgeNode*);
	void printAll();
};

GraphAdjList::GraphAdjList(int Q)
{
	vertexQ = Q;
	adjList = new AdjList* [vertexQ];
	for (int i = 0; i < vertexQ; i++)
	{
		adjList[i] = new AdjList();
	}
}

GraphAdjList::~GraphAdjList()
{
	AdjListNode* temp;
	for (int i = 0; i < vertexQ; i++)
	{
		while (adjList[i]->head != nullptr)
		{
			temp = adjList[i]->head->next;
			delete adjList[i]->head;
			adjList[i]->head = temp;
		}
	}
	delete[] adjList;
}

void GraphAdjList::GraphFill(const EdgeList* list)
{
	for (EdgeNode* temp = list->head; temp != 0; temp = temp->next)
	{
		int startV = temp->firstVertex->vertex;
		int endV = temp->secondVertex->vertex;
	
		AdjListNode* tempN = new AdjListNode();
		tempN->edge = temp;
		tempN->next = adjList[startV]->head;
		adjList[startV]->head = tempN;

		tempN = new AdjListNode();
		tempN->edge = temp;
		tempN->next = adjList[endV]->head;
		adjList[endV]->head = tempN;
	}
}

void GraphAdjList::AddEdge(EdgeNode* edge)
{
	int startV = edge->firstVertex->vertex;
	int endV = edge->secondVertex->vertex;

	AdjListNode* tempN = new AdjListNode();
	tempN->edge = edge;
	tempN->next = adjList[startV]->head;
	adjList[startV]->head = tempN;

	tempN = new AdjListNode();
	tempN->edge = edge;
	tempN->next = adjList[endV]->head;
	adjList[endV]->head = tempN;
}

int GraphAdjList::NumberOfVertices()
{
	return vertexQ;
}

//EdgeList* GraphAdjList::IncydentEdgies(int v)
//{
//	EdgeList* incidentEdgiesList = new EdgeList();
//
//	AdjListNode* pEdgie = adjList[v].head;
//	while (pEdgie != nullptr)
//	{
//		incidentEdgiesList->AddToHead(pEdgie->edge->firstVertex, pEdgie->edge->secondVertex, pEdgie->edge->dist);
//		pEdgie = pEdgie->next;
//	}
//	return incidentEdgiesList;
//}

/* Pomys³ zaczerpniêty z ksi¹¿ki                                                                    */
/* A Practical Introduction to Data Structures and Algorithm Analysis Third Edition (C++ Version)   */
/* Clifford A. Shaffer Department of Computer Science Virginia Tech Blacksburg, VA 24061            */
/* str 401 - 424 */

EdgeNode* GraphAdjList::first(int v)
{
	if (adjList[v]->head != nullptr)
	{
		position = adjList[v]->head;
		return adjList[v]->head->edge;
	}
	return nullptr;
}

EdgeNode* GraphAdjList::next(int v)
{
	if (position->next != nullptr)
	{
		position = position->next;
		return position->edge;
	}
	return nullptr;
}

void GraphAdjList::printAll()
{
	for (int i = 0; i < vertexQ; i++)
	{
		std::cout << i << " -> ";
		for (AdjListNode* temp = adjList[i]->head; temp != 0; temp = temp->next)
		{
			std::cout  << "(" << temp->edge->firstVertex->vertex << "  " << temp->edge->secondVertex->vertex << ")  " ;
		}
		std::cout << std::endl;
	}
}
#endif