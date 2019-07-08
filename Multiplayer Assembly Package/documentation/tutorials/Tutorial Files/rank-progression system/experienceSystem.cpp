#include "PGD/Solutions/experienceSystem.h"

#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

using namespace std;

expControl *experience = NULL;

const F32 expControl::expTable[MAX_LEVEL] = {0, 50, 100, 150, 200, 250, 300};

//ASSET FUNCTIONS
const char * expControl::F32ToConstChar(F32 in) {
   char output[32];
   dSprintf(output, 32, "%.0f", in);

   return output;
}

const char * expControl::intToConstChar(int in) {
   char output[sizeof(int)];
   dSprintf(output, sizeof(output), "%i", in);

   return output;
}

expControl::expControl() {
}

expControl::~expControl() {
}

void expControl::create() {
   if(experience == NULL) {
      experience = new expControl();
	   Con::printf("Expereince System Online");
   }
}

void expControl::destroy() {
   if(experience != NULL) {
      delete experience;
   	experience = NULL;
   }
}

F32 expControl::getEXP() {
   return plEXP;
}

F32 expControl::getNeededEXP() {
   int level = getLevel();
   if(level >= MAX_LEVEL) {
      return getEXP();
   }
   return expTable[level];
}

int expControl::getLevel() {
   F32 exp = getEXP();
   //special cases
   if(exp >= expTable[MAX_LEVEL - 1]) {
      return MAX_LEVEL;
   }
   //
   for(int i = 0; i < MAX_LEVEL; i++) {
      if(exp >= expTable[i]) {
         //need more
	   }
	   else {
         return i;
	   }
   }
   return -1;
}

void expControl::addEXP(F32 amount) {
   F32 prior = getEXP();
   int currentLV = getLevel();
   plEXP += amount;

   if(plEXP >= expTable[currentLV]) {
      Con::evaluatef("onLevelUp(%i);", currentLV+1);
	   //check for additional levels
	   if(plEXP >= expTable[currentLV+1]) {
         //loop through the remaining ones
	      for(int i = currentLV+1; i < MAX_LEVEL; i++) {
	         if(plEXP >= expTable[i]) {
	            Con::evaluatef("onLevelUp(%i);", currentLV+1);
		      }
	      }
	   }
   }
}

//File Controls
void expControl::saveInfo(const char *guid, const char *file) {
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

	lineWrite = "      <Info value=\"exp\">";
	lineWrite += F32ToConstChar(plEXP); 
	lineWrite += "</Info>";
	fileObj->writeLine((const U8 *)lineWrite.c_str());

	fileObj->writeLine((const U8 *)"</Data>");
	fileObj->close();
	fileObj->destroySelf();
	//encrypt the file using AES
	cryptoPackage->AESEnc_File(cryptoPackage->AESFileKey(), file, 1024);
}

void expControl::loadInfo(const char *file) {
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
	xml->pushFirstChildElement("Data");
	while(true) {
	   xml->pushFirstChildElement("Info");
		while(true) {
		   //set the attributal info
			if(strcmp(xml->attribute("value"), "exp") == 0) {
				const char *exp = xml->getData();
				plEXP = dAtof(exp);
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
		if(!xml->nextSiblingElement("Data")) {
		   break;
		}
	}
	xml->destroySelf();
}

DefineEngineFunction(addExperience, void, (F32 amount),, "(F32)") {
	experience->addEXP(amount);
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
	experience->saveInfo(guid, path);
}

DefineEngineFunction(loadClientData, void, (const char *path),, "(string) load data") {
	if(strcmp(path, "") == 0) {
		Con::errorf("loadClientData() - error, cannot save without a valid path.");
		return;
	}
	experience->loadInfo(path);
}

DefineEngineFunction(getInfo, const char *, (),, "() push the info string") {
   char exp[32], needed[32], level[4];
   dSprintf(exp, sizeof(exp), "%.0f", experience->getEXP());
   dSprintf(needed, sizeof(needed), "%.0f", experience->getNeededEXP());
   dSprintf(level, sizeof(level), "%i", experience->getLevel());
   
   String output;
   output += exp;
   output += "\t";
   output += needed;
   output += "\t";
   output += level; 
 
   return output.c_str();
}
