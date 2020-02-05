#pragma once
#include <ctime>
#include "List.h"
#include <string>
//#include <queue>


class TabuSearch
{
	int iterations = 10'000;
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
	List tabuList;


	void deleteTabuMatrix();
	void deleteTabuList();
	void addTabuMatrix(int i, int j);
	bool isInTabuMatrix(int i, int j);
	void initTabuMatrix();
	void initTabuList();
	void addTabuList(int i, int j);
	bool isInTabuList(int i, int j);
	void (TabuSearch::*initTabu)();
	void (TabuSearch::*deleteTabu)();
	void (TabuSearch::*addTabu)(int i, int j);
	bool (TabuSearch::*isInTabu)(int i, int j);


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
	void initMoveValues();

	void renewMoveValues_swap(int i,int j);

	void renewMoveValues_symetricInvert(int i, int j);

	void renewMoveValues_insert(int i, int j);



	//neighbourhood functions
	bool asymetricNeighbourhood = false;

	void chooseNeighbour_swap(int i, int j);
	int calculateNeighbourValue_swap(int i, int j);

	void chooseNeighbour_invert(int i, int j);
	int calculateNeighbourValue_symetricInvert(int i, int j);

	void chooseNeighbour_insert(int i, int j);
	int calculateNeighbourValue_insert(int i, int j);

	void (TabuSearch::*chooseNeighbour)(int, int);
	int (TabuSearch::*calculateNeighbourValue)(int, int);
	void (TabuSearch::*renewMoveValues)(int, int);


	//typedef void(TabuSearch::*NeighbourFunction)(int, int);
	//NeighbourFunction chooseNeighbour, calculateNeighbourCost;



public:
	int getBestCost();
	void algorithm(double time = 0);
	void showResult();
	TabuSearch(Matrix* matrix, int startPath, int ifDiversification, int ifAspiration, int neighbourType, int tabuContainer, int lifespan, int iterations, int criticalPoint);
	~TabuSearch();
};

