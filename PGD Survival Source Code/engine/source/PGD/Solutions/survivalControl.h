/**
survivalControl.h/.cpp 
Robert Fritzen
PGD Survival
(c) Phantom Games Development, 2012
**/
#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif
#include "PGD/Crypto/cryptoPackage.h"
#include "PGD/Solutions/PGDStore.h"
#include "math/mRandom.h"

#include "console/SimXMLDocument.h"

#include "core/fileObject.h"

#define MAX_LEVEL 50

#ifndef survivalControl_H
#define survivalControl_H

class survivalControl {
   private:
		F32 EXPMulti;
		const char *SurvivalEMKey();
		bool certVerify(std::string message, std::string signature);

   public:

      const char *F32ToConstChar(F32 in);
		const char *intToConstChar(int in);
		const char *boolToConstChar(bool in);

      static const F32 WeaponEXP[MAX_LEVEL];

      static void create();
	   static void destroy();
      
      survivalControl();
	   ~survivalControl();

		void dumpAll();

		//Weapons
		struct weaponInfo {
		   public:
				String GunName;
				String GunSpec;
				F32 kills;
				F32 headshots;
				F32 exp;

				int id;
				int unlocked; //0 or 1

				weaponInfo *next;

				weaponInfo(const char *n, int ident);
		};

      //Challenges
		struct challenge;

		struct challengeTree {
		   public:
			   int id;                      //stored ID #
		      String name;                 //name of the tree (category)
			   challengeTree *next;         //next tree in the list
				challenge *headChallenge;    //first challenge item in the tree

				challengeTree(const char *n, int ident);
		};
		struct challenge {
		   public:
		      challengeTree *head;         //the tree this challenge belongs to
			   int id;                      //stored id#
			   String name;                 //name of the challenge
			   String desc;                 //description (or task) of the challenge
			   String goal;                 //Torque String of completion IE: "17/25"
				String reward;
			   int completed;              //boolean flag of completion

				challenge *next;             //next challenge in this tree

				challenge(const char *n, const char *d, const char *g, const char *r, int ident, challengeTree *tree);
		};

		challengeTree *tier;
		weaponInfo *topWeapon;

		//Challenge Functions
		void addChallengeTree(const char *n);
		void addChallenge(const char *n, const char *d, const char *g, const char *r, challengeTree *mainHead);
		void flagCompleted(challenge *c);
		void updateProgress(challenge *c, String progress);

		challengeTree *getChallengeTree(int ident);
		//getChallenge decs.
		challenge *getChallenge(challengeTree *tree, const char *n);
		challenge *getChallenge(challengeTree *tree, int ident);

		//Weapon Functions
		void addWeapon(const char *n);
		void unlockWeapon(weaponInfo *gun);
		//getWeapon decs.
		weaponInfo *getWeapon(const char *n);
		weaponInfo *getWeapon(int ident);

		void updateWeaponDetails(weaponInfo *weapon, const char *newSpec, F32 newKills, F32 newHeadshots);
		void addWeaponEXP(weaponInfo *weapon, F32 amount, bool absolute);
		F32 getWeaponEXP(weaponInfo *weapon);
		F32 getNeededEXP(weaponInfo *weapon);
		int getWeaponLevel(weaponInfo *weapon);

		F32 getEXPMultiplier();
		void setEXPMultiplier(F32 amount, const char *rsaSig);
		//Control Functions
		void saveInfo(const char *guid, const char *file);
		void loadInfo(const char *file);
};
#endif

extern survivalControl *survive;