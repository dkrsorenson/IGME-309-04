#pragma once
#include <iostream>
using namespace std;

template <class T> class Queue
{
public:
	Queue();
	Queue(const Queue& other);
	Queue& operator=(const Queue& other);
	~Queue();

	void Push(T item);
	void Pop();
	void Print();
	int GetSize();
	bool IsEmpty();

private:
	T* itemArr;
	int itemCount;
	int size;
};

// initialize list with size of 1
template<class T>
Queue<T>::Queue()
{
	size = 1;
	itemArr = new T[size];
	itemCount = 0;
}

// Copy constructor
template<class T>
Queue<T>::Queue(const Queue& other) {
	cout << "Copy constructor called" << endl;

	size = other.size;
	itemCount = other.itemCount;

	if (itemArr != nullptr) {
		delete[] itemArr;
		itemArr = nullptr;
	}

	itemArr = new T[size];

	for (int i = 0; i < other.itemCount; i++) {
		itemArr[i] = other.itemArr[i];
	}
}

// Copy assignment operator
template<class T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
	cout << "Copy assignment operator called" << endl;

	if (this == &other) {
		return *this;
	}

	if (itemArr != nullptr) {
		delete[] itemArr;
		itemArr = nullptr;
	}

	size = other.size;
	itemCount = other.itemCount;

	itemArr = new T[size];

	copy(other.itemArr, other.itemArr + other.itemCount, itemArr);

	return *this;
}

// Clears anything on heap
template<class T>
Queue<T>::~Queue()
{
	delete[] itemArr;
	itemArr = nullptr;
	itemCount = 0;
	size = 0;
}

// Push item to end of array and allocate for more memory when needed 
template<class T>
void Queue<T>::Push(T item)
{
	if (itemCount + 1 >= size) {
		size += 4;
		T* tempArr = new T[size];

		for (int i = 0; i < itemCount; i++)
		{
			tempArr[i] = itemArr[i];
		}

		if (itemArr != nullptr) {
			delete[] itemArr;
			itemArr = nullptr;
		}

		itemArr = new T[size];
		copy(tempArr, tempArr + itemCount, itemArr);

		delete[] tempArr;
		tempArr = nullptr;
	}

	itemArr[itemCount] = item;
	itemCount++;
}

// Remove item from the queue
template<class T>
void Queue<T>::Pop()
{
	if (itemCount > 0) {
		for (int i = 0; i < itemCount - 1; i++)
		{
			itemArr[i] = itemArr[i + 1];
		}
		itemCount--;
	}
	else {
		cout << "No items in list." << endl;
	}
}

// Print the contents of the queue
template<class T>
void Queue<T>::Print()
{
	for (int i = 0; i < itemCount; i++)
	{
		if (i > 0) cout << ", " << itemArr[i];
		else cout << itemArr[i];
	}
}

// Get the size of the queue
template<class T>
int Queue<T>::GetSize()
{
	return itemCount;
}

// Check to see if the queue is empty
template<class T>
bool Queue<T>::IsEmpty()
{
	if (itemCount == 0) return true;

	return false;
}