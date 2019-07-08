#include "PGD/Solutions/newClassTemplate.h"

myNewClass *mein = NULL;

void myNewClass::create() {
   if(mein == NULL) {
	mein = new myNewClass();
	Con::printf("MAP Module Loaded: My Class");
   }
}

void myNewClass::destroy() {
   if(mein != NULL) {
      delete mein;
	mein = NULL;
	Con::printf("MAP Module Removed: My Class");
   }
}

myNewClass::myNewClass() {
   //init variables here
}

myNewClass::~myNewClass() {

}
