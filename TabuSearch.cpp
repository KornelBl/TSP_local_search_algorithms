#include "pch.h"
#include "TabuSearch.h"


void TabuSearch::setRandomPath()
{
		int length = m->vertices - 1;
		bestPath = new int[length + 2];
		currentPath = new int[length + 2];

		//losowa droga startowa
		srand(time(NULL));
		int* helper = new int[length];
		for (int i = 0; i < length; i++) {
			helper[i] = i + 1;
		}
		int index;
		bestPath[0] = currentPath[0] = bestPath[length + 1] = currentPath[length + 1] = 0;
		for (int i = 0; i < length; i++) {
			index = rand() % (length - i);
			bestPath[i + 1] = currentPath[i + 1] = helper[index];
			swap(helper, length - i - 1, index);
		}
		currentCost = bestCost = m->distanceFunction(currentPath);
		delete[] helper;
}

inline void TabuSearch::copyPath(int * from, int * to)
{
	for (int i = 0; i < m->vertices - 1; i++) {
		to[i + 1] = from[i + 1];
	}
}

void TabuSearch::swap(int * T, int a, int b)
{
	int buf = T[a];
	T[a] = T[b];
	T[b] = buf;
}

void TabuSearch::initTabuMatrix() 
{
	int len = m->vertices - 1;
	tabuMatrix = new int*[len];
	for (int i = 0; i < len; i++) {
		tabuMatrix[i] = new int[len];
		for (int j = 0; j < len; j++)
			tabuMatrix[i][j] = 0;
	}
}

void TabuSearch::initMoveValues_swap()
{
	int len = m->vertices - 1;
	moveValue = new int*[len];
	for (int i = 0; i < m->vertices - 2; i++) {
		moveValue[i] = new int[len];
		for (int j = i + 1; j < m->vertices - 1; j++) {
			moveValue[i][j] = calculateNeighbourValue_swap(i, j);
		}
	}
}

void TabuSearch::renewMoveValues_swap(int i, int j)
{
	int ind1, ind2;
	if (i > j) {
		int bufor = i;
		i = j;
		j = bufor;
	}
	int ix[6] = { i - 1,i ,i + 1,j - 1,j ,j + 1 };
	int max = 6;
	if (j == m->vertices - 2) max = 5;
	for (ind1 = 0; ind1 < i - 1; ind1++) {
		for (int at2 = 0; at2 < max; at2++) {
			ind2 = ix[at2];
			moveValue[ind1][ind2] = calculateNeighbourValue_swap(ind1, ind2);
		}
	}
	for (; ind1 < i + 2; ind1++) {
		for (ind2 = ind1 + 1; ind2 < m->vertices - 1; ind2++) {
			moveValue[ind1][ind2] = calculateNeighbourValue_swap(ind1, ind2);
		}
	}

	for (; ind1 < j - 1; ind1++) {
		for (int at2 = 3; at2 < max; at2++) {
			ind2 = ix[at2];
			moveValue[ind1][ind2] = calculateNeighbourValue_swap(ind1, ind2);
		}
	}
	for (; ind1 < j + 2; ind1++) {
		for (ind2 = ind1 + 1; ind2 < m->vertices - 1; ind2++) {
			moveValue[ind1][ind2] = calculateNeighbourValue_swap(ind1, ind2);
		}
	}
}

void TabuSearch::chooseNeighbour_swap(int i, int j)
{
	swap(currentPath, i + 1, j + 1);
	currentCost += moveValue[i][j];
	if (currentCost < bestCost) {
		bestCost = currentCost;
		copyPath(currentPath, bestPath);
	}
}

int TabuSearch::calculateNeighbourValue_swap(int i, int j)
{
	int neighbourCost = 0;
	//czy to potrzebne
	if (i > j) {
		int bufor = i;
		i = j;
		j = bufor;
	}
	i++;
	j++;
	if (j - i == 1) {
		neighbourCost -= m->cost(currentPath[i], currentPath[j]);
		neighbourCost += m->cost(currentPath[j], currentPath[i]);

	}
	else {
		neighbourCost -= m->cost(currentPath[j - 1], currentPath[j]);
		neighbourCost -= m->cost(currentPath[i], currentPath[i + 1]);
		neighbourCost += m->cost(currentPath[j - 1], currentPath[i]);
		neighbourCost += m->cost(currentPath[j], currentPath[i + 1]);
	}

	neighbourCost -= m->cost(currentPath[i - 1], currentPath[i]);
	neighbourCost += m->cost(currentPath[i - 1], currentPath[j]);
	neighbourCost -= m->cost(currentPath[j], currentPath[j + 1]);
	neighbourCost += m->cost(currentPath[i], currentPath[j + 1]);
	return neighbourCost;
}

void TabuSearch::algorithm(double time)
{
	Timing timer;
	timer.startCount();	
	//stworzenie listy tabu, wype³nienie zerami
	initTabuMatrix();
	initMoveValues_swap();
	for (int _ = 0; _ < iterations; _++) {


		//wybor nastepcy
		//w trakcie wybierania nastepcow aktualizowac liste tabu

		int bestMoveValue = INT_MAX;
		int ind1, ind2;

		for (int i = 0; i < m->vertices - 2; i++) {
			for (int j = i + 1; j < m->vertices - 1; j++) {
				if (tabuMatrix[i][j] != 0) {
					tabuMatrix[i][j] -= 1;
					//ewentualna aspiracja
					continue;
				}
				if (moveValue[i][j] < bestMoveValue) {
					ind1 = i;
					ind2 = j;
					bestMoveValue = moveValue[i][j];
				}
			}
		}
		chooseNeighbour_swap(ind1, ind2);

		//aktualizacja listy tabu
		tabuMatrix[ind1][ind2] = lifespan;
		//aktualizacja sasiedztwa
		renewMoveValues_swap(ind1, ind2);
		
		timer.endCount();
		if (time && timer.getResult() >= time) {
			break;
		}
	}
}

void TabuSearch::showResult()
{
	std::cout << "Najkrótsza œcie¿ka: ";
	for (int i = 0; i < m->vertices; i++) {
		std::cout << bestPath[i] << "->";
	}
	std::cout << "0\nKoszt: " << bestCost << std::endl;
	
}

TabuSearch::TabuSearch(Matrix* matrix)
{
	srand(time(NULL));
	m = matrix;
	setRandomPath();
	lifespan = m->vertices * 3;
}


TabuSearch::~TabuSearch()
{
	delete[] bestPath;
	delete[] currentPath;
}
