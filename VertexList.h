#ifndef VERTEXLIST_H
#define VERTEXLIST_H

#include <iostream>

class VertexNode {
public:
	int vertex;
	VertexNode* next;

	VertexNode() {
		next = nullptr;
	}

	VertexNode(int v, VertexNode* n = nullptr) {
		vertex = v;
		next = n;
	}
};

class VertexList {
private:
	VertexNode* head;
	VertexNode* tail;
public:
	VertexList() {
		head = nullptr;
		tail = nullptr;
	}

	~VertexList();

	bool isEmpty();
	void AddToHead(int);
	void AddToTail(int);
	void DeleteFromHead();
	void DeleteFromTail();
	void DeleteNode(int);
	VertexNode* getVertex(int);
	void printAll();
};

VertexList::~VertexList()
{
	VertexNode* temp;
	while (!isEmpty())
	{
		temp = head->next;
		delete head;
		head = temp;
	}
}

bool VertexList::isEmpty()
{
	return head == nullptr;
}

void VertexList::AddToHead(int v)
{
	head = new VertexNode(v, head);
	if (tail == nullptr)
	{
		tail = head;
	}
}

void VertexList::AddToTail(int v)
{
	if (tail != nullptr)
	{
		tail->next = new VertexNode(v);
		tail = tail->next;
	}
	else
	{
		head = tail = new VertexNode(v);
	}
}

void VertexList::DeleteFromHead()
{
	VertexNode* temp = head;
	if (tail == head)
	{
		head = tail = nullptr;
	}
	else
	{
		head = temp->next;
	}
	delete temp;
}

void VertexList::DeleteFromTail()
{
	if (head == tail)
	{
		delete head;
		tail = head = nullptr;
	}
	else
	{
		VertexNode* temp;
		for (temp = head; temp->next != tail; temp = temp->next);
		delete tail;
		tail = temp;
		tail->next = nullptr;
	}
}

void VertexList::DeleteNode(int v)
{
	if (head != nullptr)
		if (head == tail && v == head->vertex)
		{
			delete head;
			head = tail = nullptr;
		}
		else if (v == head->vertex)
		{
			VertexNode* temp = head;
			head = head->next;
			delete temp;
		}
		else
		{
			VertexNode* pret, * temp;
			for (pret = head, temp = head->next; temp != 0 && temp->vertex != v; pret = pret->next, temp = temp->next);
			if (temp != nullptr)
			{
				pret->next = temp->next;
				if (temp == tail)
					tail = pret;
				delete temp;
			}
		}
}

VertexNode* VertexList::getVertex(int v)
{
	VertexNode* temp;
	for (temp = head; temp != nullptr && temp->vertex != v; temp = temp->next);
	return temp;
}

void VertexList::printAll()
{
	for (VertexNode* temp = head; temp != nullptr; temp = temp->next)
		std::cout << temp->vertex << "  " << std::endl;
}

#endif