/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex7.h
**/
#include <iostream>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	template <class T> class Container {
		public:
			Container(T in) : element(in) { }

			T fetch();
			void increment();
			void decrement();
		private:
			T element;
	};
	template <> class Container<char *> {
		public:
			Container(char *in) : element(in) { }

			char *fetch();
			void increment();
			void decrement();
		private:
			char *element;
	};

#endif //_MYFILE_H_