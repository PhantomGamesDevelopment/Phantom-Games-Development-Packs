/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex6.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	template <class T> class Container {
		public:
			Container(T in) : element(in) { }

			T fetch() { return element; }
			void increment() { element++; }
			void decrement() { element--; }
		private:
			T element;
	};
	template <> class Container<char *> {
		public:
			Container(char *in) : element(in) { }

			char *fetch() { return element; }
			void increment() { printf("Cannot use increment on char *\n"); }
			void decrement() { printf("Cannot use decrement on char *\n"); }
		private:
			char *element;
	};

#endif //_MYFILE_H_