/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex11.h
**/
#include <iostream>
#include <cstdarg>
using namespace std;

#ifndef _MYFILE_H_
#define _MYFILE_H_

	class Printer {
		public:
			void print(const char *fmt, ...);
	};

	class Poly : public Printer {
		public:
			Poly() : width(1), height(1) { }
			Poly(int w, int h) : width(w), height(h) { }

			int fetchArea();
			void out();
		protected:
			int width;
			int height;
	};

#endif //_MYFILE_H_