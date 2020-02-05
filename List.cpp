#include "pch.h"
#include "List.h"

#include "List.h"
#include <iostream>
#include <conio.h>
#include <fstream>

List::List()
{
	count = 0;
	top = bottom = NULL;
}


List::~List()
{
	while (top != NULL) {
		deleteTop();
	}
}

int List::getSize()
{
	return count;
}

void List::addTop(int i, int j)
{
	count++;
	element* ele = new element(i,j);
	ele->next = top;
	if (top != NULL)top->prev = ele;
	else bottom = ele;
	top = ele;
}

void List::addBottom(int i, int j)
{
	count++;
	element* ele = new element(i,j);
	ele->next = bottom;
	if (top != NULL)bottom->next = ele;
	else top = ele;
	bottom = ele;
}


void List::deleteTop()
{
	count--;
	if (top == NULL)return;
	element* temp = top->next;
	delete top;
	if (temp != NULL)
		temp->prev = NULL;
	top = temp;
}

void List::deleteBottom()
{
	count--;
	if (bottom == NULL)return;
	element* temp = bottom->prev;
	delete bottom;
	if (temp != NULL)
		temp->next = NULL;
	bottom = temp;
}


element * List::findPtr(int i, int j)
{
	element* temp = bottom;
	while (temp != NULL) {
		if (temp->i == i && temp->j == j)return temp;
		temp = temp->prev;
	}
	return NULL;
}


void List::show()
{
	element* temp;
	temp = top;
	while (temp != NULL) {
		std::cout << temp->i << ", " << temp->j<<" ; ";
		temp = temp->next;
	}
	std::cout << "\n";
}