/**
The Ultimate Guide To Torque 3D
Chapter 16
By: Robert C Fritzen
ex2.h
**/
#ifndef _MYFILE_H_
#define _MYFILE_H_

	class myClass {
		public:
			myClass();
			~myClass();
			void set(int, int, int);
			void fetch(int *, int *, int *);

		private:
			int num1;
			int num2;
			int num3;
	};

#endif //_MYFILE_H_