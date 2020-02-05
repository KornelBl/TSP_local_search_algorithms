#pragma once
#include <string>
struct element {
	int i;
	int j;
	element* next;
	element* prev;
	element(int i, int j) {
		next = NULL;
		prev = NULL;
		this->i = i;
		this->j = j;
	}
};

class List
{
private:
	element * top;
	element * bottom;
	int count;
public:
	List();
	~List();
	int getSize();
	void addTop(int,int);					// dodaj na pocz�tek
	void addBottom(int,int);				// dodaj na koniec
	void deleteTop();					// usun pocz�tek
	void deleteBottom();				// usun koniec
	element* findPtr(int,int);				// znajdz element, zwroc wskaznik, zwraca null gdy nie znaleziono
	void show();						// wypisz liste
}
;