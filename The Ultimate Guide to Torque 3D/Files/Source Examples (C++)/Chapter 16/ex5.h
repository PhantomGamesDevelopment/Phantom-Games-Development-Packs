/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex5.h
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

#endif //_MYFILE_H_