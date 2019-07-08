#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif
#include "math/mRandom.h"

#include "console/SimXMLDocument.h"

#include "core/fileObject.h"

#ifndef statsContainer_H
#define statsContainer_H

class statsContainer {
   private:

   public:
      const char *F32ToConstChar(F32 in);
	  const char *intToConstChar(int in);

      static void create();
	  static void destroy();
      
      statsContainer();
	  ~statsContainer();

   	  struct singleStat {
	     public:
		    String statName;
			F32 statValue;
			int id;

			singleStat *next;
			singleStat(const char *n, int ident);
      };
	  singleStat *topStat;

	  void addStat(const char *n);
	  singleStat *getStat(const char *n);
	  singleStat *getStat(int ident);

	  void addToStat(singleStat *stat, F32 amount);
	  String getStatName(singleStat *stat);
	  F32 getStatValue(singleStat *stat);

	  //Control Functions
	  void saveInfo(const char *guid, const char *file);
	  void loadInfo(const char *file);
};
#endif

extern statsContainer *stats;