// TSP_heuristic_algorithms.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <conio.h>
#include "SimulatedAnnealing.h"
#include "TabuSearch.h"
#include "fstream"

using namespace std;

//void research();
void menu();

void simulatedAnnealing(Matrix* matrix, int neighbourType = 0, int temperatureType = 0);
void tabuSearch(Matrix* matrix, int startPath, int ifDiversification, int ifAspiration, int neighbourType,int tabuContainer);
void SA_neighbourChoice(Matrix* matrix);
void SA_temperatureChoice(Matrix* matrix, int neighbourType = 0);
void TS_startPathChoice(Matrix* matrix);
void TS_diversificationChoice(Matrix* matrix, int startPath);
void TS_aspirationChoice(Matrix* matrix, int startPath, int ifDiversification);
void TS_neighbourChoice(Matrix* matrix, int startPath, int ifDiversification, int ifAspiration);
void TS_tabuContainerChoice(Matrix* matrix, int startPath, int ifDiversification, int ifAspiration, int neighbourType);



int main()
{
	srand(time(NULL));
	menu();
}

//void research() {
//	std::string fileName = "DP22.csv";
//	Matrix* matrix = new Matrix;
//	ofstream file;
//	double result,timeResult;
//	Timing counter;
//	string data[8] = {"17","34","45","56","70","100","171","323"};
//	
//	//sasiedzi SA
//	int neighbourType = 1;
//	int temperatureType = 0;
//	int startPath = 0;
//	int tempIt = 500;
//	int inTempIt = 1100;
//	
//	neighbourType = 0;
//	startPath = 0; //---------------------------------zmienic po div spowrotem na 1
//	int tabuIterations = 1'000'000;
//	int ifDiversification = 0;
//	int ifAspiration = 0;
//	int tabuContainer = 0;
//	fileName = "TS_liczenie.csv";
//
//
//	//TabuSearch ts(matrix, startPath, ifDiversification, ifAspiration, neighbourType, tabuContainer,
//	//	matrix->vertices * 3, tabuIterations, matrix->vertices * 3 * 5);
//	////mierzony czas
//	//ts.algorithm();
//
//
//
//	//file.open(fileName, ios_base::out);
//	//for (int i = 0; i < 8; i++) {
//	//	matrix->readFromFile("data" + data[i] + ".txt");
//	//	result = 0;
//	//	timeResult = 0;
//	//	neighbourType = 1;
//	//	for (int j = 0; j < 20; j++) {
//	//		SimulatedAnnealing sa(matrix, neighbourType, temperatureType, startPath,tempIt,inTempIt);
//	//		counter.startCount();
//	//		//mierzony czas
//	//		sa.algorithm();
//	//		counter.endCount();
//	//		result += sa.getBestCost();
//	//		timeResult += counter.getResult() / 20;
//	//		std::cout << i << ", " << j << endl;
//	//	}
//	//	file << data[i] << ";" << startPath << ";" << result / 20 << ";" << timeResult << ";";
//
//	//	result = 0;
//	//	timeResult = 0;
//	//	neighbourType = 0;
//	//	for (int j = 0; j < 20; j++) {
//	//		TabuSearch ts(matrix, startPath, ifDiversification, ifAspiration, neighbourType, tabuContainer,
//	//			matrix->vertices * 3, tabuIterations, matrix->vertices * 3 * 5);
//	//		counter.startCount();
//	//		//mierzony czas
//	//		ts.algorithm(10'000'000);
//	//		counter.endCount();
//	//		result += ts.getBestCost();
//	//		timeResult += counter.getResult() / 20;
//	//		std::cout << i << ", " << j << endl;
//	//	}
//	//	file << result / 20 << ";" << timeResult << endl;
//	//}
//	//file.close();
//
//
//
//
//
//
//
//
//	int cadencies[5] = {3,5,7,9,11};
//
//	file.open(fileName, ios_base::out);
//	for (int i = 0; i < 8; i++) {
//		matrix->readFromFile("data" + data[i] + ".txt");
//		file << data[i] << ";";
//		for (int startPath = 1; startPath < 2; startPath++) {
//			result = 0;
//			timeResult = 0;
//			for (int j = 0; j < 20; j++) {
//				TabuSearch ts(matrix, startPath, ifDiversification, ifAspiration, neighbourType, tabuContainer,
//					matrix->vertices * 3, tabuIterations,matrix->vertices * 3 * 9);
//				counter.startCount();
//				//mierzony czas
//				ts.algorithm(5'000'000);
//				counter.endCount();
//				result += ts.getBestCost();
//				timeResult += counter.getResult() / 20;
//				std::cout << i << ", " << j << endl;
//			}
//			file << startPath << ";" << result / 20 << ";" << timeResult << ";";
//		}
//		file << endl;
//	}
//	file.close(); 
//
//
//	//file.open(fileName, ios_base::out);
//	//for (int i = 0; i < 8; i++) {
//	//	matrix->readFromFile("data" + data[i] + ".txt");
//	//	for (startPath = 0; startPath < 2; startPath++) {
//	//		result = 0;
//	//		timeResult = 0;
//	//		for (int j = 0; j < 20; j++) {
//	//			SimulatedAnnealing sa(matrix, neighbourType, temperatureType, startPath,tempIt,inTempIt);
//	//			counter.startCount();
//	//			//mierzony czas
//	//			sa.algorithm();
//	//			counter.endCount();
//	//			result += sa.getBestCost();
//	//			timeResult += counter.getResult() / 20;
//	//			std::cout << i << ", " << j << endl;
//	//		}
//	//		file << data[i] << ";" << startPath << ";" << result / 20 << ";" << timeResult << endl;
//	//	}
//	//}
//	//file.close();
//	delete matrix;
//}

void menu() {
	char h;
	Matrix* matrix = new Matrix;
	std::string filename;
	std::cout << "Podaj nazwe pliku\n";
	std::cin >> filename;
	matrix->readFromFile(filename);
	do {
		system("cls");
		cout << matrix->name << endl;
		//matrix->show();
		cout << "\n1.Symulowane wyżarzanie" <<
			"\n2.Tabu Search" <<
			"\n3.Zmien graf" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			SA_neighbourChoice(matrix);
			break;
		case '2':
			TS_startPathChoice(matrix);
			break;
		case '3':
			std::cout << "Podaj nazwe pliku\n";
			std::cin >> filename;
			matrix->readFromFile(filename);
			break;
		}
	} while (h != 27);
	system("cls");
	delete matrix;
}


void simulatedAnnealing(Matrix* matrix,int neighbourType, int temperatureType)
{
	Timing timer;
	SimulatedAnnealing sa(matrix,neighbourType,temperatureType);
	timer.startCount();
	sa.algorithm();
	timer.endCount();
	sa.showResult();
	cout << endl << "Czas: " << timer.getResult() / 1000 << " ms";
	_getch();
}

void tabuSearch(Matrix * matrix, int startPath,int ifDiversification,int ifAspiration,int neighbourType, int tabuContainer)
{
	Timing timer;
	float seconds = 0;
	TabuSearch ts(matrix, startPath,ifDiversification, ifAspiration, neighbourType, tabuContainer,matrix->vertices * 3,50'000,matrix->vertices * 20);
	cout << "ile sekund?";
	cin >> seconds;
	timer.startCount();
	ts.algorithm(1000000 * seconds);
	timer.endCount();
	ts.showResult();
	cout << endl << "Czas: " << timer.getResult() / 1000 << " ms";
	_getch();
}

void SA_neighbourChoice(Matrix * matrix)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "SIMULATED ANNEALING " <<
			"\n1.swap" <<
			"\n2.insert" <<
			"\n3.invert asymetric" <<
			"\n4.invert symetric" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			SA_temperatureChoice(matrix, 0);
			break;
		case '2':
			SA_temperatureChoice(matrix, 1);
			break;
		case '3':
			SA_temperatureChoice(matrix, 2);
			break;
		case '4':
			SA_temperatureChoice(matrix, 3);
			break;
		}
	} while (h != 27);
}

void SA_temperatureChoice(Matrix * matrix, int neighbourType)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "SIMULATED ANNEALING - temperature" <<
			"\n1.geometryczna" <<
			"\n2.liniowa" <<
			"\n3.logarytmiczna" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			simulatedAnnealing(matrix, neighbourType, 0);
			break;
		case '2':
			simulatedAnnealing(matrix, neighbourType, 1);
			break;
		case '3':
			simulatedAnnealing(matrix, neighbourType, 2);
			break;
		}
	} while (h != 27);
}

void TS_startPathChoice(Matrix * matrix)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "TABU SEARCH - starting path" <<
			"\n1.random" <<
			"\n2.Nearest Neighbour" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			TS_diversificationChoice(matrix, 0);
			break;
		case '2':
			TS_diversificationChoice(matrix, 1);
			break;
		}
	} while (h != 27);
}

void TS_diversificationChoice(Matrix * matrix, int startPath)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "TABU SEARCH - dywersyfikacja?" <<
			"\n1.Tak" <<
			"\n2.Nie" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			TS_aspirationChoice(matrix,startPath, 0);
			break;
		case '2':
			TS_aspirationChoice(matrix,startPath, 1);
			break;
		}
	} while (h != 27);
}

void TS_aspirationChoice(Matrix * matrix, int startPath, int ifDiversification)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "TABU SEARCH - Aspiracja?" <<
			"\n1.Tak" <<
			"\n2.Nie" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			TS_neighbourChoice(matrix,startPath, ifDiversification, 0);
			break;
		case '2':
			TS_neighbourChoice(matrix, startPath, ifDiversification, 1);
			break;
		}
	} while (h != 27);
}

void TS_neighbourChoice(Matrix * matrix, int startPath, int ifDiversification, int ifAspiration)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "TABU SEARCH - rodzaj sasiedztwa" <<
			"\n1.swap" <<
			"\n2.insert" <<
			"\n3.symetric invert" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			TS_tabuContainerChoice(matrix, startPath, ifDiversification, ifAspiration, 0);
			break;
		case '2':
			TS_tabuContainerChoice(matrix, startPath, ifDiversification, ifAspiration, 1);
			break;
		case '3':
			TS_tabuContainerChoice(matrix, startPath, ifDiversification, ifAspiration, 2);
			break;
		}
	} while (h != 27);

}

void TS_tabuContainerChoice(Matrix * matrix, int startPath, int ifDiversification, int ifAspiration, int neighbourType)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		cout << "TABU SEARCH - rodzaj kontenera tabu" <<
			"\n1.Matrix" <<
			"\n2.List" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			tabuSearch(matrix, startPath, ifDiversification, ifAspiration, neighbourType, 0);
			break;
		case '2':
			tabuSearch(matrix, startPath, ifDiversification, ifAspiration, neighbourType, 1);
			break;
		}
	} while (h != 27);

}


