/**
PGDStore.cpp
(c) Phantom Games Development, 2012
All Rights Reserved

PGDStore:
Synopsis:
The PGDStore class is a giant encrypted data storage module that can build fields of data and save them in AES-256 Hex. It
defines a linked list of all of the fields of data, sort of like an XML table, and then it takes in the individual data points
and stores them. Once that is done, the entire list is encrypted, and can be saved to an exterior file for future loading.
**/

#include "PGD/Solutions/PGDStore.h"

PGDStore *pgs_main = NULL;
PGDStore::PGDStore() {
   chain_head = NULL;
}

PGDStore::~PGDStore() {

}

void PGDStore::create() {
   if(pgs_main == NULL) {
	   pgs_main = new PGDStore();
		Con::printf("MAP Module Loaded: PGS");
	}
}

void PGDStore::destroy() {
	if(pgs_main != NULL) {
	   delete pgs_main;
		pgs_main = NULL;
		Con::printf("MAP Module Removed: PGS");
	}
}

void PGDStore::Add_PGS_Table(const char *n) {
	if(chain_head == NULL) {
	   //create L-List head
		chain_head = new _PGS_Value(n);
	}
	else {
	   _PGS_Value *tmp = chain_head, *ptr = new _PGS_Value(n);
		while(tmp->nic != NULL) {
		   tmp = tmp->nic;
		}
		tmp->nic = ptr;
	}
}

//******************************
//_PGS_Value Struct Definitions
//******************************
PGDStore::_PGS_Value::_PGS_Value(const char *n) {
   name = n;
	nic = NULL;

	set_value("0");
	Con::printf("PGDStore: Added_PGS: %s -> %s", name, get_value().c_str());
}

void PGDStore::_PGS_Value::set_value(std::string v) {
	cryptoPackage->AESEncrypt(cryptoPackage->pgdHash("Multiplayer Assembly Package"), v, value, 512);
}

std::string PGDStore::_PGS_Value::get_value() {
	std::string out;
	cryptoPackage->AESDecrypt(cryptoPackage->pgdHash("Multiplayer Assembly Package"), value, out, 512);
	return out;
}

//******************************
//_PGS_Value Struct Functions
//******************************
PGDStore::_PGS_Value *PGDStore::getByName(const char *n) {
   _PGS_Value *tmp = chain_head;
	do {
		if(strcmp(tmp->name, n) == 0) {
         return tmp;		
		}
		else {
		   tmp = tmp->nic;
		}
	}while(tmp->nic != NULL);

	return NULL;
}

std::string PGDStore::get_value(const char *n) {
	_PGS_Value *tmp = getByName(n);
	if(tmp != NULL) {
		return tmp->get_value();
	}
	Con::errorf("PGDStore::get_value() - supplied target name returned NULL data");
	return "NULL";
}

bool PGDStore::set_value(const char *n, const char *nv) {
	_PGS_Value *tmp = getByName(n);
	if(tmp != NULL) {
		tmp->set_value(nv);
		return true;
	}
	Con::errorf("PGDStore::set_value() - supplied target name returned NULL data");
	return false;
}

//************************************************
//File Writing/Reading
//************************************************
void PGDStore::saveToFile(const char *filePath) {
   _PGS_Value *tmp = chain_head;
	//open the file
	std::fstream file_out(filePath, std::ios::out);
	do {
		file_out << tmp->name << ":" << tmp->value << "\n";
	}while(tmp->nic != NULL);
	file_out.close();
	//fully encrypt the file
	cryptoPackage->AESEnc_File(cryptoPackage->pgdHash("Multiplayer Assembly Package"), filePath, 1024);
}

void PGDStore::loadFromFile(const char *filePath) {
   _PGS_Value *tmp = chain_head;
	bool fullLoad = false;
	//decrypt the file
	cryptoPackage->AESDec_File(cryptoPackage->pgdHash("Multiplayer Assembly Package"), filePath, 1024);
	//create the file reader
	std::fstream file_in(filePath, std::ios::in);
	std::string line, newName, newValue, f_newValue;
	while(!file_in.eof()) {
	   getline(file_in, line);
		//split it...
		newName = line.substr(0, line.find_first_of(":"));
		newValue = line.substr(line.find_first_of(":")+1, line.length());
		//decrypt the value
		cryptoPackage->AESDecrypt(cryptoPackage->pgdHash("Multiplayer Assembly Package"), newValue, f_newValue, 512);
		//add the PGS Entry
		if(chain_head == NULL || fullLoad) {
		   //we're fully loading everything
			fullLoad = true;
			Add_PGS_Table(newName.c_str());
			set_value(newName.c_str(), f_newValue.c_str());
		}
		else {
		   //this is where things get a little more tricky..
			//we need to first find the entry... and if it doesn't exist, create it
			_PGS_Value *find = getByName(newName.c_str());
			if(find == NULL) {
				Add_PGS_Table(newName.c_str());
				set_value(newName.c_str(), f_newValue.c_str());
			}
			else {
				set_value(newName.c_str(), f_newValue.c_str());
			}
		}
	}
	file_in.close();
}