#ifndef QUEUE_H
#define QUEUE_H

class Qelement
{
public:
	int dist;       //dystans
	int vertex;     //wierzcho³ek
};

class Queue
{
private:
	Qelement** heap;       //Tabela wskaznikow do elementow  Qelement na ktorej buduje kopiec
	int size;              //Liczba elementow w kopcu
	int capacity;          //Pojemnosc kopca
	int* pos;              //tabela indeksow na potrzeby decreaseKey(), dla okreslenia pozycji wezla w kolejce

	//zwraca indeks rodzica dla podanego indeksu wierzcholka
	int Parent(int v) { return v / 2; }

	//zwraca indeks lewego dziecka dla podanego indeksu wierzcholka
	int LeftChildId(int v) { return 2 * v; }

	//zwraca indeks prawego dziecka dla podanego indeksu wierzcholka
	int RightChildId(int v) { return 2 * v + 1; }

	//Przywracanie wlasnosci kopca
	void DownHeap(int);
	void UpHeap(int);
	void swap(Qelement**, int, int);

public:
	Queue(int cp);
	~Queue();
	
	//czy kolejka pusta
	bool IsEmpty();

	//wstawia podana wartosc do kopca  
	void Insert(int, int);

	//Usuwa wierzcholek kopca zwracajac jego wartosc
	Qelement* Extract();

	//Zmniejsza wartosc dystansu danego wierzcholka
	void DecreaseKey(int, int);

	//Sprawdza czy dany wierzcholek 'v' jest jeszcze w kolejce
	bool IsInQueue(int v);
};

Queue::Queue(int cp)
{
	capacity = cp;
	heap = new Qelement * [capacity+1];    //tworzy tablica o rozmiarze rozmiar (capacity+1) bo kopiec od indeksu 1
	pos = new int[capacity];               //Tabela mapujaca wezly na pozycja w kolejce 
	size = 0;                              //pusta kolejka
}

Queue::~Queue()
{
	for (int i = 0; i < capacity + 1; i++)
	{
		delete heap[i];
	}
	delete[] heap;
	delete[] pos;
}

bool Queue::IsEmpty()
{
	return size == 0;
}

void Queue::swap(Qelement** hp, int ind1, int ind2)
{
	Qelement* tmp = hp[ind1];
	hp[ind1] = hp[ind2];
	hp[ind2] = tmp;
}

void Queue::UpHeap(int idx)
{
	while (idx > 1 && heap[idx]->dist < heap[Parent(idx)]->dist)
	{
		// Swap pozycje w kolejce
		pos[heap[idx]->vertex] = Parent(idx);
		pos[heap[Parent(idx)]->vertex] = idx;

		// Swap wezly 
		swap(heap, Parent(idx), idx);

		idx = Parent(idx);
	}
}

void Queue::DownHeap(int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = LeftChildId(idx);
	right = RightChildId(idx);

	if (left < size && heap[left]->dist < heap[smallest]->dist)
		smallest = left;

	if (right < size && heap[right]->dist < heap[smallest]->dist)
		smallest = right;

	if (smallest != idx)
	{
		// Swap pozycje w kolejce
		pos[heap[smallest]->vertex] = idx;
		pos[heap[idx]->vertex] = smallest;

		// Swap wezly 
		swap(heap, smallest, idx);

		DownHeap(smallest);
	}
}

void Queue::Insert(int dist, int v)
{
	Qelement* el = new Qelement();
	el->vertex = v;
	el->dist = dist;

	heap[size + 1] = el;                     //wstaw do kolejki
	pos[v] = size + 1;

	size++;                                  //zwieksz liczbe elementow kolejki
}

void Queue::DecreaseKey(int dist, int v)
{
	int i = pos[v];                          //pobieram indeks dla v w tabeli Heap

	if (heap[i]->dist < dist)                //Dystans dla wierzcholka v jest mniejszy, nie robie nic
		return;

	heap[i]->dist = dist;

	UpHeap(i);                               //przywracam wlasciwosc kopca w górê
}

Qelement* Queue::Extract()
{
	if (IsEmpty())                           //jezeli kopiec jest pusty zakoncz bez zadnej akcji
		return 0;

	Qelement* root = heap[1];                //przechowuje element z korzenia w zmiennej root

	Qelement* lastNode = heap[size];         //przechowuje w lastNode ostatni element kopca

	heap[1] = lastNode;                      //przesuwam do korzenia ostatni element

	// Swap pozycje w kolejce
	pos[root->vertex] = size;
	pos[lastNode->vertex] = 1;

	--size;                                  //zmniejsz liczbe elementow kopca

	if(size > 1)
		DownHeap(1);                         //przywracam wlasciwosc kopca w dó³
	
	return root;
}

bool Queue::IsInQueue(int v)
{
	if (pos[v] <= size)	return true;
	return false;
}
#endif
