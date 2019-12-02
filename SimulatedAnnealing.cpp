#include "pch.h"
#include "SimulatedAnnealing.h"

void SimulatedAnnealing::setRandomPath(){
	int length = m->vertices - 1;
	bestPath = new int[length+2];
	currentPath = new int[length+2];

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

inline void SimulatedAnnealing::copyPath(int * from, int * to)
{
	for (int i = 0; i < m->vertices - 1; i++) {
		to[i + 1] = from[i + 1];
	}
}

void SimulatedAnnealing::chooseNeighbour_swap(int i, int j)
{
	swap(currentPath, i + 1, j + 1);
	currentCost = neighbourCost;
}

void SimulatedAnnealing::calculateNeighbourCost_swap(int i, int j)
{
	neighbourCost = currentCost;
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
	//neighbourCost = m->distanceFunction(neighbourPath);
}

void SimulatedAnnealing::chooseNeighbour_insert(int i, int j)
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
	currentCost = neighbourCost;

}

void SimulatedAnnealing::calculateNeighbourCost_insert(int i, int j)
{
	i++;
	j++;
	neighbourCost = currentCost;
	if (i - j == 1) {
		return;
	}
	else {
		//rozlaczamy i
		neighbourCost -= m->cost(currentPath[i - 1], currentPath[i]);

		//rozlaczamy j
		neighbourCost -= m->cost(currentPath[j - 1], currentPath[j]);
		neighbourCost -= m->cost(currentPath[j], currentPath[j + 1]);
		neighbourCost += m->cost(currentPath[j - 1], currentPath[j + 1]);

		//laczymy i z j
		neighbourCost += m->cost(currentPath[i - 1], currentPath[j]);
		neighbourCost += m->cost(currentPath[j], currentPath[i]);
	}
}

void SimulatedAnnealing::chooseNeighbour_invert(int i, int j)
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
	currentCost = neighbourCost;
}

void SimulatedAnnealing::calculateNeighbourCost_invert(int i, int j)
{
	neighbourCost = currentCost;
	if (i > j) {
		int bufor = i;
		i = j;
		j = bufor;
	}
	i++;
	j++;
	int bufor;
	for (int at = i; at < j; at++) {
		neighbourCost -= m->cost(currentPath[at], currentPath[at + 1]);
		neighbourCost += m->cost(currentPath[at+1], currentPath[at]);
	}
	neighbourCost -= m->cost(currentPath[i - 1], currentPath[i]);
	neighbourCost -= m->cost(currentPath[j], currentPath[j + 1]);

	neighbourCost += m->cost(currentPath[i - 1], currentPath[j]);
	neighbourCost += m->cost(currentPath[i], currentPath[j + 1]);

}

inline double SimulatedAnnealing::probability(double temperature, int cost)
{
	return exp((currentCost-cost)/temperature);
}

inline double SimulatedAnnealing::nextTemp_09(double temperature)
{
	return 0.95 * temperature;
}

void SimulatedAnnealing::swap(int* T, int a, int b) {
	int buf = T[a];
	T[a] = T[b];
	T[b] = buf;
	return;
}

void SimulatedAnnealing::algorithm(double time)
{
	Timing timer;
	timer.startCount();
	int index1, index2;
	for (double T = maxT; T > minT; T = (this->*nextTemp)(T)) {
		for (int i = 0; i < iterations; i++) {
			index1 = rand() % (m->vertices - 1);
			do
			{
				index2 = rand() % (m->vertices - 1);
			} while (index1 == index2);

			(this->*calculateNeighbourCost)(index1, index2);

			if (neighbourCost < currentCost) {
				(this->*chooseNeighbour)(index1, index2);  
				if (currentCost < bestCost) {
					bestCost = currentCost;
					copyPath(currentPath, bestPath);
				}
			}
			else if (((double)rand() / RAND_MAX) < probability(T, neighbourCost))
			{
				(this->*chooseNeighbour)(index1, index2);
			}
		}
		timer.endCount();
		if (time && timer.getResult() >= time) {
			lastT = T;
			break;
			}
		}
}

void SimulatedAnnealing::showResult()
{
	std::cout << "Najkrótsza œcie¿ka: ";
	for (int i = 0; i < m->vertices; i++) {
		std::cout << bestPath[i] << "->" ;
	}
	std::cout << "0\nKoszt: " << bestCost << std::endl;
	std::cout << "min now max Temperature" << minT << "\t" << lastT << "\t" << maxT;
}

SimulatedAnnealing::SimulatedAnnealing(Matrix* matrix,int neighbourType)
{
	srand(time(NULL));
	m = matrix;	
	setRandomPath();
	// temperature initialization
	maxT = currentCost;
	minT = pow(10,-5) * maxT;
	// setting chosen neighbour functions
	switch (neighbourType)
	{
	case 0:
		calculateNeighbourCost = &SimulatedAnnealing::calculateNeighbourCost_swap;
		chooseNeighbour = &SimulatedAnnealing::chooseNeighbour_swap;
		break;
	case 1:
		calculateNeighbourCost = &SimulatedAnnealing::calculateNeighbourCost_insert;
		chooseNeighbour = &SimulatedAnnealing::chooseNeighbour_insert;
		break;
	case 2:
		calculateNeighbourCost = &SimulatedAnnealing::calculateNeighbourCost_invert;
		chooseNeighbour = &SimulatedAnnealing::chooseNeighbour_invert;
		break;
	default:
		break;
	}
	nextTemp = &SimulatedAnnealing::nextTemp_09;
}

SimulatedAnnealing::~SimulatedAnnealing()
{
	delete[] bestPath;
	delete[] currentPath;
}
