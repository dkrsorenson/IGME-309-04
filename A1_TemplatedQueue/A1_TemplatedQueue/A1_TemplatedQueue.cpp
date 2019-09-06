// A1_TemplatedQueue.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Dakota Sorenson, DSA2 Section 4

#include <iostream>
#include <string>
#include "Queue.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

using namespace std;

void QueueExample();

int main()
{
	// Display the queue example to show it working
	QueueExample();

	// Display memory leak report
	_CrtDumpMemoryLeaks();

	return 0;
}

void QueueExample() {
	// Create new list of ints
	Queue<int>* items1 = new Queue<int>();

	// Check if queue is empty 
	cout << "Items 1 Queue Created." << endl;
	string empty = (items1->IsEmpty()) ? "True" : "False";
	cout << "Is Items 1 empty? " << empty << "\n" << endl;

	// Push items to queue
	cout << "Pushed value '1' to Items 1." << endl;
	items1->Push(1);
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	cout << "Pushed value '100' to Items 1." << endl;
	items1->Push(100);
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	cout << "Pushed value '30' to Items 1." << endl;
	items1->Push(30);
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	cout << "Pushed value '15' to Items 1:" << endl;
	items1->Push(15);
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	cout << "Pushed value '77' to Items 1:" << endl;
	items1->Push(77);
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	// Print queue size
	cout << "Size of Items 1: " << items1->GetSize() << endl;
	empty = (items1->IsEmpty()) ? "True" : "False";
	cout << "Is Items 1 empty? " << empty << endl << endl;

	// Pop items from list
	cout << "Popped an item from Items 1." << endl;
	items1->Pop();
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	cout << "Popped an item from Items 1." << endl;
	items1->Pop();
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl << endl;

	// Print size of queue again
	cout << "Size of Items 1: " << items1->GetSize() << endl;
	empty = (items1->IsEmpty()) ? "True" : "False";
	cout << "Is Items 1 empty? " << empty << endl;

	// Create items 2 and use copy constructor
	cout << endl;
	Queue<int>* items2 = new Queue<int>(*items1);
	cout << "Created Items 2 queue using Items 1." << endl;
	cout << "Items 2 Values: ";
	items2->Print();
	cout << endl;

	// Add something to items 1, print items 1 and 2 to show diff
	cout << endl;
	cout << "Pushed value '45' to Items 1:" << endl;
	items1->Push(45);
	cout << "Items 1 Values: ";
	items1->Print();
	cout << endl;
	cout << "Items 2 Values: ";
	items2->Print();
	cout << endl;

	// Create items 3 and call copy constructor
	cout << endl;
	Queue<int> items3 = *items1;
	cout << "Created Items 3 queue using Items 1." << endl;
	cout << "Items 3 Values: ";
	items3.Print();
	cout << endl;

	// Set items 3 to items 2 to call copy assignment constructor
	cout << endl;
	items3 = *items2;
	cout << "Change Items 3 assignment to Items 2." << endl;
	cout << "Items 3 Values: ";
	items3.Print();
	cout << endl;

	// Clean memory is happy memory
	delete items2;
	delete items1;
}