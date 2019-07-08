#include "PGD/Solutions/statsControl.h"

#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

using namespace std;

statsContainer *stats = NULL;

statsContainer::statsContainer() {
   topStat = NULL;
}

statsContainer::~statsContainer() {
}

void statsContainer::create() {
   if(stats == NULL) {
      stats = new statsContainer();
		Con::printf("* MAP: Stats System Module Activated");
   }
}

void statsContainer::destroy() {
   if(stats != NULL) {
      delete stats;
	   stats = NULL;
   }
}

//Asset Functions
const char *statsContainer::F32ToConstChar(F32 in) {
   char output[32];
   dSprintf(output, 32, "%.0f", in);
   return output;
}

const char *statsContainer::intToConstChar(int in) {
   char output[32];
   dSprintf(output, 32, "%i", in);
   return output;
}

//singleStat Class:
statsContainer::singleStat::singleStat(const char *n, int ident) {
   statName = n;
	statValue = 0;
	id = ident;
	next = NULL;
}

void statsContainer::addStat(const char *n) {
	if(topStat == NULL) {
		topStat = new singleStat(n, 0);
	}
	else {
	   int index = 0;
		singleStat *temp = topStat;
		while(temp->next != NULL) {
		   temp = temp->next;
			index++;
		}
		singleStat *ptr = new singleStat(n, index+1);
		temp->next = ptr;
	}
}

statsContainer::singleStat *statsContainer::getStat(const char *n) {
   singleStat *theStat = topStat;
	bool done = false;
	while(!done) {
		if(strcmp(theStat->statName, n) == 0) {
		   return theStat;
		}
	   theStat = theStat->next;
		if(theStat == NULL) {
		   done = true;
		}
	}
	return NULL;
}

statsContainer::singleStat *statsContainer::getStat(int ident) {
   singleStat *theStat = topStat;
	bool done = false;
	while(!done) {
		if(theStat->id == ident) {
		   return theStat;
		}
	   theStat = theStat->next;
		if(theStat == NULL) {
		   done = true;
		}
	}
	return NULL;
}

void statsContainer::addToStat(singleStat *stat, F32 amount) {
   if(stat == NULL) {
      return;
   }
   stat->statValue += amount;
}

String statsContainer::getStatName(singleStat *stat) {
   return stat->statName;
}

F32 statsContainer::getStatValue(singleStat *stat) {
   return stat->statValue;
}

void statsContainer::saveInfo(const char *guid, const char *file) {
   singleStat *stat = topStat;

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

   doneWrite = false;

   fileObj->writeLine((const U8 *)lineWrite.c_str());
   fileObj->writeLine((const U8 *)"<Data>");
   while(!doneWrite) {
      lineWrite = "   <Stat sIndex=\"";
	  lineWrite += intToConstChar(stat->id); 
	  lineWrite += "\">";
	  fileObj->writeLine((const U8 *)lineWrite.c_str());

	  lineWrite = "      <Info value=\"sName\">"; 
	  lineWrite += stat->statName.c_str(); 
	  lineWrite += "</Info>";
	  fileObj->writeLine((const U8 *)lineWrite.c_str());  

	  lineWrite = "      <Info value=\"sValue\">"; 
	  lineWrite += F32ToConstChar(stat->statValue); 
	  lineWrite += "</Info>";
	  fileObj->writeLine((const U8 *)lineWrite.c_str()); 
	  fileObj->writeLine((const U8 *)"   </Stat>");
	  stat = stat->next;
	  if(stat == NULL) {
	     doneWrite = true;
	  }
   }
   fileObj->writeLine((const U8 *)"</Data>");

   fileObj->close();
   fileObj->destroySelf();
   //encrypt the file using AES
   cryptoPackage->AESEnc_File(cryptoPackage->AESFileKey(), file, 1024);
}

void statsContainer::loadInfo(const char *file) {
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
   xml->pushFirstChildElement("Stat");
   while(true) {
      //Phantom139: We assume the stats are saved into the game by default and the info will be here.
	  singleStat *stat = getStat(dAtoi(xml->attribute("sIndex")));
	  xml->pushFirstChildElement("Info");
	  while(true) {
         if(strcmp(xml->attribute("value"), "sName") == 0) {
		    const char *storedName = xml->getData();
		    stat->statName = storedName;
		 }
		 else if(strcmp(xml->attribute("value"), "sValue") == 0) {
		    const char *storedValue = xml->getData();
			stat->statValue = dAtof(storedValue);				
		 }	  
		 else {
		    Con::errorf("Unknown field found in stats loader: %s", xml->attribute("value"));
		 }
		 //check for next item.
		 if(!xml->nextSiblingElement("Info")) {
		    break;
		 }
	  }
	  xml->popElement();
	  if(!xml->nextSiblingElement("Stat")) {
	     break;
	  }
   }
   xml->destroySelf();
}

//TS Definitions
DefineEngineFunction(addStat, void, (const char *name),, "(string) Adds a stat to the stats system") {
   stats->addStat(name);
}

DefineEngineFunction(getStatID, int, (const char *name),, "(string) Returns the internal stat ID by Name") {
   statsContainer::singleStat *stat = stats->getStat(name);
   if(stat != NULL) {
      return stat->id;
   }
   else {
      return -1;
   }
}

//Phantom139: In TorqueScript use %info = getStatInfo(getStatID(name here)); then: %statName = getField(%info, 0); and %statValue = getField(%info, 1);
DefineEngineFunction(getStatInfo, const char *, (int id),, "(int) Returns a stat's info (name TAB value) via ID") {
   statsContainer::singleStat *stat = stats->getStat(id);
   if(stat == NULL) {
      Con::errorf("Error: No such stat ID exists in the system.");
	  return;
   }

   String out;
   out = getStatName(stat);
   out += "\t";
   out += F32ToConstChar(getStatValue(stat));

   return out.c_str();
}

//If you just want a non-tabbed string value of the stat, use this function (good for things like: EXP, Kills, Headshots, ect.)
DefineEngineFunction(getStatValue, const char *, (int id),, "(int) Returns a stat's numerical value via ID") {
   statsContainer::singleStat *stat = stats->getStat(id);
   if(stat == NULL) {
      Con::errorf("Error: No such stat ID exists in the system.");
	  return;
   }

   String out;
   out = F32ToConstChar(getStatValue(stat));
   return out.c_str();
}

DefineEngineFunction(updateStat, void, (int id, F32 amount),, "(int, F32) Add or subtract an amount from a stat.") {
   statsContainer::singleStat *stat = stats->getStat(id);
   if(stat == NULL) {
      Con::errorf("Error: No such stat ID exists in the system.");
	  return;
   }

   stats->addToStat(stat, amount); //this function uses a basic += relation, so (+ or -) in the F32 amount will work.
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
   stats->saveInfo(guid, path);
}

DefineEngineFunction(loadClientData, void, (const char *path),, "(string) load data") {
   if(strcmp(path, "") == 0) {
      Con::errorf("loadClientData() - error, cannot load without a valid path.");
	  return;
   }
   stats->loadInfo(path);
}