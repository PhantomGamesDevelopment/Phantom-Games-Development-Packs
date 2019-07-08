/**
PGDCOMMOD.cpp
Written By: Robert Fritzen

Copywrite 2012, Phantom Games Development
Controls administrator certificates.
**/

/**
SAMPLE CERTIFICATE FILE:
cmcert.xml
<?xml version="1.0" encoding="utf-8" standalone="yes" ?>
<CMCERTLIST>
    <CMDAT>
        <CERT>GUID:ALEVEL:RSASIG</CERT>
    </CMDAT>
</CMCERTLIST>
**/

#include "PGD/Crypto/cryptoPackage.h"
#include "PGD/Solutions/PGDCOMMOD.h"

#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

PCM *communityModerator = NULL;

PCM::PCM() {
}
PCM::~PCM() {
}

void PCM::create() {
	if(communityModerator == NULL) {
		communityModerator = new PCM();
	}
	Con::printf("PGD: MODULE LOADED => PCM (community moderator)");
}

void PCM::destroy() {
	if(communityModerator != NULL) {
		delete communityModerator;
		communityModerator = NULL;
	}
}

std::string PCM::verifyCMCertificate(std::string unpacked, std::string guid) {
	//strip RSA/GUID, check validity of GUID by doing a strcmp check
	//Con::printf("D: %s", unpacked.c_str());
	std::string guidC, adminC, rsaStr;
   guidC = unpacked.substr(0, unpacked.find_first_of(":"));
	unpacked = unpacked.substr(unpacked.find_first_of(":")+1, unpacked.length());
	adminC = unpacked.substr(0, unpacked.find_first_of(":"));
	unpacked = unpacked.substr(unpacked.find_first_of(":")+1, unpacked.length());
	rsaStr = unpacked.substr(0, unpacked.length());
   //check GUID right now
	//Con::printf("Comparing: %s => %s", guid.c_str(), guidC.c_str());
	if(guidC.compare(guid) != 0) {
	   return "-2";
	}
	//ready the rsa verify sig
	std::string msg = guidC.append(adminC);
	//Con::printf("SigTest1: %s", msg.c_str());
	//hash it, PGD
	std::string valid = cryptoPackage->pgdHash(msg);
	if(cryptoPackage->caVerify(valid, rsaStr)) {
		return adminC;
	}
	else {
	   return "-1";
	}
}

/**

ENGINE FUNCTION DEFINITIONS

**/

DefineEngineFunction(verifyCMCert, const char *, (const char* ctfc, const char *guid),, "Verify a CM cert") {
   return communityModerator->verifyCMCertificate(ctfc, guid).c_str();
}