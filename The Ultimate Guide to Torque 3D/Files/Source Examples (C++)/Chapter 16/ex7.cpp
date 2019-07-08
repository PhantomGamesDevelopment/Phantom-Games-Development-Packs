/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex7.cpp
**/
#include "ex7.h"

void main() {
	Container<int> myNumber(20);
	Container<char> myLetter('q');
	Container<char *> myWord("Hello");

	cout << "Contents Are: " << myNumber.fetch() << ", " << myLetter.fetch() << ", " << myWord.fetch() << endl;
	myNumber.decrement();
	myLetter.increment();
	myWord.increment();
	cout << "Contents Are Now: " << myNumber.fetch() << ", " << myLetter.fetch() << ", " << myWord.fetch() << endl;
}

//Container<class T>
template <class T> T Container<T>::fetch() {
	return element;
}

template <class T> void Container<T>::increment() {
	element++;
}

template <class T> void Container<T>::decrement() {
	element--;
}

//Container<char *>
char *Container<char *>::fetch() {
	return element;
}

void Container<char *>::increment() {
	cout << "Cannot perform ::increment() on char *" << endl;	
}

void Container<char *>::decrement() {
	cout << "Cannot perform ::decrement() on char *" << endl;	
}