/**
The Ultimate Guide To Torque 3D
Chapter 14
By: Robert C Fritzen
ex48.cpp
**/
#include <iostream>
#include <list>
#include <stdlib.h> 
#include <time.h>
using namespace std;

struct Volume {
	double len, wid, hei;
	char name[32];

	Volume(int index) {
		len = 1.0 + ((double)rand() / RAND_MAX) * (25.0 - 1.0);
		wid = 1.0 + ((double)rand() / RAND_MAX) * (25.0 - 1.0);
		hei = 1.0 + ((double)rand() / RAND_MAX) * (25.0 - 1.0);
		sprintf(name, "Volume%i", index+1); 
	}

	double fetchV() { return len * wid * hei; }
	char *fetchN() { return name; }
};

void main() {
	srand(time(0));
	Volume *v;
	std::list<Volume*> list1, list2, list3;
	for(int i = 0; i < 10; i++) {		
		v = new Volume(i);
		list1.push_back(v);
		cout << "Created new volume: " << v->fetchN() << ", " << v->fetchV() << endl;
	}
	//Thresholds
	double threshold = 500.0, total = 0.0;
	for(std::list<Volume*>::iterator it = list1.begin(); it != list1.end(); it++) {
		v = *it;
		if(v->fetchV() >= threshold) {
			list2.push_back(v);
		}
		else {
			list3.push_back(v);
		}
		total += v->fetchV();
	}
	cout << "Volumes Above Threshold (" << threshold << "):" << endl;
   for(std::list<Volume*>::iterator it = list2.begin(); it != list2.end(); it++) {
		v = *it;
		cout << v->fetchN() << " at " << v->fetchV() << endl;
	}
	cout << "Volumes Below Threshold (" << threshold << "):" << endl;
   for(std::list<Volume*>::iterator it = list3.begin(); it != list3.end(); it++) {
		v = *it;
		cout << v->fetchN() << " at " << v->fetchV() << endl;
	}
	cout << "Total Volume: " << total << endl;
}