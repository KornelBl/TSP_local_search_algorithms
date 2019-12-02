// TSP_heuristic_algorithms.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <conio.h>




#include "SimulatedAnnealing.h"

using namespace std;

void menu();

void simulatedAnnealing(Matrix* matrix, int neighbourType = 0);
void SAmenu(Matrix* matrix);


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
			"\n2.Dynamic Programming" <<
			"\n3.Branch and Bound" <<
			"\n4.Zmien graf" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			SAmenu(matrix);
			break;
		case '2':
			break;
		case '3':
			break;
		case '4':
			std::cout << "Podaj nazwe pliku\n";
			std::cin >> filename;
			matrix->readFromFile(filename);
			break;
		}
	} while (h != 27);
	system("cls");
	delete matrix;
}


void simulatedAnnealing(Matrix* matrix,int neighbourType)
{
	Timing timer;
	SimulatedAnnealing sa(matrix,neighbourType);
	timer.startCount();
	sa.algorithm();
	timer.endCount();
	sa.showResult();
	cout << endl << "Czas: " << timer.getResult() / 1000 << " ms";
	_getch();
}

void SAmenu(Matrix * matrix)
{
	char h;
	do {
		system("cls");
		cout << matrix->name << endl;
		matrix->show();
		cout << "SIMULATED ANNEALING " <<
			"\n1.swap" <<
			"\n2.insert" <<
			"\n3.invert" <<
			"\n[Esc] Koniec\n";
		fflush(stdin);
		h = _getch();
		switch (h) {
		case '1':
			simulatedAnnealing(matrix, 0);
			break;
		case '2':
			simulatedAnnealing(matrix, 1);
			break;
		case '3':
			simulatedAnnealing(matrix, 2);
			break;
		}
	} while (h != 27);
}


