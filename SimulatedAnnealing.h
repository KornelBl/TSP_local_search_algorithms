#pragma once
#include <ctime>
#include <cmath>


class SimulatedAnnealing
{
	int* bestPath;
	int bestCost = INT_MAX;
	int* currentPath;
	int currentCost = INT_MAX;
	int neighbourCost = INT_MAX;

	int iterations = 100000;
	double maxT;
	double lastT;
	double minT = 0;
	Matrix* m;

	void setRandomPath();
	inline void copyPath(int* from, int* to);
	void swap(int* T, int a, int b);

	//neighbour functions
	void chooseNeighbour_swap(int i, int j);
	void calculateNeighbourCost_swap(int i, int j);

	void chooseNeighbour_insert(int i, int j);
	void calculateNeighbourCost_insert(int i, int j);

	void chooseNeighbour_invert(int i, int j);
	void calculateNeighbourCost_invert(int i, int j);

	inline double probability(double temperature, int cost);
	inline double nextTemp_09(double temperature);
	

	typedef void(SimulatedAnnealing::*NeighbourFunction)(int, int);
	NeighbourFunction chooseNeighbour, calculateNeighbourCost;
	typedef double(SimulatedAnnealing::*TemperatureFunction)(double);
	TemperatureFunction nextTemp;


public:

	void algorithm(double time = 0);	
	void showResult();
	SimulatedAnnealing(Matrix* matrix,int neighbourType);
	~SimulatedAnnealing();
};

