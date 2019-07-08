/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex12.h
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

	class Poly {
		public:
			Poly() : width(1), height(1) { }
			Poly(int w, int h) : width(w), height(h) { }

			int fetchArea();
		protected:
			int width;
			int height;
	};

	class Triangle : public Poly, public Printer {
		public:
			Triangle() : Poly() { }
			Triangle(int w, int h) : Poly(w, h) { }

			int fetchArea();
			void out();
	};

#endif //_MYFILE_H_