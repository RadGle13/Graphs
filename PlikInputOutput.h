#ifndef PLIKIMPUTOUTPUT_H
#define PLIKIMPUTOUTPUT_H

#include <fstream>

//klasa do odczytu naglowka z pliku
class Naglowek
{
public:
	int liczbaKrawedzi;
	int liczbaWierzcholkow;
	int wierzcholekStartowy;
};

//klasa do odczytu krawedzi z pliku
class Krawedzie
{
public:
	int poczatkowyWierzcholek;
	int koncowyWierzcholek;
	int waga;
};

std::ifstream& operator >> (std::ifstream& StrmWej, Naglowek* h)
{
	StrmWej >> h->liczbaKrawedzi;
	StrmWej >> h->liczbaWierzcholkow;
	StrmWej >> h->wierzcholekStartowy;

	return StrmWej;
}

std::ifstream& operator >> (std::ifstream& StrmWej, Krawedzie& e)
{
	StrmWej >> e.poczatkowyWierzcholek;
	StrmWej >> e.koncowyWierzcholek;
	StrmWej >> e.waga;

	return StrmWej;
}

#endif
