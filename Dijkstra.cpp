#include <ctime>
#include <fstream>
#include <Windows.h>
#include "GraphAdjMatrix.h"
#include "GraphAdjList.h"
#include "PlikInputOutput.h"
#include "Queue.h"

using namespace std;

//Do pomiaru czasu 
LARGE_INTEGER getTimer()
{
	LARGE_INTEGER time;
	DWORD_PTR oldmask = SetThreadAffinityMask(GetCurrentThread(), 0);
	QueryPerformanceCounter(&time);
	SetThreadAffinityMask(GetCurrentThread(), oldmask);
	return time;
}

void PrzygotujDane(Naglowek* naglowek, Krawedzie* tableEdge, float gestosc)
{
	int k = 0, index;
	Krawedzie* temp;
	int maxIloscKrawedzi;

	if (gestosc == 1)
	{
		for (int i = 0; i < naglowek->liczbaWierzcholkow; i++)
		{
			for (int j = i + 1; j < naglowek->liczbaWierzcholkow; j++)
			{
				tableEdge[k].poczatkowyWierzcholek = i;
				tableEdge[k].koncowyWierzcholek = j;
				tableEdge[k].waga = 10 + rand()%200;
				k++;
			}
		}
	}
	else
	{
		maxIloscKrawedzi = naglowek->liczbaWierzcholkow * (naglowek->liczbaWierzcholkow - 1) / 2;
		temp = new Krawedzie[maxIloscKrawedzi];
		k = 0;
		for (int i = 0; i < naglowek->liczbaWierzcholkow; i++)
		{
			for (int j = i + 1; j < naglowek->liczbaWierzcholkow; j++)
			{
				temp[k].poczatkowyWierzcholek = i;
				temp[k].koncowyWierzcholek = j;
				temp[k].waga = 10 + rand() % 200;
				k++;
			}
		}

		for (int i = 0; i < naglowek->liczbaKrawedzi; i++)
		{
			index = (rand() % (maxIloscKrawedzi - i));
			tableEdge[i].poczatkowyWierzcholek = temp[index].poczatkowyWierzcholek;
			tableEdge[i].koncowyWierzcholek = temp[index].koncowyWierzcholek;
			tableEdge[i].waga = temp[index].waga;

			//Zamieniam wybrany element z elementem na koñcu tabeli
			std::swap(temp[index], temp[(naglowek->liczbaKrawedzi-1)-i]);
		}
		delete[] temp;
	}
}


void printPath(std::ofstream& Plik, int* parent, int j)
{
	// Base Case : If j is source 
	if (parent[j] == -1)
		return;

	printPath(Plik, parent, parent[j]);

	Plik << j << "  ";
}

void saveToFile(int* dist, int n, int* parent, int start)
{
	std::ofstream Plik;
	Plik.open("DijkstryGraph.txt", std::ios::out);            //Otwieram plik do zapisu   

	Plik << "                       Odleglosc   Droga" << std::endl;
	for (int i = 0; i < n; i++)
	{
		Plik << std::right << std::setw(5) <<  start << " -> " << std::setw(7) << std::left << i << std::setw(8);

		if (dist[i] != INT_MAX)
		{
			Plik << std::right << dist[i] << std::setw(12) << start << "  ";
		}
		else
		{
			Plik << "brak" << std::setw(4) << start << "  ";
		}

		printPath(Plik, parent, i);
		Plik << std::endl;
	}

	Plik.close();
}

//template<typename T>
//void DijkstraDistances(T* G, int start, bool printToFile = false)
//{
//	int* dist;                              //tablica dystansów do wierzcho³ka
//	int* parent;                            //tablica poprzedników
//	Queue* prQueue;                         //WskaŸnik do kolejki prioryterowej
//
//	prQueue = new Queue(G->NumberOfVertices());
//
//	dist = new int[G->NumberOfVertices()];
//	parent = new int[G->NumberOfVertices()];
//
//	//Ustalam dystanse na INT_MAX (symulacja nieskoñczonoœci) i dodajê do kolejki
//	for (int v = 0; v < G->NumberOfVertices(); v++)
//	{
//		dist[v] = INT_MAX;
//		prQueue->Insert(dist[v], v);
//		parent[v] = -1;
//	}
//
//	//Dla wierzcho³ka startowego
//	dist[start] = 0;
//	prQueue->DecreaseKey(dist[start], start);
//
//	while (!prQueue->IsEmpty())
//	{
//		int u = prQueue->Extract()->vertex;
//	
//		EdgeList* pEdgie = G->IncydentEdgies(u);                //lista krawêdzi incydentnych dla u
//
//		for (EdgeNode* node = pEdgie->head; node != 0; node = node->next)
//		{
//			int v = node->opposite(u)->vertex;
//
//			if (prQueue->IsInQueue(v) && dist[u] != INT_MAX && (node->dist + dist[u]) < dist[v])
//			{
//				dist[v] = node->dist + dist[u];
//				prQueue->DecreaseKey(dist[v], v);
//				parent[v] = u;
//			}
//		}
//	}
//
//	if (printToFile)
//		saveToFile(dist, G->NumberOfVertices(), parent, start);
//
//	delete[] dist;
//	delete[] parent;
//}

template<typename T>
void DijkstraDistances(T* G, int start, bool printToFile = false)
{
	int* dist;                              //tablica dystansów do wierzcho³ka
	int* parent;                            //tablica poprzedników
	Queue* prQueue;                         //WskaŸnik do kolejki prioryterowej
	EdgeNode* incydentEdgie;                //krawêdŸ incydentna

	prQueue = new Queue(G->NumberOfVertices());

	dist = new int[G->NumberOfVertices()];
	parent = new int[G->NumberOfVertices()];

	//Ustalam dystanse na INT_MAX (symulacja nieskoñczonoœci) i dodajê do kolejki
	for (int v = 0; v < G->NumberOfVertices(); v++)
	{
		dist[v] = INT_MAX;
		prQueue->Insert(dist[v], v);
		parent[v] = -1;
	}

	//Dla wierzcho³ka startowego
	dist[start] = 0;
	prQueue->DecreaseKey(dist[start], start);

	while (!prQueue->IsEmpty())
	{
		int u = prQueue->Extract()->vertex;

		for (incydentEdgie = G->first(u); incydentEdgie != nullptr; incydentEdgie = G->next(u))
		{
			int v = incydentEdgie->opposite(u)->vertex;
			if (prQueue->IsInQueue(v) && dist[u] != INT_MAX && (incydentEdgie->dist + dist[u]) < dist[v])
			{
				dist[v] = incydentEdgie->dist + dist[u];
				prQueue->DecreaseKey(dist[v], v);
				parent[v] = u;
			}
		}
	}

	if (printToFile)
		saveToFile(dist, G->NumberOfVertices(), parent, start);

	delete[] dist;
	delete[] parent;
}


int main()
{
	//Obiekty wykorzystywane do przygotowania danych
	Naglowek* naglowek;									//wskaznik do klasy naglowka (odczyt danych z pliku)
	Krawedzie* tableKrawedzie;							//tabela krawedzi (odczyt danych z pliku)

	int a;                                              //sposob reprezentacji(Lista, Macierz)
	int d;                                              //sposob przygotowania danych (z pliku, generowanie) 
	std::string nazeaPliku;                             //nazwa pliku z danymi
	float dens[4] = { 0.25, 0.5, 0.75, 1 };             //gêstoœæ grafu
	int liczebnosc;                                     //liczba wierzcholkow

	//Graf
    VertexList* listaWierzcholkow;                      //Lista wierzcholkow
	EdgeList* listaKrawedzi;                            //Lista krawedzi
	GraphAdjMatrix* graphAdjMatrix;                     //Graf - macierz sasiedztwa
	GraphAdjList* graphAdjList;                         //Graf - lista sasiedztwa

	//Pomiar czasu
	LARGE_INTEGER start, stop, freq;
	double czas;
	int minCzas, maxCzas;
	float sredniCzas;
	std::ofstream plik;

	srand(time(NULL));   //Funkcja ustawia punkt startowy, który jest stosowany do generowania serii pseudo losowych liczb calkowitych.
						 //Funkcja rand bedzie generowala zawsze ta sama sekwencje liczb, jezeli nie zostanie wywolana wczesniej funkcja srand, 
						 //lub funkcja srand zostanie wywolana z argumentem równym 	
		
	do
	{
		std::cout << "Wybierz sposob reprezentacji grafu:" << std::endl
			      << "1 Lista sasiedztwa" << std::endl
			      << "2 Macierz sasiedztwa" << std::endl
			      << "0 exit" << std::endl << std::endl;
		std::cin >> a;

		if (a != 0)
		{
			std::cout << "Sposob przygotowania danych:" << std::endl
			          << "1 Zaczytane z pliku" << std::endl
					  << "2 Badanie efektywnosci" << std::endl << std::endl;
			std::cin >> d;

			if (d == 1)
			{
				std::cout << "Podaj nazwe pliku z danymi:" << std::endl;
				std::cin >> nazeaPliku;
			}
			if (d == 2)
			{
				std::cout << "Podaj liczbe wierzcholkow:" << std::endl;
				std::cin >> liczebnosc;
			}

			naglowek = new Naglowek();

			if (d == 1)                                         //Dane pobierane z pliku
			{
				std::ifstream Plik;
				Plik.open(nazeaPliku, std::ios::in);            //Otwieram plik do odczytu     

				if (!Plik.is_open())
				{
					std::cout << "Blad otwarcia pliku!!!!!" << std::endl;
					system("pause");
					return 1;
				}

				Plik >> naglowek;
				tableKrawedzie = new Krawedzie[naglowek->liczbaKrawedzi];
				for (int i = 0; i < naglowek->liczbaKrawedzi; i++)
				{
					Plik >> tableKrawedzie[i];
				}
				Plik.close();

				//Tworzê listê wierzcholkow
				listaWierzcholkow = new VertexList();
				for (int i = 0; i < naglowek->liczbaWierzcholkow; i++)
				{
					listaWierzcholkow->AddToHead(i);
				}

				//Tworzê listê krawedzi
				listaKrawedzi = new EdgeList();
				for (int i = 0; i < naglowek->liczbaKrawedzi; i++)
				{
					listaKrawedzi->AddToHead(listaWierzcholkow->getVertex(tableKrawedzie[i].poczatkowyWierzcholek), listaWierzcholkow->getVertex(tableKrawedzie[i].koncowyWierzcholek), tableKrawedzie[i].waga);
				}

				//Budowa grafu
				if (a == 1)				//Lista s¹siedztwa
				{
					graphAdjList = new GraphAdjList(naglowek->liczbaWierzcholkow);
					graphAdjList->GraphFill(listaKrawedzi);
					DijkstraDistances<GraphAdjList>(graphAdjList, naglowek->wierzcholekStartowy, true);
					//graphAdjList->printAll();
					delete graphAdjList;
				}
				if (a == 2)             //Macierz s¹siedztwa
				{
					graphAdjMatrix = new GraphAdjMatrix(naglowek->liczbaWierzcholkow);

					graphAdjMatrix->GraphFill(listaKrawedzi);
					DijkstraDistances<GraphAdjMatrix>(graphAdjMatrix, naglowek->wierzcholekStartowy, true);
					//graphAdjList->printAll();
					delete graphAdjMatrix;
				}

				delete listaKrawedzi;
				delete listaWierzcholkow;
				delete[] tableKrawedzie;
			}
			else                                                //Dane generowane na potrzeby badania efektywnosci
			{
				plik.open("Dijsktra.txt", std::ios::app);

				for (float gestosc : dens)
				{
					int liczbaKrawedzi = (liczebnosc * (liczebnosc - 1) / 2) * gestosc;
					naglowek->liczbaKrawedzi = liczbaKrawedzi;
					naglowek->liczbaWierzcholkow = liczebnosc;
					naglowek->wierzcholekStartowy = 0;

					tableKrawedzie = new Krawedzie[naglowek->liczbaKrawedzi];				

					sredniCzas = 0;
					minCzas = 1000000;
					maxCzas = -1;

					

					for (int ilosc = 0; ilosc < 100; ilosc++)
					{
						//Przygotowanie danych	
						PrzygotujDane(naglowek, tableKrawedzie, gestosc);

						//Tworzê listê wierzcholkow
						listaWierzcholkow = new VertexList();
						for (int i = 0; i < naglowek->liczbaWierzcholkow; i++)
						{
							listaWierzcholkow->AddToHead(i);
						}

						//Tworzê listê krawedzi
						listaKrawedzi = new EdgeList();
						for (int i = 0; i < naglowek->liczbaKrawedzi; i++)
						{
							listaKrawedzi->AddToHead(listaWierzcholkow->getVertex(tableKrawedzie[i].poczatkowyWierzcholek), listaWierzcholkow->getVertex(tableKrawedzie[i].koncowyWierzcholek), tableKrawedzie[i].waga);
						}

						QueryPerformanceFrequency(&freq);    //Do pomiaru czasu

						//Budowa grafu
						if (a == 1)
						{
							graphAdjList = new GraphAdjList(naglowek->liczbaWierzcholkow);
							graphAdjList->GraphFill(listaKrawedzi);

							start = getTimer();
							DijkstraDistances(graphAdjList, naglowek->wierzcholekStartowy, false);
							stop = getTimer();

							czas = 1000 * (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;

							sredniCzas = sredniCzas + czas;

							if (czas < minCzas) minCzas = czas;
							if (czas > maxCzas) maxCzas = czas;

							delete graphAdjList;
						}
						if (a == 2)
						{
							graphAdjMatrix = new GraphAdjMatrix(naglowek->liczbaWierzcholkow);
							graphAdjMatrix->GraphFill(listaKrawedzi);

							start = getTimer();
							DijkstraDistances(graphAdjMatrix, naglowek->wierzcholekStartowy, false);
							stop = getTimer();

							czas = 1000 * (stop.QuadPart - start.QuadPart) / (double)freq.QuadPart;

							sredniCzas = sredniCzas + czas;

							if (czas < minCzas) minCzas = czas;
							if (czas > maxCzas) maxCzas = czas;

							delete graphAdjMatrix;
						}

						delete listaKrawedzi;
						delete listaWierzcholkow;
					}
					sredniCzas = sredniCzas / 100;

					plik <<  "Liczebnosc;" << liczebnosc << ";Gestosc;" << gestosc << ";sredni czas;" << sredniCzas << std::endl;
					std::cout << "Gestosc    " << std::left << std::setw(8) << std::dec << gestosc << "sredni czas    " << std::setw(6) << std::dec << sredniCzas << std::endl;

					delete[] tableKrawedzie;
				}
				plik.close();
			}
		}
		std::cout << std::endl;
	} 
	while (a != 0);

	return 0;
}
