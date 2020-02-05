#include "pch.h"
#include "TabuSearch.h"
#include <fstream>
#include <string>
using namespace std;

int TabuSearch::getBestCost()
{
	return bestCost;
}

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

void TabuSearch::setNNPath()
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
	int minIndex;
	int at = 0;
	int minCost = INT_MAX;
	bestPath[0] = currentPath[0] = bestPath[length + 1] = currentPath[length + 1] = 0;
	for (int i = 0; i < length; i++) {
		minCost = INT_MAX;
		for (int j = 0; j < length - i; j++) {
			if (m->cost(at, helper[j]) < minCost) {
				minCost = m->cost(at, helper[j]);
				minIndex = j;
			}
		}
		at = bestPath[i + 1] = currentPath[i + 1] = helper[minIndex];
		swap(helper, length - i - 1, minIndex);
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

void TabuSearch::deleteTabuMatrix()
{
	for (int i = 0; i < m->vertices - 1; i++) {
		delete[] tabuMatrix[i];
	}
	delete[] tabuMatrix;
}

void TabuSearch::deleteTabuList()
{
	tabuList.~List();
}

void TabuSearch::addTabuMatrix(int i, int j)
{
	tabuMatrix[i][j] = tabuLifespan;
	tabuMatrix[j][i] = tabuLifespan;
}

bool TabuSearch::isInTabuMatrix(int i, int j)
{
	if (tabuMatrix[i][j] != 0) {
		tabuMatrix[i][j]--;
		return true;
	}
	else return false;
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

void TabuSearch::initTabuList()
{

}

void TabuSearch::addTabuList(int i, int j)
{
	tabuList.addTop(i, j);
	if (tabuList.getSize() >= tabuLifespan) tabuList.deleteBottom();
}

bool TabuSearch::isInTabuList(int i, int j)
{
	if (tabuList.findPtr(i, j) != NULL)return true;
	else return false;
}

void TabuSearch::diversificate()
{
	diversificationCounter = 0;
	//part of setRandomPath
	int* helper = new int[m->vertices - 1];
	for (int i = 0; i < m->vertices - 1; i++) {
		helper[i] = i + 1;
	}
	int index;
	for (int i = 0; i < m->vertices - 1; i++) {
		index = rand() % (m->vertices - 1 - i);
		currentPath[i + 1] = helper[index];
		swap(helper, m->vertices - i - 2, index);
	}
	currentCost = m->distanceFunction(currentPath);
	delete[] helper;
	//init move values
	for (int i = 0; i < m->vertices - 2; i++) {
		for (int j = i + 1; j < m->vertices - 1; j++) {
			moveValue[i][j] = (this->*calculateNeighbourValue)(i, j);
		}
	}
	//init tabu Matrix
	(this->*deleteTabu)();
	(this->*initTabu)();
}

bool TabuSearch::aspirationCriterium(int i, int j)
{
	if ((moveValue[i][j] + currentCost) < bestCost) return true;
	else return false;
}

void TabuSearch::deleteMoveValue()
{
	for (int i = 0; i < m->vertices - 2; i++) {
		delete[] moveValue[i];
	}
	delete[] moveValue;
}

void TabuSearch::initMoveValues()
{
	int len = m->vertices - 1;
	moveValue = new int*[len];
	for (int i = 0; i < len ; i++) {
		moveValue[i] = new int[len];
		for (int j = 0; j < len; j++) {
			moveValue[i][j] = (this->*calculateNeighbourValue)(i, j);
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

void TabuSearch::renewMoveValues_symetricInvert(int i, int j)
{
	int ind1, ind2;
	if (i > j) {
		int bufor = i;
		i = j;
		j = bufor;
	}
	/*for (int iAt = 0; iAt < i; iAt++) {
		for (int jAt = j + 1; jAt < m->vertices - 1; jAt++) {
			moveValue[iAt][jAt] = calculateNeighbourValue_symetricInvert(iAt, jAt);
		}
	}*/ //to jest niepotrzebne
	moveValue[i][j] = -moveValue[i][j];

	// iAt, jAt;
	for (int iAt = 0; iAt < i + 1; iAt++) {
		for (int jAt = i; jAt < j + 1; jAt++) {
			moveValue[i][j] = calculateNeighbourValue_symetricInvert(iAt, jAt);
		}
	}

	for (int iAt = i; iAt < j + 1; iAt++) {
		for (int jAt = iAt+1; jAt < m->vertices - 1; jAt++) {
			moveValue[i][j] = calculateNeighbourValue_symetricInvert(iAt, jAt);
		}
	}
	


}

void TabuSearch::renewMoveValues_insert(int i, int j)
{	
	int ind1, ind2;

	int ind_i[3] = { i, 0 ,0 };
	int ind_j[5] = { i - 1 ,i ,j ,0 ,0 };

	if (i > j) {
		for (ind1 = 0; ind1 < j; ind1++) {
			for (ind2 = j + 1; ind2 < i - 2; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1][ind2 + 1];
			}
		}
		for (ind1 = j + 1; ind1 < i - 1; ind1++) {
			for (ind2 = 0; ind2 < j - 1; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1 + 1][ind2];
			}
			for (ind2 = j + 1; ind2 < i - 2; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1 + 1][ind2 + 1];
			}
			for (ind2 = i + 1; ind2 < m->vertices - 1; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1 + 1][ind2];
			}
		}
		for (ind1 = i + 1; ind1 < m->vertices - 1; ind1++) {
			for (ind2 = j + 1; ind2 < i - 2; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1][ind2 + 1];
			}
		}
		ind_i[1] = j;
		ind_i[2] = j - 1;

		ind_j[3] = j - 1;
		ind_j[4] = i - 2;
	}
	else {
		for (ind1 = m->vertices - 2; ind1 > j + 1; ind1--) {
			for (ind2 = j - 1; ind2 > i + 1; ind2--) {
				moveValue[ind1][ind2] = moveValue[ind1][ind2 - 1];
			}
		}
		for (ind1 = j; ind1 > i + 1; ind1--) {
			for (ind2 = 1; ind2 < i - 1; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1 - 1][ind2];
			}
			for (ind2 = j - 1; ind2 > i + 1; ind2--) {
				moveValue[ind1][ind2] = moveValue[ind1 - 1][ind2 - 1];
			}
			for (ind2 = j + 2; ind2 < m->vertices - 1; ind2++) {
				moveValue[ind1][ind2] = moveValue[ind1 - 1][ind2];
			}
		}
		for (ind1 = 0; ind1 < i; ind1++) {
			for (ind2 = j - 1; ind2 > i - 2; ind2--) {
				moveValue[ind1][ind2] = moveValue[ind1][ind2 - 1];
			}
		}
		ind_i[1] = i + 1;
		ind_i[2] = j + 1;

		ind_j[3] = j + 1;
		ind_j[4] = i + 1;
	}

	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 5; b++) {
			if (ind_i[a] >= m->vertices - 1 || ind_j[b] >= m->vertices - 1 || ind_i[a] < 0 || ind_j[b] < 0) continue;
			moveValue[ind_i[a]][ind_j[b]] = calculateNeighbourValue_insert(ind_i[a], ind_j[b]);
		}
	}

}

void TabuSearch::chooseNeighbour_swap(int i, int j)
{
	swap(currentPath, i + 1, j + 1);
	currentCost += moveValue[i][j];
	if (currentCost < bestCost) {
		diversificationCounter = 0;
		bestCost = currentCost;
		copyPath(currentPath, bestPath);
	}
	else {
		diversificationCounter++;
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

void TabuSearch::chooseNeighbour_invert(int i, int j)
{
	int bufor;
	if (i > j) {
		bufor = i;
		i = j;
		j = bufor;
	}
	i++;
	j++;
	for (int temp = 0; temp < (j - i + 1) / 2; temp++) {
		bufor = currentPath[i + temp];
		currentPath[i + temp] = currentPath[j - temp];
		currentPath[j - temp] = bufor;
	}
	currentCost += moveValue[i][j];
}

int TabuSearch::calculateNeighbourValue_symetricInvert(int i, int j)
{
	int neighbourCost = currentCost;
	if (i > j) {
		int bufor = i;
		i = j;
		j = bufor;
	}
	i++;
	j++;

	neighbourCost -= m->cost(currentPath[i - 1], currentPath[i]);
	neighbourCost += m->cost(currentPath[i - 1], currentPath[j]);
	neighbourCost -= m->cost(currentPath[j], currentPath[j + 1]);
	neighbourCost += m->cost(currentPath[i], currentPath[j + 1]);
	return neighbourCost;
}

void TabuSearch::chooseNeighbour_insert(int i, int j)
{
	int bufor = currentPath[j + 1];
	if (j > i) {
		for (int at = j + 1; at > i + 1; at--) {
			currentPath[at] = currentPath[at - 1];
		}
		currentPath[i + 1] = bufor;
	}
	else {
		for (int at = j + 1; at < i; at++) {
			currentPath[at] = currentPath[at + 1];
		}
		currentPath[i] = bufor;
	}

	currentCost += moveValue[i][j];
	if (currentCost < bestCost) {
		diversificationCounter = 0;
		bestCost = currentCost;
		copyPath(currentPath, bestPath);
	}
	else {
		diversificationCounter++;
	}
}

int TabuSearch::calculateNeighbourValue_insert(int i, int j)
{
	i++;
	j++;
	int neighbourValue = 0;
	if (i - j == 1) {
		return 0;
	}
	else {
		//rozlaczamy i
		neighbourValue -= m->cost(currentPath[i - 1], currentPath[i]);

		//rozlaczamy j
		neighbourValue -= m->cost(currentPath[j - 1], currentPath[j]);
		neighbourValue -= m->cost(currentPath[j], currentPath[j + 1]);
		neighbourValue += m->cost(currentPath[j - 1], currentPath[j + 1]);

		//laczymy i z j
		neighbourValue += m->cost(currentPath[i - 1], currentPath[j]);
		neighbourValue += m->cost(currentPath[j], currentPath[i]);
	}
	return neighbourValue;
}

void TabuSearch::algorithm(double time)
{
	//std::string filename = "TS_323.csv";
	//fstream file;
	//file.open(filename, ios_base::out);



	Timing timer;
	timer.startCount();	

	//stworzenie listy tabu, wype³nienie zerami
	(this->*initTabu)();
	initMoveValues();
	int diversificationCounter = 0;
	int ind1, ind2, bestMoveValue;
	for (int _ = 0; _ < iterations; _++) {
		//obsluga pliku



		bestMoveValue = INT_MAX;
		//wybor nastepcy
		//w trakcie wybierania nastepcow aktualizowac liste tabu
		for (int i = 0; i < m->vertices - 1; i++) {
			for (int j = i + 1; j < m->vertices - 1; j++) {
				if ((this->*isInTabu)(i,j)) {
					//aspiracja
					if (aspiration) {
						if (moveValue[i][j] < bestMoveValue && aspirationCriterium(i, j)) {
							ind1 = i;
							ind2 = j;
							bestMoveValue = moveValue[i][j];
						}
					}
					continue;
				}
				else if (moveValue[i][j] < bestMoveValue) {
						ind1 = i;
						ind2 = j;
						bestMoveValue = moveValue[i][j];
					}

				if (asymetricNeighbourhood) {
					if ((this->*isInTabu)(j, i)) {
						//aspiracja
						if (aspiration) {
							if (moveValue[j][i] < bestMoveValue && aspirationCriterium(j, i)) {
								ind1 = j;
								ind2 = i;
								bestMoveValue = moveValue[j][i];
							}
						}
						continue;
					}
					else if (moveValue[j][i] < bestMoveValue) {
						ind1 = j;
						ind2 = i;
						bestMoveValue = moveValue[j][i];
					}
				}
				
			}
		}
		//podmiana na wybranego s¹siada
		(this->*chooseNeighbour)(ind1, ind2);
		//dywersyfikacja po braku poprawy najlepszego rozwiazanie przez okreœlon¹ liczbe iteracji
		if (diversificationCounter == diversificationPoint && diversification) {
			diversificate();
		}
		else {
			//aktualizacja listy tabu
			(this->*addTabu)(ind1, ind2);
			//aktualizacja sasiedztwa
			(this->*renewMoveValues)(ind1, ind2);
		}
		timer.endCount();

		//plik
		//file << timer.getResult() << ";" << currentCost << ";" << bestCost << endl;


		if (time && timer.getResult() >= time) {
			break;
		}
	}
	//file.close();
}

void TabuSearch::showResult()
{
	std::cout << "Najkrótsza œcie¿ka: ";
	for (int i = 0; i < m->vertices; i++) {
		std::cout << bestPath[i] << "->";
	}
	std::cout << "0\nKoszt: " << bestCost << std::endl;
	
}

TabuSearch::TabuSearch(Matrix* matrix, int startPath, int ifDiversification, int ifAspiration, int neighbourType, int tabuContainer, int lifespan, int iterations, int criticalPoint)
{
	srand(time(NULL));
	m = matrix;
	switch (startPath)
	{
	case 0:
		setRandomPath();
		break;
	case 1:
		setNNPath();
		break;
	default:
		break;
	}

	switch (ifDiversification)
	{
	case 0:
		diversification = true;
		break;
	case 1:
		diversification = false;
		break;
	default:
		break;
	}

	switch (ifAspiration)
	{
	case 0:
		aspiration = true;
		break;
	case 1:
		aspiration = false;
		break;
	default:
		break;
	}

	switch (neighbourType)
	{
	case 0:
		asymetricNeighbourhood = false;
		chooseNeighbour = &TabuSearch::chooseNeighbour_swap;
		calculateNeighbourValue = &TabuSearch::calculateNeighbourValue_swap;
		renewMoveValues = &TabuSearch::renewMoveValues_swap;
		break;
	case 1:
		asymetricNeighbourhood = true;
		chooseNeighbour = &TabuSearch::chooseNeighbour_insert;
		calculateNeighbourValue = &TabuSearch::calculateNeighbourValue_insert;
		renewMoveValues = &TabuSearch::renewMoveValues_insert;
		break;
	case 2:
		asymetricNeighbourhood = true;
		chooseNeighbour = &TabuSearch::chooseNeighbour_invert;
		calculateNeighbourValue = &TabuSearch::calculateNeighbourValue_symetricInvert;
		renewMoveValues = &TabuSearch::renewMoveValues_symetricInvert;
		break;
	default:
		break;
	}

	switch (tabuContainer)
	{
	case 0:
		initTabu = &TabuSearch::initTabuMatrix;
		deleteTabu = &TabuSearch::deleteTabuMatrix;
		addTabu = &TabuSearch::addTabuMatrix;
		isInTabu = &TabuSearch::isInTabuMatrix;
		break;
	case 1:
		initTabu = &TabuSearch::initTabuList;
		deleteTabu = &TabuSearch::deleteTabuList;
		addTabu = &TabuSearch::addTabuList;
		isInTabu = &TabuSearch::isInTabuList;
		break;
	default:
		break;
	}



	tabuLifespan = lifespan;
	diversificationPoint = criticalPoint;
	this->iterations = iterations;
}


TabuSearch::~TabuSearch()
{
	delete[] bestPath;
	delete[] currentPath;
	(this->*deleteTabu)();
	deleteMoveValue();
}
