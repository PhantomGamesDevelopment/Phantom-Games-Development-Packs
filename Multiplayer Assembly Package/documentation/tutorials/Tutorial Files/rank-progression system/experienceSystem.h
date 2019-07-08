#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif

#include "PGD/Crypto/cryptoPackage.h"
#include "PGD/Solutions/PGDStore.h"
#include "math/mRandom.h"

#include "console/SimXMLDocument.h"

#include "core/fileObject.h"

#define MAX_LEVEL 7

#ifndef experienceSystem_H
#define experienceSystem_H

class expControl {
   private:
      F32 plEXP;

   public:

      const char *F32ToConstChar(F32 in);
	   const char *intToConstChar(int in);

      static const F32 expTable[MAX_LEVEL];

      static void create();
	   static void destroy();
      
      expControl();
	   ~expControl();

	   void addEXP(F32 amount);
	   F32 getEXP();
	   F32 getNeededEXP();
	   int getLevel();

	   //Control Functions
	   void saveInfo(const char *guid, const char *file);
	   void loadInfo(const char *file);
};
#endif

extern expControl *experience;
