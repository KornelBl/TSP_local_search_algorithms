#pragma once
#include <ctime>
//#include <queue>


class TabuSearch
{
	int iterations = 3'00'000;
	int* bestPath;
	int bestCost = INT_MAX;
	int* currentPath;
	int currentCost = INT_MAX;
	Matrix* m;


	void setRandomPath(); 
	inline void copyPath(int* from, int* to);
	void swap(int* T, int a, int b);

	//zbior tabu
	int lifespan;
	int** tabuMatrix;
	void initTabuMatrix();

	struct Neighbour {
		int ind1;
		int ind2;
		int movVal;
	};
	//zbior sasiadow
	int** moveValue;
	void initMoveValues_swap();
	void renewMoveValues_swap(int i,int j);
	//kolejka sasiadow
	//std::priority_queue<Neighbour> queue;


	//neighbourhood functions
	void chooseNeighbour_swap(int i, int j);
	int calculateNeighbourValue_swap(int i, int j);



public:
	void algorithm(double time = 0);
	void showResult();
	TabuSearch(Matrix* matrix);
	~TabuSearch();
};

