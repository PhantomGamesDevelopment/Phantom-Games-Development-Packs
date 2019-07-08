/**
cryptoPackage.cpp
Header file for cryptoPP function calls

Phantom Games Development Authentication Package xxz568
Package Version: 1.2, Compiled Mar. 24, 2011
Created By: Robert Clayton Fritzen (Phantom139)
Copyright 2011, Phantom Games Development

Libraries Used In Development: CryptoPP

Credit to the security and cryptography functions go to Wei Dai, who created the CryptoPP library, 
Phantom Games Development does not take any credit for these functions, or the provided function examples
used from the CryptoPP Library, all rights and provided licence agreements to those are reserved 
by the CryptoPP Group and their respective authors.

Unauthorized usage of this system is strictly prohibited, and may be met with legal measures.
This system cannot be used in countries to which the United States Of America has embargoed goods to.

CHANGE LOG
1.0:

* First Implementation
* Basic Hashing Algorithm (SHA1)
* Implementation of Hex Encode/Decode, Base64 Encode/Decode
* RSA Key Generation
* CryptoPP::Integer Loading

1.1:

* Public Release 1
* Revised RSA Code, Now Uses Sign/Verify, as Well as CA_Verify
* Added CA_public for placement of CA's public exponent
* Added Whirlpool Hash, and implementation of PGD Combo Hash
* Added AES Encryption/Decryption, Random Block IV
* Added Method of Getting UTC Time
* Fixed a bug in Integer Loading (Can now properly load Hex Values)

1.2:

* Reimplemented AES, now uses PBKDF, and fixed 0x00 Decryption Failure Error (Thanks Geoff Beier!)
* Added More Hashing Functions:
* SHA2 Family (224, 256, 384, 512)
* Tiger

**/

// Crypto++ Library
#ifdef _DEBUG
#  pragma comment ( lib, "cryptlibd" )
#else
#  pragma comment ( lib, "cryptlib" )
#endif
//Include CryptoPP headers
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1 

#include <modes.h>
#include <base64.h>
#include <aes.h>
#include <filters.h>
#include <rsa.h>
#include <sha.h>
#include <hex.h>
#include <whrlpool.h>
#include <osrng.h>
#include <pwdbased.h>
#include <files.h>
#include <md5.h>
//End
#include <time.h>
#include <iostream>
#include <strstream>

#ifndef _CONSOLE_H_
   #include "console/console.h"
   #include "console/consoleInternal.h"
#endif
#include "core/volume.h"
#include "core/strings/stringUnit.h"
#include "core/fileObject.h"

//#include <curl/curl.h>
//#pragma comment ( lib, "libcurl" )
//#define CURL_STATICLIB

using namespace CryptoPP;

#ifndef xxz568_H
#define xxz568_H

class xxz568 {
   public:  
   //--------------------------
   //xxz568 RSA functions
	  const char * caPublic();
     const char * AESFileKey();

	  bool caVerify(std::string message, std::string signature);

	  const char *iToCC(int in);
   //--------------------------
   //xxz568 Hashing functions
	  std::string sha1(std::string text);      //1.0
	  std::string whirlpool(std::string text); //1.1
	  std::string pgdHash(std::string text);   //1.1
   //--------------------------
   //xxz568 Encryption Functions
	  int AESEncrypt(std::string key, std::string input, std::string &output, unsigned int PBKDFiterations);
     int AESDecrypt(std::string key, std::string input, std::string &output, unsigned int PBKDFiterations);

	  int AESEnc_File(std::string key, std::string path, unsigned int PBKDFiterations, std::string outfile = "");
	  int AESDec_File(std::string key, std::string path, unsigned int PBKDFiterations, std::string outfile = "");

	  bool isEncryptedFile(const char *path);

	  int HexEncode(std::string input, std::string &output);
	  int HexDecode(std::string input, std::string &output);
	  int Base64Encode(std::string input, std::string &out);
	  int Base64Decode(std::string input, std::string &out);
   //--------------------------
   //xxz568 Misc Functions
	  std::string IntegerToString(const CryptoPP::Integer refs);
     unsigned int getUTC();

	  CryptoPP::Integer load(std::string inbound, int radix);

	  const char *getUserPath(const char *platform);
   //--------------------------

	  //xxz568 & libCurl Code:
	  void setPGDServer(String server, String authPath);
	  void generateRSAKey();
     String recoverAccount(const char *username, const char *password);
	  String registerAccount(const char *username, const char *password, const char *email, const char *key);
	  void writeCertificate(String n, String g, String e, String rsa_e, String rsa_n, String rsa_s, String hash, String rsa_d);
	  const char *getServer();

	  static void create();
	  static void destroy();
	  xxz568();
	  ~xxz568();

   private:
     InvertibleRSAFunction accountRSA;
	  String connectedServer;
	  String authenticationPath;

   protected:
};

#endif

extern xxz568 *cryptoPackage;