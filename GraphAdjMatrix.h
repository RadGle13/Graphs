#ifndef GRAPHADJMATRIX_H
#define GRAPHADJMATRIX_H

#include <fstream>
#include <iomanip>
#include "EdgeList.h"
#include "GraphAdjList.h"


class GraphAdjMatrix {
private:
	EdgeNode*** matrix;				    //macierz wskaŸników do obiektów krawêdzi
	int vertexQ;					    //liczba wierzcho³ków 
	int position;                       //bie¿¹ca pozycja dla metod first, next
public:
	GraphAdjMatrix(int);
	~GraphAdjMatrix();

	void GraphFill(const EdgeList*);
	int NumberOfVertices();               //zwraca liczbê wierzcho³ków grafu
	//EdgeList* IncydentEdgies(int);        //zwraca listê krawêdzi incydentnych dla wierzcho³ka
	EdgeNode* first(int);                 //Zwraca pierwsz¹ krawêdŸ incydentn¹ dla wierzchó³ka
	EdgeNode* next(int);                  //Zwraca nastêpn¹ krawêdŸ incydentn¹ dla wierzchó³ka

	void AddEdge(EdgeNode*);
	void printAll();
};

GraphAdjMatrix::GraphAdjMatrix(int Q)
{
	vertexQ = Q;
	matrix = new EdgeNode **[vertexQ];

	for (int i = 0; i < vertexQ; i++)
	{
		matrix[i] = new EdgeNode* [vertexQ];
	}

	for (int i = 0; i < vertexQ; i++)
	{
		for (int j = 0; j < vertexQ; j++)
		{
			matrix[i][j] = nullptr;
		}
	}
}

GraphAdjMatrix::~GraphAdjMatrix()
{
	for (int i = 0; i < vertexQ; i++)
	{
		delete matrix[i];
	}
	delete matrix;
}

void GraphAdjMatrix::GraphFill(const EdgeList* list)
{
	for (EdgeNode* temp = list->head; temp != 0; temp = temp->next)
	{
		matrix[temp->firstVertex->vertex][temp->secondVertex->vertex] = temp;
		matrix[temp->secondVertex->vertex][temp->firstVertex->vertex] = temp;
	}
}

void GraphAdjMatrix::AddEdge(EdgeNode* edge)
{
	matrix[edge->firstVertex->vertex][edge->secondVertex->vertex] = edge;
	matrix[edge->secondVertex->vertex][edge->firstVertex->vertex] = edge;
}

int GraphAdjMatrix::NumberOfVertices()
{
	return vertexQ;
}

//EdgeList* GraphAdjMatrix::IncydentEdgies(int v)
//{
//	EdgeList* incidentEdgiesList = new EdgeList();
//	for (int i = 0; i < vertexQ; i++)
//	{
//		if (matrix[v][i] != nullptr)
//		{
//			incidentEdgiesList->AddToHead(matrix[v][i]->firstVertex, matrix[v][i]->secondVertex, matrix[v][i]->dist);
//		}
//
//	}
//	return incidentEdgiesList;
//}

/* Pomys³ zaczerpniêty z ksi¹¿ki                                                                    */
/* A Practical Introduction to Data Structures and Algorithm Analysis Third Edition (C++ Version)   */
/* Clifford A. Shaffer Department of Computer Science Virginia Tech Blacksburg, VA 24061            */
/* str 401 - 424 */

EdgeNode* GraphAdjMatrix::first(int v)
{
	for (int i = 0; i < vertexQ; i++)
	{
		if (matrix[v][i] != nullptr)
		{
			position = i;
			return matrix[v][i];
		}
	}
	return nullptr;
}

EdgeNode* GraphAdjMatrix::next(int v)
{
	for (int i = position+1; i < vertexQ; i++)
	{
		if (matrix[v][i] != nullptr)
		{
			position = i;
			return matrix[v][i];
		}
	}
	return nullptr;
}

void GraphAdjMatrix::printAll()
{
	for (int i = 0; i < vertexQ; i++)
	{
		std::cout << i << " -> ";
		for (int j = 0; j < vertexQ; j++)
		{
			if (matrix[i][j] != 0)
			{
				std::cout << "(" << matrix[i][j]->firstVertex->vertex << "  " << matrix[i][j]->secondVertex->vertex << ")  ";
			}
		}
		std::cout << std::endl;
	}
}
#endif
