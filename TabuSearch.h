#pragma once
#include <ctime>
//#include <queue>


class TabuSearch
{
	int iterations = 300'000;
	int* bestPath;
	int bestCost = INT_MAX;
	int* currentPath;
	int currentCost = INT_MAX;
	Matrix* m;


	void setRandomPath(); 
	void setNNPath();
	inline void copyPath(int* from, int* to);
	void swap(int* T, int a, int b);

	//zbior tabu
	int tabuLifespan;
	int** tabuMatrix;
	void deleteTabuMatrix();
	void initTabuMatrix();

	//aspiracja

	//dywersyfikacja
	bool diversification = true;
	int diversificationCounter = 0;
	int diversificationPoint = 300;
	void diversificate();

	//aspiracja
	bool aspiration = true;
	bool aspirationCriterium(int i,int j);

	//zbior sasiadow
	int** moveValue;
	void deleteMoveValue();
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
	TabuSearch(Matrix* matrix, int startPath, int ifDiversification, int ifAspiration);
	~TabuSearch();
};

