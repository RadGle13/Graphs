#ifndef EDGELIST_H
#define EDGELIST_H

#include <iostream>
#include <iterator>
#include "VertexList.h"

using namespace std;

class EdgeNode {
public:
	VertexNode* firstVertex;
	VertexNode* secondVertex;
	int dist;
	EdgeNode* next;

	EdgeNode()
	{
		next = nullptr;
		firstVertex = nullptr;
		secondVertex = nullptr;
	}

	EdgeNode(VertexNode* f, VertexNode* s, int  d, EdgeNode* n = nullptr)
	{
		firstVertex = f;
		secondVertex = s;
		dist = d;
		next = n;
	}

	VertexNode* opposite(int);
};

VertexNode* EdgeNode::opposite(int v)
{
	if (firstVertex != nullptr && firstVertex->vertex == v) return secondVertex;
	if (secondVertex != nullptr && secondVertex->vertex == v) return firstVertex;

	return nullptr;
}

class EdgeList {
public:
	EdgeNode* head;
	EdgeNode* tail;

	EdgeList() {
		head = nullptr;
		tail = nullptr;
	}

	~EdgeList();

	bool isEmpty();
	void AddToHead(VertexNode*, VertexNode*, int);
	void AddToTail(VertexNode*, VertexNode*, int);
	void DeleteFromHead();
	void DeleteFromTail();
	EdgeNode* getEdge(VertexNode*, VertexNode*);
	void printAll();
};

EdgeList::~EdgeList()
{
	EdgeNode* temp;
	while (!isEmpty())
	{
		temp = head->next;
		delete head;
		head = temp;
	}
}

bool EdgeList::isEmpty()
{
	return head == 0;
}

void EdgeList::AddToHead(VertexNode* f, VertexNode* s, int d)
{
	head = new EdgeNode(f, s, d, head);
	if(tail == nullptr)
	{
		tail = head;
	}
}

void EdgeList::AddToTail(VertexNode* f, VertexNode* s, int d)
{
	if (tail != nullptr)
	{
		tail->next = new EdgeNode(f, s, d);
		tail = tail->next;
	}
	else 
	{
		head = tail = new EdgeNode(f, s, d);
	}
}

void EdgeList::DeleteFromHead()
{
	EdgeNode* temp = head;
	if(tail == head)
	{
		head = tail = nullptr;
	}
	else
	{
		head = temp->next;
	}
	delete temp;
}

void EdgeList::DeleteFromTail()
{
	if (head == tail)
	{
		delete head;
		tail = head = nullptr;
	}
	else
	{
		EdgeNode* temp;
		for (temp = head; temp->next != tail; temp = temp->next);
		delete tail;
		tail = temp;
		tail->next = nullptr;
	}
}

EdgeNode* EdgeList::getEdge(VertexNode* f, VertexNode* s)
{
	EdgeNode* temp;
	for (temp = head; temp != nullptr && (temp->secondVertex != f || temp->firstVertex != s); temp = temp->next);
	return temp;
}

void EdgeList::printAll()
{
	for (EdgeNode* temp = head; temp != nullptr; temp = temp->next)
		std::cout << temp->firstVertex->vertex << "  " << temp->secondVertex->vertex << std::endl;
}
#endif 
