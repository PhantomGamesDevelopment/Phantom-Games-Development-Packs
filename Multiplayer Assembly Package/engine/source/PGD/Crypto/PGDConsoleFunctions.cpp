/**
Phantom Games Development:
Cryptography Based Console Functions/Methods

Package Version: MAP 1.5 (Compiled Jun 22, 2013)
Created By: Robert Clayton Fritzen (Phantom139)
Copyright 2011 - 2013, Phantom Games Development
**/

//Disable annoyance errors
#pragma warning (disable : 4996)
#pragma warning (disable : 4239)

#define _LOADAUTH 1
#define _LOADALL 1
#include "PGD/Control/PGDMain.h"

#include "platform/platform.h"
#include "console/console.h"
#include "console/consoleInternal.h"
#include "console/engineAPI.h"

#include <fstream>
#include <string>
using namespace std; 

//console functions
//ConsoleFunctionGroupBegin(PGDCryptoFunctions, "PGD's crypto library functions for RSA, AES, and Hashing.");
#ifndef _PGDAUTH
   //inform the user that this .exe is not using the crypto lib.
   DefineEngineFunction(getFileMD5, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(PGDHash, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(whirlpool, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(sha1, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(base64encode, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(base64decode, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(getUTC, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(DecryptAccount, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(StoreSuccessfulLogin, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(GatherAccountDetails, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(AESEncrypt, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(AESDecrypt, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(setPGDServer, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(registerAccount, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
   DefineEngineFunction(recoverAccount, void, (),, "(Disabled)") {
      Con::printf("This executable is running on a non-auth based game, this function has been disabled for that purpose.");
   }
#endif

//Credit Where Due: Nathan Martin
DefineConsoleFunction( getEnvironmentVariable, const char*, (const char *variableName),,  
                "@brief Returns the value of the requested operating system's environment variable.\n"  
                "@param variableName Name of the environment variable\n"  
                "@return String value of the requested environment variable.\n"  
                "@ingroup Console") {  
    // get requested environment variable value  
    const char *result = getenv(variableName);  
  
    // verify successful operation  
    if(result) {  
       // success, allocate return space and return the result  
       char *ret = Con::getReturnBuffer(dStrlen(result) +1);  
       dStrcpy(ret, result);  
       return ret;  
    }  
    // fail, environment variable doesn't exist  
    return "";  
} 

//Phantom139:: Useful base-converting function of mine.
DefineEngineFunction(ConvertNumber, const char *, (const char * input, const char * newBase),, "converts a number") {
   int number = atoi(input);
   char * final = (char *)malloc(strlen(input));
   _itoa(number, final, atoi(newBase));

   return (const char *)final;
}

#ifdef _PGDAUTH
DefineEngineFunction(getFileMD5, const char *, (const char *input),, "Obtain a file's MD5 Hash") {
   std::string result;  
   //check if the file exists  
   if(!Platform::isFile(input)) {  
      //do not remove this if!!! you WILL get a crash if you do and point to a non existing file.  
      return "File Not Found";  
   }  
   //  
   const char * done = (const char *)malloc(64);  
  
   CryptoPP::Weak::MD5 hash;  
   CryptoPP::FileSource(input,true,new  
   CryptoPP::HashFilter(hash,new CryptoPP::HexEncoder(new  
   CryptoPP::StringSink(result),false)));   
  
   strcpy((char *)done, result.c_str());  
  
   return done;  
}

DefineEngineFunction(PGDHash, const char *, (const char *input),, "Hashes a string with PGD's hash") {
   std::string output_hash = cryptoPackage->pgdHash(input);
   const char *done = (const char *)malloc(128);
   strcpy((char *)done, output_hash.c_str());
   return done;
}

DefineEngineFunction(whirlpool, const char *, (const char *input),, "Hashes a string with the whirlpool hash") {
   std::string output_hash = cryptoPackage->whirlpool(input);
   const char *done = (const char *)malloc(128);
   strcpy((char *)done, output_hash.c_str());
   return done;
}

DefineEngineFunction(sha1, const char *, (const char *input),, "Hashes a string with the sha1 hash") {
   std::string output_hash = cryptoPackage->sha1(input);
   const char *done = (const char *)malloc(64);
   strcpy((char *)done, output_hash.c_str());
   return done;
}

DefineEngineFunction(base64encode, const char *, (const char *input),, "(string) returns the base64 encoded value of a string") {
   std::string output_b64E; 
   cryptoPackage->Base64Encode(input, output_b64E);
   return output_b64E.c_str();
}

DefineEngineFunction(base64decode, const char *, (const char *input),, "(string) returns the base64 decoded value of a string") {
   std::string output_b64D;
   cryptoPackage->Base64Decode(input, output_b64D);
   return output_b64D.c_str();
}

DefineEngineFunction(getUTC, int, (),,"(string) returns the UTC time string") {
   unsigned int utc = cryptoPackage->getUTC();
   int out = (int)utc;
   return out;
}

DefineEngineFunction(AESEncrypt, const char *, (const char *input, const char *key),, "(string) [input, key] AES Encrypt") {  
   std::string out;  
   cryptoPackage->AESEncrypt(string(key), string(input), out, 1024);  
      
   char *str = Con::getReturnBuffer(out.size() +1);  
   dStrcpy(str, out.c_str());  
      
   return str;  
}  

DefineEngineFunction(AESDecrypt, const char *, (const char *input, const char *key),, "(string) [input, key] AES Decrypt") {  
   std::string out;  
   cryptoPackage->AESDecrypt(string(key), string(input), out, 1024);  
      
   char *str = Con::getReturnBuffer(out.size() +1);  
   dStrcpy(str, out.c_str());  
      
   return str;  
}  

DefineEngineFunction(AESEncFile, void, (const char *file, const char *key, const char *out),, "(string) [input, key] AES Encrypt") {  
	Con::printf("Encrypting File: %s", file);
	cryptoPackage->AESEnc_File(key, file, 2048, out);  
}  

DefineEngineFunction(AESDecFile, void, (const char *file, const char *key, const char *out),, "(string) [input, key] AES Decrypt") {  
	Con::printf("Decrypting File: %s", file);
	cryptoPackage->AESDec_File(key, file, 2048, out);
}  

DefineEngineFunction(DecryptAccount, const char *, (const char *data, const char *decHash, const char *key),, "(string) decrypts an account running AES-CBC cipher") {
   //vars: encoded exponent, decrypted hash, password input
   std::string holderStringDec, decodedToStr, fin;
   std::string aes_cipher_key = key;
   aes_cipher_key.append(decHash);
   // run AES rounds, then compare to decoded hash
   std::string decodeHex = data;
   cryptoPackage->AESDecrypt(aes_cipher_key, decodeHex, holderStringDec, 1024);
   //fileobject test
   if(cryptoPackage->pgdHash(holderStringDec).compare(std::string(decHash)) == 0) {
      fin = holderStringDec;
   }
   else {
      fin = "INVALID_PASSWORD";
   }
   char *str = Con::getReturnBuffer(fin.size() +1);  
   dStrcpy(str, fin.c_str());  
      
   return str;
}

//Not sure why I havent depricated this yet...
//I'll do that later ;) 
DefineEngineFunction(StoreSuccessfulLogin, int, (const char *E, const char *N, const char *S),, "stores account exponents in a data structure") {
   char * EE = (char *)malloc(512);
   char * NE = (char *)malloc(512);
   char * SE = (char *)malloc(512);
   //
   strcpy(EE, E);
   strcpy(NE, N);
   strcpy(SE, S);

   Con::setVariable("$ClientKey_E", EE);
   Con::setVariable("$ClientKey_N", NE);
   Con::setVariable("$ClientKey_S", SE);
   return 1;
}

DefineEngineFunction(GatherAccountDetails, void, (const char *det, const char *E, const char *N, const char *S),, 
	"(string) returns a long appended string of account details (E, N, SIG)") {
   //RSA_Verify Time :D
   //what is needed:
      //1: Full Details: whirlpool($guid@$name)
      //2: The Signature: base64
   //We are given the guid, email, and name by the client, attach E and N, then calc a whirlpool hash.
   std::string toWhrl = det;
   toWhrl.append(E);
   toWhrl.append(N);
   std::string toWhrl_final = toWhrl;
   toWhrl_final = cryptoPackage->pgdHash(toWhrl);
   //
   std::string hexSig;
   hexSig.assign(string(S));
   //Con::printf("CA_V: Debug: %s | %s | %s", toWhrl.c_str(), toWhrl_final.c_str(), hexSig.c_str());
   //
   bool rsaverifyresult = cryptoPackage->caVerify(toWhrl_final, hexSig);   
   if(rsaverifyresult != true) {
	   //Naughty Naughty... someone is using a bogus certificate
	   //Kill Them!!!
	   Con::printf("CA_Verify has denied this account certificate.");
	   Con::executef("MessageBoxOk", "Invalid Account", "Invalid Account Certificate");
	   Con::evaluatef("schedule(250, 0, disconnect);"); //Phantom139: Schedule to fix simObject crash.
	   return;
   }
   // give our client E/N in an appending sig to work with
   Con::setVariable("AuthenticationDetails", "");
   std::string worker;
   worker.assign(E);
   worker.append(N);
   Con::setVariable("AuthenticationDetails", worker.c_str());
   //give the client his signature too!
   Con::setVariable("AuthenticationSignature", "");
   std::string clientSig;
   clientSig.assign(S);
   Con::setVariable("AuthenticationSignature", clientSig.c_str());
   //
   Con::setVariable("AccountDetails", "");
   std::string hold;
   hold.assign(E);
   hold.append(":");
   hold.append(N);
   hold.append(":");
   hold.append(S);
   //Store it
   Con::setVariable("AccountDetails", hold.c_str());
   Con::printf("AccountDetails is stored.");
}

DefineEngineFunction(setPGDServer, void, (const char *server, const char *port),, "(string, string)") {
   cryptoPackage->setPGDServer(server, port);   
}
  
//Phantom139: New Stuff: cURL registration & recover
DefineEngineFunction(registerAccount, void, (const char *username, const char *password, const char *email, const char *key),, "(string, string, string, string)") {
	String result = cryptoPackage->registerAccount(username, password, email, key);
	if(strcmp(Con::getVariable("$DebugMode"), "1") == 0) {
		Con::printf("DEBUG: %s", result.c_str());
	}
}
   
DefineEngineFunction(recoverAccount, void, (const char *username, const char *password),, "(string, string)") {
	String result = cryptoPackage->recoverAccount(username, password);
	if(strcmp(Con::getVariable("$DebugMode"), "1") == 0) {
		Con::printf("DEBUG: %s", result.c_str());
	}   
}
#endif