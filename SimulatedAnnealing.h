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


	int tempIterations = 1000;
	int iterations = 10000;
	double maxT;
	double lastT;
	double minT = 0.01; 
	Matrix* m;

	void setRandomPath();
	void setNNPath();
	inline void copyPath(int* from, int* to);
	void swap(int* T, int a, int b);

	//neighbour functions
	void chooseNeighbour_swap(int i, int j);
	void calculateNeighbourCost_swap(int i, int j);

	void chooseNeighbour_insert(int i, int j);
	void calculateNeighbourCost_insert(int i, int j);

	void chooseNeighbour_invert(int i, int j);
	void calculateNeighbourCost_invert(int i, int j);

	//temperature functions
	inline double probability(double temperature, int cost);
	inline double nextTemp_geometric(int temperatureNumber);
	double factorGeometric = 0.95;
	inline double nextTemp_linear(int temperatureNumber);
	double linearDiff;
	inline double nextTemp_log(int temperatureNumber);
	double lnBase;

	typedef void(SimulatedAnnealing::*NeighbourFunction)(int, int);
	NeighbourFunction chooseNeighbour, calculateNeighbourCost;
	typedef double(SimulatedAnnealing::*TemperatureFunction)(int);
	TemperatureFunction nextTemp;


public:

	void algorithm(double time = 0);	
	void showResult();
	SimulatedAnnealing(Matrix* matrix,int neighbourType,int temperatureType);
	~SimulatedAnnealing();
};

