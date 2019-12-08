// TSP_heuristic_algorithms.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <conio.h>
#include "SimulatedAnnealing.h"
#include "TabuSearch.h"

using namespace std;

void menu();

void simulatedAnnealing(Matrix* matrix, int neighbourType = 0, int temperatureType = 0);
void tabuSearch(Matrix* matrix, int startPath);
void SA_neighbourChoice(Matrix* matrix);
void SA_temperatureChoice(Matrix* matrix, int neighbourType = 0);
void TS_startPathChoice(Matrix* matrix);
void TS_diversificationChoice(Matrix* matrix, int startPath);
void TS_aspirationChoice(Matrix* matrix, int startPath, int ifDiversification);

int main()
{
	menu();
}

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
		matrix->show();
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

void tabuSearch(Matrix * matrix, int startPath,int ifDiversification,int ifAspiration)
{
	Timing timer;
	TabuSearch ts(matrix, startPath,ifDiversification, ifAspiration);
	timer.startCount();
	ts.algorithm(1000000);
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
			"\n3.invert" <<
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
			tabuSearch(matrix,startPath, ifDiversification, 0);
			break;
		case '2':
			tabuSearch(matrix, startPath, ifDiversification, 1);
			break;
		}
	} while (h != 27);
}


