/**
survivalControl.h/.cpp 
Robert Fritzen
PGD Survival
(c) Phantom Games Development, 2012
**/
#include "PGD/Solutions/survivalControl.h"

#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

using namespace std;

survivalControl *survive = NULL;

const F32 survivalControl::WeaponEXP[MAX_LEVEL] = {0, 50, 100, 150, 200, 250, 300, 400, 550, 700
	             , 850, 1000, 1250, 1500, 1750, 2000, 2250, 2500
					 , 2750, 3000, 3500, 4000, 4500, 5000, 5500, 6000
					 , 6500, 7000, 7750, 8500, 9250, 10000, 11000, 12500
	             , 14000, 16000, 18000, 20000, 22500, 25000, 27500, 30000
	             , 32500, 35000, 37500, 40000, 42500, 45000, 47500, 50000};

const char *survivalControl::SurvivalEMKey() {
   return "Key Omitted For Obvious Reasons";
}

bool survivalControl::certVerify(std::string message, std::string signature) {
   std::string dec, fin;
   fin.assign(SurvivalEMKey());
   fin.append("h");
   Integer rsaPub(fin.c_str()), rsaexp("65537");
   
   std::string holder;

   try {
      RSASSA_PKCS1v15_SHA_Verifier verifier;
      verifier.AccessKey().Initialize(rsaPub, rsaexp);

      cryptoPackage->HexDecode(signature, holder);

      StringSource(message+holder, true,
          new SignatureVerificationFilter(
              verifier, NULL,
              SignatureVerificationFilter::THROW_EXCEPTION
         ) // SignatureVerificationFilter
      ); // StringSource
   }
   catch(CryptoPP::Exception e) {
      return false;
   }

   return true;
}

//ASSET FUNCTIONS
const char *survivalControl::F32ToConstChar(F32 in) {
   char output[32];
	dSprintf(output, 32, "%.0f", in);

	return output;
}

const char *survivalControl::intToConstChar(int in) {
   char output[sizeof(int)];
	dSprintf(output, sizeof(output), "%i", in);

	return output;
}

const char *survivalControl::boolToConstChar(bool in) {
	if(in) {
	   return "1";
	}
	else {
	   return "0";
	}
}
//END

survivalControl::survivalControl() {
   tier = NULL;
	topWeapon = NULL;

	EXPMulti = 1.0f;
}

survivalControl::~survivalControl() {
}

void survivalControl::create() {
   if(survive == NULL) {
      survive = new survivalControl();
		Con::printf("survival control created... awaiting instructions");
   }
}

void survivalControl::destroy() {
   if(survive != NULL) {
      delete survive;
	   survive = NULL;
   }
}

survivalControl::weaponInfo::weaponInfo(const char *n, int ident) {
   GunName = n;
	GunSpec = "none";
	kills = 0;
	headshots = 0;
	exp = 0;
	id = ident;
	unlocked = 0;
	next = NULL;
}

survivalControl::challenge::challenge(const char *n, const char *d, const char *g, const char *r, int ident, challengeTree *tree) {
   head = tree;
	name = n;
	desc = d;
	goal = g; //Phantom: The 'goal' field is sort of obsolete now, however certain challenges will use this to store info.
	reward = r;
	id = ident;
	completed = 0;
	next = NULL;
}

survivalControl::challengeTree::challengeTree(const char *n, int ident) {
   name = n;
   id = ident;
	next = NULL;
	headChallenge = NULL;
}

void survivalControl::addChallengeTree(const char *n) {
	//Con::printf("addChallengeTree(%s)", n);
	if(tier == NULL) {
	   tier = new challengeTree(n, 0);
	}
	else {
		int index = 0;
	   challengeTree *temp = tier;
		while(temp->next != NULL) {
		   temp = temp->next;
			index++;
		}
		//create 2nd one
		challengeTree *ptr = new challengeTree(n, index+1);
		temp->next = ptr;
	}
}

void survivalControl::addChallenge(const char *n, const char *d, const char *g, const char * r, challengeTree *mainHead) {
	if(mainHead == NULL) {
		Con::errorf("Error: Attempting to add challenge to non-existant tree, breaking");
		return;
	}
	//
	if(mainHead->headChallenge == NULL) {
		mainHead->headChallenge = new challenge(n, d, g, r, 0, mainHead);
	}
	else {
	   int index = 0;
		challenge *temp = mainHead->headChallenge;
		while(temp->next != NULL) {
		   temp = temp->next;
			index++;
		}
		challenge *ptr = new challenge(n, d, g, r, index+1, mainHead);
		temp->next = ptr;
	}
}

survivalControl::challengeTree *survivalControl::getChallengeTree(int index) {
   challengeTree *top = tier;
	bool done = false;
	while(!done) {
		if(top->id == index) {
		   return top;
		}
      top = top->next;	
		if(top == NULL) {
		   done = true;
		}
	}
	return NULL;
}

survivalControl::challenge *survivalControl::getChallenge(survivalControl::challengeTree *tree, const char *n) {
	if(tree == NULL) {
		Con::errorf("Error: Attempting to get challenge from non-existant tree, breaking");
		return NULL;
	}
	bool done = false;
	challenge *top = tree->headChallenge;
	while(!done) {
		if(strcmp(n, top->name) == 0) {
		   return top;
		}
		top = top->next;
		if(top == NULL) {
		   done = true;
		}
	}
   return NULL;
}

survivalControl::challenge *survivalControl::getChallenge(survivalControl::challengeTree *tree, int ident) {
	if(tree == NULL) {
		Con::errorf("Error: Attempting to get challenge from non-existant tree, breaking");
		return NULL;
	}
	bool done = false;
	challenge *top = tree->headChallenge;
	while(!done) {
		if(ident == top->id) {
		   return top;
		}
		top = top->next;
	   if(top == NULL) {
		   done = true;
		}
	}
   return NULL;
}

void survivalControl::flagCompleted(challenge *c) {
	if(!c->completed) {
	   c->completed = 1;
		Con::evaluatef("onChallengeCompleted(%i, %i, \"%s\");", c->head->id, c->id, c->reward.c_str());
	}
}

void survivalControl::updateProgress(challenge *c, String progress) {
	c->goal = progress;
}

void survivalControl::addWeapon(const char *n) {
	//Con::printf("addWeapon(%s)", n);
	if(topWeapon == NULL) {
		topWeapon = new weaponInfo(n, 0);
	}
	else {
	   int index = 0;
		weaponInfo *temp = topWeapon;
		while(temp->next != NULL) {
		   temp = temp->next;
			index++;
		}
		weaponInfo *ptr = new weaponInfo(n, index+1);
		temp->next = ptr;
	}
}

survivalControl::weaponInfo *survivalControl::getWeapon(const char *n) {
   weaponInfo *weapon = topWeapon;
	bool done = false;
	while(!done) {
		if(strcmp(weapon->GunName, n) == 0) {
		   return weapon;
		}
	   weapon = weapon->next;
		if(weapon == NULL) {
		   done = true;
		}
	}
	return NULL;
}

survivalControl::weaponInfo *survivalControl::getWeapon(int ident) {
   weaponInfo *weapon = topWeapon;
	bool done = false;
	while(!done) {
		if(weapon->id == ident) {
		   return weapon;
		}
	   weapon = weapon->next;
		if(weapon == NULL) {
		   done = true;
		}
	}
	return NULL;
}

void survivalControl::updateWeaponDetails(weaponInfo *weapon, const char *newSpec, F32 newKills, F32 newHeadshots) {
   weapon->GunSpec = newSpec;
	weapon->kills = newKills;
	weapon->headshots = newHeadshots;
}

F32 survivalControl::getEXPMultiplier() {
   return EXPMulti;
}

void survivalControl::setEXPMultiplier(F32 amount, const char *rsaSig) {
	//verify the signature
	char number[32];
	dSprintf(number, sizeof(number), "%.1f", amount);
	std::string weWant = "PGDEXP";
	weWant.append(number);
	std::string valid = cryptoPackage->pgdHash(weWant);
	if(certVerify(valid, string(rsaSig))) {
		Con::printf("* PGD: EXP Multiplier Set to: %f", amount);
		EXPMulti = amount;
	}
	else {
		Con::errorf("*** EXP Multiplier Set Failed, Certificate Verification Denied.");
	}
}

F32 survivalControl::getWeaponEXP(weaponInfo *weapon) {
   return weapon->exp;
}

F32 survivalControl::getNeededEXP(weaponInfo *weapon) {
   int level = getWeaponLevel(weapon);
	if(level >= 50) {
	   return getWeaponEXP(weapon);
	}
   return WeaponEXP[level];
}

int survivalControl::getWeaponLevel(weaponInfo *weapon) {
   F32 exp = getWeaponEXP(weapon);
	//special cases
	if(exp >= WeaponEXP[49]) {
	   return 50;
	}
	//
	for(int i = 0; i < 50; i++) {
		if(exp >= WeaponEXP[i]) {
		   //need more
		}
		else {
		   return i;
		}
	}
	return -1;
}

void survivalControl::addWeaponEXP(weaponInfo *weapon, F32 amount, bool absolute) {
	F32 prior = getWeaponEXP(weapon);
   int currentLV = getWeaponLevel(weapon);

	if(!absolute) {
	   weapon->exp += amount * EXPMulti;
	}
	else {
	   weapon->exp += amount;
	}

	if(weapon->exp >= WeaponEXP[currentLV]) {
		//Con::printf("onLevelUpWeapon(%s, %i);", weapon->GunName.c_str(), currentLV+1);
		Con::evaluatef("onLevelUpWeapon(%s, %i);", weapon->GunName.c_str(), currentLV+1);
		//check for additional levels
		if(weapon->exp >= WeaponEXP[currentLV+1]) {
		   //loop through the remaining ones
			for(int i = currentLV+1; i < 50; i++) {
				if(weapon->exp >= WeaponEXP[i]) {
					//Con::printf("onLevelUpWeapon(%s, %i);", weapon->GunName.c_str(), currentLV+1);
					Con::evaluatef("onLevelUpWeapon(%s, %i);", weapon->GunName.c_str(), currentLV+1);
				}
			}
		}
	}
}

void survivalControl::unlockWeapon(survivalControl::weaponInfo *gun) {
	if(!gun->unlocked) {
	   gun->unlocked = 1;
	   Con::evaluatef("onUnlockGun(%i);", gun->id);
	}
}

//File Controls
void survivalControl::saveInfo(const char *guid, const char *file) {
   FileObject *fileObj = new FileObject();
	fileObj->registerObject();

   fileObj->openForWrite(file, false);
	String lineWrite;
   
	Con::printf("SaveInfo: %s => %s", guid, file);
	//write the XML file Heading
	fileObj->writeLine((const U8 *)"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>");

   lineWrite = "<!-- Saved Data for: ";
	lineWrite += guid;
	lineWrite += " -->"; 

	fileObj->writeLine((const U8 *)lineWrite.c_str());

	fileObj->writeLine((const U8 *)"<Data>");
	fileObj->writeLine((const U8 *)"  <!-- Weapon Information -->");
	//start at the head
	fileObj->writeLine((const U8 *)"  <WeaponData>");
	bool doneWrite = false, doneWrite2 = false;
	weaponInfo *gun = getWeapon(0);
	while(!doneWrite) {
		lineWrite = "    <Weapon name=\"";
		lineWrite += gun->GunName.c_str();
		lineWrite += "\">";
		fileObj->writeLine((const U8 *)lineWrite.c_str());

		lineWrite = "      <Info value=\"exp\">";
		lineWrite += F32ToConstChar(gun->exp);
		lineWrite += "</Info>";
		fileObj->writeLine((const U8 *)lineWrite.c_str());

		lineWrite = "      <Info value=\"kills\">";
		lineWrite += F32ToConstChar(gun->kills); 
		lineWrite += "</Info>";
		fileObj->writeLine((const U8 *)lineWrite.c_str());

		lineWrite = "      <Info value=\"headshots\">";
		lineWrite += F32ToConstChar(gun->headshots);
		lineWrite += "</Info>";
		fileObj->writeLine((const U8 *)lineWrite.c_str());

		lineWrite = "      <Info value=\"activeSpecialization\">";
		lineWrite += gun->GunSpec.c_str(); 
		lineWrite += "</Info>";
		fileObj->writeLine((const U8 *)lineWrite.c_str());

		lineWrite = "      <Info value=\"unlocked\">";
		lineWrite += intToConstChar(gun->unlocked); 
		lineWrite += "</Info>";
		fileObj->writeLine((const U8 *)lineWrite.c_str());
		fileObj->writeLine((const U8 *)"    </Weapon>");

		//Con::printf("GUN: %i (%s) => %i, %i, %s, %b", gun->id, gun->GunName.c_str(), gun->kills, gun->headshots, gun->GunSpec.c_str(), gun->unlocked);

		gun = gun->next;
		if(gun == NULL) {
		   doneWrite = true;
		}
	}
	fileObj->writeLine((const U8 *)"  </WeaponData>");
	fileObj->writeLine((const U8 *)"  <!-- Challenge Information -->");
	fileObj->writeLine((const U8 *)"  <ChallengeData>");
	doneWrite = false;
	challengeTree *tree = tier;
	challenge *c = tier->headChallenge;
	while(!doneWrite) {
		c = tree->headChallenge;
	   //write the challenge trees
		lineWrite = "    <ChallengeTree treeIndex=\""; 
		lineWrite += intToConstChar(tree->id); 
		lineWrite += "\">"; 
		fileObj->writeLine((const U8 *)lineWrite.c_str());

		while(!doneWrite2) {
			lineWrite = "      <Challenge cIndex=\"";
			lineWrite += intToConstChar(c->id); 
			lineWrite += "\">";
			fileObj->writeLine((const U8 *)lineWrite.c_str());

			lineWrite = "        <Info value=\"cGoal\">"; 
			lineWrite += c->goal.c_str(); 
			lineWrite += "</Info>";
			fileObj->writeLine((const U8 *)lineWrite.c_str());

			lineWrite = "        <Info value=\"complete\">"; 
			lineWrite += intToConstChar(c->completed); 
			lineWrite += "</Info>";
			fileObj->writeLine((const U8 *)lineWrite.c_str());
			fileObj->writeLine((const U8 *)"      </Challenge>");
		   c = c->next;
			if(c == NULL) {
			   doneWrite2 = true;
			}
		}
		doneWrite2 = false;
	   fileObj->writeLine((const U8 *)"    </ChallengeTree>");
		tree = tree->next;
		if(tree == NULL) {
		   doneWrite = true;
		}
	}
	fileObj->writeLine((const U8 *)"  </ChallengeData>");
	fileObj->writeLine((const U8 *)"</Data>");
	fileObj->close();
	fileObj->destroySelf();
	//encrypt the file using AES
	cryptoPackage->AESEnc_File(cryptoPackage->AESFileKey(), file, 1024);
}

void survivalControl::loadInfo(const char *file) {
   SimXMLDocument *xml = new SimXMLDocument();
	if(!Platform::isFile(file)) {
		Con::errorf("loadInfo: File %s not found.", file);
		return;
	}
	xml->registerObject();
	cryptoPackage->AESDec_File(cryptoPackage->AESFileKey(), file, 1024);
	xml->loadFile(file);
   cryptoPackage->AESEnc_File(cryptoPackage->AESFileKey(), file, 1024);

	xml->pushChildElement(0);
	xml->pushFirstChildElement("WeaponData");
	while(true) {
		xml->pushFirstChildElement("Weapon");
		while(true) {
			weaponInfo *gun = getWeapon(xml->attribute("name"));
			xml->pushFirstChildElement("Info");
			while(true) {
				//set the attributal info
				if(strcmp(xml->attribute("value"), "exp") == 0) {
					const char *exp = xml->getData();
					gun->exp = dAtof(exp);
				}
				else if(strcmp(xml->attribute("value"), "kills") == 0) {
					const char *kills = xml->getData();
					gun->kills = dAtof(kills);				
				}
				else if(strcmp(xml->attribute("value"), "headshots") == 0) {
					const char *headshots = xml->getData();
					gun->headshots = dAtof(headshots);				
				}
				else if(strcmp(xml->attribute("value"), "activeSpecialization") == 0) {
					gun->GunSpec = xml->getData();		
				}
				else if(strcmp(xml->attribute("value"), "unlocked") == 0) {
					gun->unlocked = dAtoi(xml->getData());
				}
				else {
					Con::errorf("FILE LOAD ERROR: Invalid Field %s found in weapon load", xml->attribute("value"));
				}
				//check for next item.
				if(!xml->nextSiblingElement("Info")) {
				   break;
				}
			}
			xml->popElement();
			if(!xml->nextSiblingElement("Weapon")) {
			   break;
			}
		}
		xml->popElement();
		break;
	}
	xml->popElement();
	xml->pushFirstChildElement("ChallengeData");
	while(true) {
	   xml->pushFirstChildElement("ChallengeTree");
		while(true) {
		   challengeTree *tree = getChallengeTree(atoi(xml->attribute("treeIndex")));
		   xml->pushFirstChildElement("Challenge");
		   while(true) {
				challenge *c = getChallenge(tree, atoi(xml->attribute("cIndex")));
			   xml->pushFirstChildElement("Info");
				while(true) {
				   if(strcmp(xml->attribute("value"), "cGoal") == 0) {
						c->goal = xml->getData();
				   }
				   else if(strcmp(xml->attribute("value"), "complete") == 0) {
					   const char *compl = xml->getData();
					   c->completed = dAtoi(compl);				
				   }
				   else {
					   Con::errorf("FILE LOAD ERROR: Invalid Field %s found in challenge load", xml->attribute("value"));
				   }
					//
					if(!xml->nextSiblingElement("Info")) {
					   break;
					}
				}
				xml->popElement();
				if(!xml->nextSiblingElement("Challenge")) {
				   break;
				}
			}
			xml->popElement();
			if(!xml->nextSiblingElement("ChallengeTree")) {
			   break;
			}
		}
		break;
	}
	xml->destroySelf();
}

void survivalControl::dumpAll() {
   weaponInfo *gun = topWeapon;
	challengeTree *top = tier;
	challenge *c = tier->headChallenge;

	bool done = false, done2 = false;
	Con::printf("WEAPONS:");
	while(!done) {
		Con::printf("%i: %s", gun->id, gun->GunName.c_str());
		gun = gun->next;
		if(gun == NULL) {
		   done = true;
		}
	}
	done = false;
	Con::printf("\nCHALLENGES:");
	while(!done) {
		Con::printf("TREE %i: %s", top->id, top->name.c_str());
		c = top->headChallenge;
		while(!done2) {
			Con::printf("CHALLENGE %i, %s (%s)", c->id, c->name.c_str(), c->desc.c_str());
			c = c->next;
			if(c == NULL) {
			   done2 = true;
			}
		}
		done2 = false;
		//
		top = top->next;
		if(top == NULL) {
		   done = true;
		}
	}
}

//TS Definitions
DefineEngineFunction(addWeapon, void, (const char *name),, "(string) Adds a weapon to the challenge system") {
	survive->addWeapon(name);
}

DefineEngineFunction(getWeaponID, int, (const char *name),, "(string) Returns the internal weapon ID") {
	survivalControl::weaponInfo *weapon = survive->getWeapon(name);
	if(weapon != NULL) {
	   return weapon->id;
	}
	else {
	   return -1;
	}
}

DefineEngineFunction(addChallengeTree, void, (const char *name),, "(string) Adds a challenge tree") {
   survive->addChallengeTree(name);
}

DefineEngineFunction(addChallenge, void, (int cTreeIndex, const char *name, const char *desc, const char *goal, const char *rew),,
							"(int, string, string, string) Adds a challenge to a challenge tree") {
   survive->addChallenge(name, desc, goal, rew, survive->getChallengeTree(cTreeIndex));
}

DefineEngineFunction(challengeComplete, int, (int cTreeIndex, int challengeIndex),, "(int, int) checks completion of a challenge") {
	survivalControl::challengeTree *tree = survive->getChallengeTree(cTreeIndex);
	if(tree == NULL) {
	   return -1;
	}
	survivalControl::challenge *chlg = survive->getChallenge(tree, challengeIndex);
	if(chlg == NULL) {
	   return -1;
	}
	return chlg->completed;
}

DefineEngineFunction(weaponUnlocked, int, (int wID),, "(int) returns if a weapon is unlocked") {
	survivalControl::weaponInfo *gun = survive->getWeapon(wID);
	return gun->unlocked;
}

DefineEngineFunction(unlockWeapon, void, (int wID),, "(int)") {
	survivalControl::weaponInfo *gun = survive->getWeapon(wID);
	survive->unlockWeapon(gun);
}

DefineEngineFunction(getChallengeByName, int, (int weaponID, const char *name),, "(int, string) get challenge ID by name") {
	survivalControl::challengeTree *tree = survive->getChallengeTree(weaponID);
	if(tree == NULL) {
	   return -1;
	}
	else {
		survivalControl::challenge *c = survive->getChallenge(tree, name);
		if(c == NULL) {
		   return -1;
		}
		return c->id;
	}
}

DefineEngineFunction(getChallenge, const char *, (int weaponID, int cPos),, "(int, int) get challenge info") {
   //test the weapon/tree
	survivalControl::challengeTree *tree = survive->getChallengeTree(weaponID);
	if(tree == NULL) {
	   return "ERROR";
	}
	else {
	   //test the challenge to the tree
		survivalControl::challenge *chlg = survive->getChallenge(tree, cPos);
		if(chlg == NULL) {
		   return "ERROR";
		}
		else {			
			String output;
			output += chlg->name;
			output += "\t";
			output += chlg->desc;
			output += "\t";
			output += chlg->goal;
			output += "\t";
			output += chlg->reward;

			return output.c_str();
		}
	}
}

DefineEngineFunction(updateChallengeProgress, void, (int cTree, int cid, const char *progress),, "(int, int, string)") {
   //test the weapon/tree
	survivalControl::challengeTree *tree = survive->getChallengeTree(cTree);
	if(tree == NULL) {
		Con::errorf("updateChallengeProgress - Bad Tree");
	}
	else {
	   //test the challenge to the tree
		survivalControl::challenge *chlg = survive->getChallenge(tree, cid);
		if(chlg == NULL) {
		   Con::errorf("updateChallengeProgress - Bad Tree/CID Combo");
		}
		else {	
			//Con::printf("update progress: %i %i => %s", cTree, cid, progress);
		   survive->updateProgress(chlg, progress);
		}
	}
}

DefineEngineFunction(completeChallenge, void, (int cTree, int CID),, "(int, int)") {
	survivalControl::challengeTree *tree = survive->getChallengeTree(cTree);
	if(tree == NULL) {
		Con::errorf("completeChallenge - Bad Tree");
	}
	else {
		survivalControl::challenge *chlg = survive->getChallenge(tree, CID);
		if(chlg == NULL) {
		   Con::errorf("completeChallenge - Bad Tree/CID Combo");
		}
		else { 
			survive->flagCompleted(chlg);
		}
	}
}

DefineEngineFunction(setEXPModifier, void, (F32 newMod, const char *sig),, "(int, string)") {
	survive->setEXPMultiplier(newMod, sig);
}

DefineEngineFunction(getEXPModifier, F32, (),, "returns the current EXP Modifier") {
	return survive->getEXPMultiplier();
}

DefineEngineFunction(addWeaponExperience, void, (int weaponID, F32 amount),, "(int, F32)") {
	survivalControl::weaponInfo *gun = survive->getWeapon(weaponID);
	survive->addWeaponEXP(gun, amount, false);
}

DefineEngineFunction(processReward, void, (int weaponID, const char *reward),, "(int, string)") {
	survivalControl::weaponInfo *gun = survive->getWeapon(weaponID);
	//strip field
	String sReward = reward;
	sReward.replace("EXP", "");
	survive->addWeaponEXP(gun, dAtof(sReward.c_str()), true);
}

DefineEngineFunction(getWeaponInfo, const char *, (int weaponID),, "(int) push the weapon info string") {
	survivalControl::weaponInfo *gun = survive->getWeapon(weaponID);
	if(gun != NULL) {
      char exp[32], needed[32], level[4], kills[32], hss[32];
		dSprintf(exp, sizeof(exp), "%.0f", gun->exp);
		dSprintf(needed, sizeof(needed), "%.0f", survive->getNeededEXP(gun));
		dSprintf(level, sizeof(level), "%i", survive->getWeaponLevel(gun));
		dSprintf(kills, sizeof(kills), "%.0f", gun->kills);
		dSprintf(hss, sizeof(hss), "%.0f", gun->headshots);

	   String output;
		output += gun->GunName;
		output += "\t";
		output += gun->GunSpec;
		output += "\t";
		output += exp;
		output += "\t";
		output += needed;
		output += "\t";
		output += level; //kinda did this to make the GUI screen easier, could have just easily did a getLevel(exp)
		output += "\t";
		output += kills;
		output += "\t";
		output += hss;

		return output.c_str();
	}
	return "ERROR";
}

DefineEngineFunction(setWeaponSpec, void, (int wID, const char *spec),, "(int, string) set a weapon specialization") {
	survivalControl::weaponInfo *gun = survive->getWeapon(wID);
	if(gun != NULL) {
      F32 kills = gun->kills;
		F32 headshots = gun->headshots;

	   survive->updateWeaponDetails(gun, spec, kills, headshots);
	}
}

DefineEngineFunction(addWeaponKill, void, (int wID),, "(int) add a kill to a weapon") {
	survivalControl::weaponInfo *gun = survive->getWeapon(wID);
	if(gun != NULL) {
		String spec = gun->GunSpec;
		F32 headshots = gun->headshots;

		survive->updateWeaponDetails(gun, spec.c_str(), (gun->kills + 1), headshots);
	}
}

DefineEngineFunction(addWeaponHeadshot, void, (int wID),, "(int) add a headshot to a weapon") {
	survivalControl::weaponInfo *gun = survive->getWeapon(wID);
	if(gun != NULL) {
		String spec = gun->GunSpec;
		F32 kills = gun->kills;

		survive->updateWeaponDetails(gun, spec.c_str(), kills, (gun->headshots + 1));
	}
}

DefineEngineFunction(saveClientData, void, (const char *guid, const char *path),, "(string, string) save data") {
	if(strcmp(guid, "") == 0) {
		Con::errorf("saveClientData() - error, cannot save without a valid guid.");
		return;
	}
	if(strcmp(path, "") == 0) {
		Con::errorf("saveClientData() - error, cannot save without a valid path.");
		return;
	}
	survive->saveInfo(guid, path);
}

DefineEngineFunction(loadClientData, void, (const char *path),, "(string) load data") {
	if(strcmp(path, "") == 0) {
		Con::errorf("loadClientData() - error, cannot save without a valid path.");
		return;
	}
	survive->loadInfo(path);
}

DefineEngineFunction(checkInfo, void, (),, "") {
   survive->dumpAll();
}