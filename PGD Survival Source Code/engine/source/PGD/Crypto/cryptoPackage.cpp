/**
cryptoPackage.cpp
source cryptoPP function calls

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
**/

#include <fstream>
#include "cryptoPackage.h"
#include "../Solutions/tcpCurl.h"

xxz568 *cryptoPackage = NULL;
xxz568::xxz568() {}
xxz568::~xxz568() {}
void xxz568::create() {
   if(cryptoPackage == NULL) {
      cryptoPackage = new xxz568();
   }
}

void xxz568::destroy() {
   if(cryptoPackage != NULL) {
      delete cryptoPackage;
	  cryptoPackage = NULL;
   }
}

const char * xxz568::caPublic() {
   return "This Is Omitted Stuffs";
}

const char * xxz568::AESFileKey() { return "This is Omitted Stuffs"; }

std::string xxz568::sha1(std::string text) {
   CryptoPP::SHA1 hash;  

   std::string digest;
   // Thank you, Wei Dai, for making this possible:
   CryptoPP::StringSource foo(text, true,
      new CryptoPP::HashFilter(hash,
	     new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(digest), false)));

   return digest;
}

std::string xxz568::whirlpool(std::string text) {
   CryptoPP::Whirlpool hash;  

   std::string digest;
   // Thank you, Wei Dai, for making this possible:
   CryptoPP::StringSource foo(text, true,
      new CryptoPP::HashFilter(hash,
	     new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(digest), false)));

   return digest;
}

std::string xxz568::pgdHash(std::string text) {
   std::string innerLayer, outerLayer;
   innerLayer = sha1(text);
   outerLayer = whirlpool(innerLayer);
   return outerLayer;
}

//xxz568 AES Encryption/Decryption, Modifications for 1.1
//Addition of PBKDF, and AES fixes using SecByteBlock
//Credit to these modifications go to: Geoff Beier, geoffbeier@gmail.com
int xxz568::AESEncrypt(std::string key, std::string input, std::string &output, unsigned int PBKDFiterations) {

   std::string hashedKey = whirlpool(key);

   AutoSeededX917RNG<AES> rng;

   SecByteBlock iv(AES::BLOCKSIZE);
   rng.GenerateBlock(iv,iv.size());

   // See NIST SP 800-132 for detailed recommendations on length, generation and
   // format of the salt. This test program will just generate a random one. That
   // might not be sufficient for every application.
   SecByteBlock pwsalt(AES::DEFAULT_KEYLENGTH);
   rng.GenerateBlock(pwsalt,pwsalt.size());

   SecByteBlock derivedkey(AES::DEFAULT_KEYLENGTH);
   PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
   pbkdf.DeriveKey(
      // buffer that holds the derived key
	  derivedkey, derivedkey.size(),
	   // purpose byte. unused by this PBKDF implementation.
	   0x00,
	   // password bytes. careful to be consistent with encoding...
	   (byte *) hashedKey.data(), hashedKey.size(),
	   // salt bytes
	   pwsalt, pwsalt.size(),
	   // iteration count. See SP 800-132 for details. You want this as large as you can tolerate.
	   // make sure to use the same iteration count on both sides...
	   PBKDFiterations
   );
   std::string ciphertext;

   CBC_Mode<AES>::Encryption aesencryption(derivedkey,derivedkey.size(),iv);
   // encrypt message using key derived above, storing the hex encoded result into ciphertext
   StringSource encryptor(input, true,
      new StreamTransformationFilter(aesencryption, new HexEncoder( new StringSink(ciphertext), false))
   );
   std::string hexsalt, hexiv;
   ArraySource saltEncoder(pwsalt,pwsalt.size(), true, new HexEncoder(new StringSink(hexsalt), false));
   ArraySource ivEncoder(iv,iv.size(), true, new HexEncoder(new StringSink(hexiv), false));

   output.assign(hexsalt);
   output.append(hexiv);
   output.append(ciphertext);
   //
   return 1;
}

int xxz568::AESDecrypt(std::string key, std::string input, std::string &output, unsigned int PBKDFiterations) {

   std::string hashedKey = whirlpool(key);

   std::string saltPull, ivPull, cipherPull, store;
   store.assign(input);
   saltPull.assign(store.substr(0, 32));
   store.assign(store.substr(32, store.length()));
   ivPull.assign(store.substr(0, 32));
   store.assign(store.substr(32, store.length()));
   cipherPull.assign(store.substr(0, store.length()));

   // now recover the plain text given the password, salt, IV and ciphertext
   PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
   SecByteBlock recoveredkey(AES::DEFAULT_KEYLENGTH);
   SecByteBlock recoveredsalt(AES::DEFAULT_KEYLENGTH);
   StringSource saltDecoder(saltPull,true,new HexDecoder(new ArraySink(recoveredsalt, recoveredsalt.size())));
   pbkdf.DeriveKey(recoveredkey, recoveredkey.size(), 0x00, (byte *) hashedKey.data(), hashedKey.size(),
      recoveredsalt, recoveredsalt.size(), PBKDFiterations);
   SecByteBlock recoverediv(AES::BLOCKSIZE);
   StringSource ivDecoder(ivPull,true,new HexDecoder(new ArraySink(recoverediv, recoverediv.size())));
   try {
      CBC_Mode<AES>::Decryption aesdecryption(recoveredkey, recoveredkey.size(), recoverediv);
      StringSource decryptor(cipherPull, true, new HexDecoder(
         new StreamTransformationFilter(aesdecryption, new StringSink(output))
      ));
   }
   catch(CryptoPP::Exception e) {
	  output = "Decryption Error: ";
	  output.append(e.GetWhat());
   }
   return 1;
}

int xxz568::HexEncode(std::string input, std::string &output) {
   CryptoPP::StringSource foo(input, true,
	  new CryptoPP::HexEncoder(
         new CryptoPP::StringSink(output), false));
   return 1;
}

int xxz568::HexDecode(std::string input, std::string &output) {
   CryptoPP::StringSource foo(input, true,
	  new CryptoPP::HexDecoder(
         new CryptoPP::StringSink(output)));
   return 1;
}

unsigned int xxz568::getUTC() {
   unsigned int uiTime = static_cast<unsigned int>( time( NULL ) );
   return uiTime;
}

int xxz568::Base64Encode(std::string input, std::string &output) {
   StringSink* sink = new StringSink(output);
   Base64Encoder* base64_enc = new Base64Encoder(sink);
   StringSource source(input, true, base64_enc);
   return 1;
}

int xxz568::Base64Decode(std::string input, std::string &output) {
   StringSink* sink = new StringSink(output);
   Base64Decoder* base64_dec = new Base64Decoder(sink);
   StringSource source(input, true, base64_dec);
   return 1;
}

std::string xxz568::IntegerToString(const Integer refs) {
   std::ostrstream oss;
   oss << refs;
   oss << std::hex << refs; // use this for hex output
   std::string s(oss.str());
   s = s.substr(0, s.find_first_of("."));
   return s;// output is now in s
}

bool xxz568::caVerify(std::string message, std::string signature) {
   std::string dec, fin;
   fin.assign(caPublic());
   fin.append("h");
   Integer rsaPub(fin.c_str()), rsaexp("65537");
   
   std::string holder;

   try {
      RSASSA_PKCS1v15_SHA_Verifier verifier;
      verifier.AccessKey().Initialize(rsaPub, rsaexp);

      HexDecode(signature, holder);

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

const char * xxz568::iToCC(int in) {
   char newVal[sizeof(int)];
	_itoa(in, newVal, 10);
	return newVal;
}

CryptoPP::Integer xxz568::load(std::string inbound, int r) {
   std::string move;
   move.assign(inbound);
   switch(r) {
	  case 10:
	     break;
	  case 16:
	     move.append("h");
		 break;
	  default:
	     break;
   }
   CryptoPP::Integer hold(move.c_str());
   return hold;
}


/**
 AES FILE ENCRYPTION CYCLES
**/
int xxz568::AESEnc_File(std::string key, std::string path, unsigned int PBKDFiterations, std::string outFile) {
	if(outFile.compare("") == 0) {
	   outFile = path;
	}
   std::string hashedKey = whirlpool(key);

   AutoSeededX917RNG<AES> rng;

   SecByteBlock iv(AES::BLOCKSIZE);
   rng.GenerateBlock(iv,iv.size());

   // See NIST SP 800-132 for detailed recommendations on length, generation and
   // format of the salt. This test program will just generate a random one. That
   // might not be sufficient for every application.
   SecByteBlock pwsalt(AES::DEFAULT_KEYLENGTH);
   rng.GenerateBlock(pwsalt,pwsalt.size());

   SecByteBlock derivedkey(AES::DEFAULT_KEYLENGTH);
   PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
   pbkdf.DeriveKey(
      // buffer that holds the derived key
	  derivedkey, derivedkey.size(),
	   // purpose byte. unused by this PBKDF implementation.
	   0x00,
	   // password bytes. careful to be consistent with encoding...
	   (byte *) hashedKey.data(), hashedKey.size(),
	   // salt bytes
	   pwsalt, pwsalt.size(),
	   // iteration count. See SP 800-132 for details. You want this as large as you can tolerate.
	   // make sure to use the same iteration count on both sides...
	   PBKDFiterations
   );

   CBC_Mode<AES>::Encryption aesencryption(derivedkey,derivedkey.size(),iv);
   std::string hexsalt, hexiv;
   ArraySource saltEncoder(pwsalt,pwsalt.size(), true, new HexEncoder(new StringSink(hexsalt), false));
   ArraySource ivEncoder(iv,iv.size(), true, new HexEncoder(new StringSink(hexiv), false));

   std::string storeThat, fileInputData, fileOut;
   std::ifstream inData;
	inData.clear();
   inData.open(path.c_str());
   while(!inData.eof()) {
	  std::getline(inData, storeThat);
	  fileInputData.append(storeThat).append("\n");
	  storeThat = "";
   }

   StringSource encryptor(fileInputData, true,
      new StreamTransformationFilter(aesencryption, new HexEncoder( new StringSink(fileOut), false))
   );

   std::ofstream fileEdit(outFile.c_str(), std::ios::out);
	fileEdit << "Phantom Games Development (AES-256-CBC): " << path.c_str() << "\n";
   fileEdit << hexsalt << hexiv;
   fileEdit << fileOut;
   fileEdit.close();
   //
   return 1;
}

int xxz568::AESDec_File(std::string key, std::string path, unsigned int PBKDFiterations, std::string outFile) {
	if(outFile.compare("") == 0) {
	   outFile = path;
	}

   std::string hashedKey = whirlpool(key);
   std::string store, saltPull, ivPull, cipherData, outData;
   
   std::ifstream inReader;//(path.c_str(), std::ios::in);
	inReader.clear();
	inReader.open(path.c_str(), std::ios::in);
	//skip the first line
	std::getline(inReader, store);
	store = ""; //empty it...
	//
   std::getline(inReader, store); //read the first line (encryption data);
 
   saltPull.assign(store.substr(0, 32));
   store.assign(store.substr(32, store.length()));
   ivPull.assign(store.substr(0, 32));
	store.assign(store.substr(32, store.length()));
	cipherData.append(store);
   //if anything is left, grab it.
   while(!inReader.eof()) {
     std::getline(inReader, store);
	  cipherData.append(store);
	  store = "";
   }
	inReader.close();
   // now recover the plain text given the password, salt, IV and ciphertext
   PKCS5_PBKDF2_HMAC<SHA256> pbkdf;
   SecByteBlock recoveredkey(AES::DEFAULT_KEYLENGTH);
   SecByteBlock recoveredsalt(AES::DEFAULT_KEYLENGTH);
   StringSource saltDecoder(saltPull,true,new HexDecoder(new ArraySink(recoveredsalt, recoveredsalt.size())));
   pbkdf.DeriveKey(recoveredkey, recoveredkey.size(), 0x00, (byte *) hashedKey.data(), hashedKey.size(),
      recoveredsalt, recoveredsalt.size(), PBKDFiterations);
   SecByteBlock recoverediv(AES::BLOCKSIZE);
   StringSource ivDecoder(ivPull,true,new HexDecoder(new ArraySink(recoverediv, recoverediv.size())));
   try {
      CBC_Mode<AES>::Decryption aesdecryption(recoveredkey, recoveredkey.size(), recoverediv);
      StringSource decryptor(cipherData, true, new HexDecoder(
         new StreamTransformationFilter(aesdecryption, new StringSink(outData))
      ));
   }
   catch(CryptoPP::Exception e) {
	  cipherData = "Decryption Error: ";
	  cipherData.append(e.GetWhat());
   }

   //_unlink(path.c_str());
   std::ofstream fileReplace(outFile.c_str(), std::ios::out); //overwrite existing.
   fileReplace << outData << std::endl;
   fileReplace.close();

   return 1;
}

const char *xxz568::getUserPath(const char *platform) {
	if(strcmp(platform, "windows") == 0) {
	   const char *appPath = getenv("LOCALAPPDATA");
		if(strcmp(appPath, "") == 0) {
		   //winXP or below
			const char *appPath = getenv("APPDATA");
		}
		//add a trail to it.
		strcat((char *)appPath, "/");
		return appPath;
	}
	else {
	   const char *appPath = getenv("HOME");
		if(strcmp(platform, "macos") == 0) {
		   strcat((char *)appPath, "/Library/Application Support/");
		}
		else {
		   strcat((char *)appPath, "/.");
		}
		return appPath;
	}
}

bool xxz568::isEncryptedFile(const char *path) {
   //open the file and run the first line through to see if it contains the tag
	std::string line;
	std::fstream fileo(path, std::ios::in);
	std::getline(fileo, line);
	fileo.close();

	if(line.substr(0, 40).compare("Phantom Games Development (AES-256-CBC):") == 0) {
	   return true;
	}
	return false;
}

//------------------------------
void xxz568::setPGDServer(String server, String authPath) {
   connectedServer = server;
	authenticationPath = authPath;
	Con::printf("* Connected To Server: %s", server.c_str());
}

const char *xxz568::getServer() {
	String s = connectedServer + authenticationPath;
	return s.c_str();
}

void xxz568::generateRSAKey() {
   AutoSeededRandomPool rng;

   while(accountRSA.Validate(rng, 3) == false) {
      accountRSA.GenerateRandomWithKeySize(rng, 512);
      //parameters.SetPublicExponent(65537);
      RSA::PrivateKey privateKey(accountRSA);
      RSA::PublicKey publicKey(accountRSA);
   }
	Con::printf("* RSA Key Generated");
}

String xxz568::recoverAccount(const char *username, const char *password) {
	unsigned int utc = cryptoPackage->getUTC();
	char utcTime[64];
	_itoa(utc, utcTime, 10);

	std::string time = std::string(utcTime);
	time = time.substr(0, time.length() - 3);
	//
	std::string sha1 = cryptoPackage->sha1(time);
	//
	std::string input = username;
	input.append(password);
	input.append("2.71828");
	std::string hashed = pgdHash(input);

	std::string sendResult = sha1.append(hashed);
	std::string data = "authMode=3&request=";
	data.append(username);
	data.append("&hashsend=");
	data.append(sendResult);

	if(strcmp(Con::getVariable("$DebugMode"), "1") == 0) {
		Con::printf("DEBUG:\nServer: %s\nRequest: %s", getServer(), data.c_str());
	}

	String recover = tcpBP->readURL(getServer(), data.c_str());
	String FirstWord = StringUnit::getUnit( recover, 0, " \t\n");
	if(FirstWord.compare("$PGD$INCORRECT_PASS") == 0 || FirstWord.compare("$PGD$NO_SUCH_ACCOUNT") == 0) {
	   Con::executef("MessageBoxOK", "Error", "Invalid Username or Password");
	}
	else if(FirstWord.compare("$PGD$RECVR") == 0) {
		recover = recover.replace("$PGD$RECVR ", "");
		recover = recover.replace("[NL]", " ");
		String un = StringUnit::getUnit( recover, 0, " \t\n");
		String guid = StringUnit::getUnit( recover, 1, " \t\n");
		String email = StringUnit::getUnit( recover, 2, " \t\n");
		String rsae = StringUnit::getUnit( recover, 3, " \t\n");
		String rsan = StringUnit::getUnit( recover, 4, " \t\n");
		String rsasig = StringUnit::getUnit( recover, 5, " \t\n");
		String hash = StringUnit::getUnit( recover, 6, " \t\n");
		String rsad = StringUnit::getUnit( recover, 7, " \t\n");

		writeCertificate(un, guid, email, rsae, rsan, rsasig, hash, rsad);
		Con::executef("MessageBoxOK", "Success", "Account Has Been Recovered! You may now log in.");
	}
	else {
		Con::executef("MessageBoxOK", "Error", "Account Server Returned an Unrecognized Command\nPlease send console.log to development team if problem persists.");
		Con::errorf("recoverAccount() - Unrecognized Command: %s", FirstWord.c_str());
	}

	return recover;
}

String xxz568::registerAccount(const char *username, const char *password, const char *email, const char *key) {
   generateRSAKey();   //create our RSA Keypair

	std::string e = IntegerToString(accountRSA.GetPublicExponent()).c_str();
	std::string n = IntegerToString(accountRSA.GetModulus()).c_str();
	std::string d = IntegerToString(accountRSA.GetPrivateExponent()).c_str();

	std::string input = username;
	input.append(password);
	input.append("2.71828");
	std::string hashed = pgdHash(input);

	std::string decHash = pgdHash(d);
	std::string aesKey = password;
	aesKey.append(decHash);

	std::string outD;
   try {
      AESEncrypt(aesKey, d, outD, 1024);
   }
   catch(CryptoPP::Exception e) {
	   Con::errorf("encryptAccountKey: Failed, %s", e.GetWhat().c_str());
		return "ENCRYPT_FAIL";
   }
	//-----------------------------------------------------------
	std::string payload = username;
	payload.append("\t");
	payload.append(email);
	payload.append("\t");
	payload.append(pgdHash(key));
	payload.append("\t");
	payload.append(outD);
	payload.append("\t");
	payload.append(n);
	payload.append("\t");
	payload.append(e);
	payload.append("\t");
	payload.append(decHash);
	payload.append("\t");
	payload.append(hashed);

	std::string data = "authMode=2&fullPayload=";
	data.append(payload);

	if(strcmp(Con::getVariable("$DebugMode"), "1") == 0) {
		Con::printf("DEBUG:\nServer: %s\nRequest: %s", getServer(), data.c_str());
	}

	String reg = tcpBP->readURL(getServer(), data.c_str());
   String FirstWord = StringUnit::getUnit( reg, 0, " \t\n");
	if(FirstWord.compare("$PGD$BAD_NAME") == 0) {
	   Con::executef("MessageBoxOK", "Error", "The server has denied your username\nPlease check for invalid characters or words.");
	}
	else if(FirstWord.compare("$PGD$NAME_TAKEN") == 0) {
	   Con::executef("MessageBoxOK", "Error", "This username has already been registered.");
	}
	else if(FirstWord.compare("$PGD$NO_EMAIL") == 0) {
	   Con::executef("MessageBoxOK", "Error", "Your email field is missing.");
	}
	else if(FirstWord.compare("$PGD$BAD_EMAIL") == 0) {
	   Con::executef("MessageBoxOK", "Error", "You have typed an invalid email address.");
	}
	else if(FirstWord.compare("$PGD$NO_KEY") == 0) {
	   Con::executef("MessageBoxOK", "Error", "Your account key field is missing.");
	}
	else if(FirstWord.compare("$PGD$INVALID_KEY") == 0) {
	   Con::executef("MessageBoxOK", "Error", "You have typed an invalid account key.");
	}
	else if(FirstWord.compare("$PGD$EMAIL_NOT_BOUND") == 0) {
	   Con::executef("MessageBoxOK", "Error", "This account key is not assigned to your email address.\nCheck both the key and email address and try again.");
	}
	else if(FirstWord.compare("$PGD$SIGN_FAILURE") == 0) {
	   Con::executef("MessageBoxOK", "Error", "The account server has failed to generate a valid RSA Signature.\nPlease Try Again, if the problem persists contact the support team.");
	}
	else if(FirstWord.compare("$PGD$SIGN_ERROR") == 0) {
	   Con::executef("MessageBoxOK", "Error", "The account server encountered a RSA Signature Error.\nPlease Try Again.");
	}
	else if(FirstWord.compare("$PGD$INTERNAL_ERROR") == 0) {
	   Con::executef("MessageBoxOK", "Error", "The server has encountered an internal error.\nPlease try again later.");
	}
	else if(FirstWord.compare("$PGD$CERT") == 0) {
		reg = reg.replace("$PGD$CERT ", "");
		String un = StringUnit::getUnit( reg, 0, "\t\n");
		String guid = StringUnit::getUnit( reg, 1, "\t\n");
		String rsasig = StringUnit::getUnit( reg, 2, "\t\n");

		writeCertificate(un, guid, email, e.c_str(), n.c_str(), rsasig, decHash.c_str(), outD.c_str());
		Con::executef("MessageBoxOK", "Registration Successful", "Account Registration Sucessful, you may now log in!");
	}
	else {
		Con::executef("MessageBoxOK", "Error", "Account Server Returned an Unrecognized Command\nPlease send console.log to development team if problem persists.");
		Con::errorf("registerAccount() - Unrecognized Command: %s", FirstWord.c_str());
	}

	return reg;
}

void xxz568::writeCertificate(String n, String g, String e, String rsa_e, String rsa_n, String rsa_s, String hash, String rsa_d) {
	//setup the buffer.
	String newBuffer = "  <AccountData>\n";
	newBuffer += "    <Account>";
	newBuffer += n;
	newBuffer += "</Account>\n";
	newBuffer += "    <AccountGUID>";
	newBuffer += g;
	newBuffer += "</AccountGUID>\n";
	newBuffer += "    <AccountEMAIL>";
	newBuffer += e;
	newBuffer += "</AccountEMAIL>\n";
	newBuffer += "    <AccountRSA>";
	newBuffer += rsa_e;
	newBuffer += ":";
	newBuffer += rsa_n;
	newBuffer += ":";
	newBuffer += rsa_s;
	newBuffer += "</AccountRSA>\n";
	newBuffer += "    <AccountRSAP>";
	newBuffer += hash;
	newBuffer += ":";
	newBuffer += rsa_d;
	newBuffer += "</AccountRSAP>\n";
	newBuffer += "  </AccountData>";
	//

	FileObject *fObj = new FileObject();
	fObj->registerObject();

	String filePath = cryptoPackage->getUserPath(Con::getVariable("$platform"));
	//get the full path
	if(strcmp(Con::getVariable("$appDataDirName"), "") == 0) {
	   filePath += Con::getVariable("$appName");
	}
	else {
	   filePath += Con::getVariable("$appDataDirName");	
	}
	filePath += "/accounts/";
	//check if the dir exists, otherwise add it.
	if(!Torque::FS::IsDirectory(filePath)) {
		Platform::createPath(filePath.c_str());
	}
	//set the correct file to access.
	filePath += "certs.xml";
	if(Platform::isFile(filePath.c_str())) {
		Con::printf("File Exists..");
	   //existing certificate file
		// -Step 1: Read Existing File.
		String fileBuffer, cLine;
		fObj->readMemory(filePath.c_str());
		bool canCopy = false;
		while(!fObj->isEOF()) {
			cLine = (const char *)(fObj->readLine());
			if(cLine.trim().compare("<AccountData>") == 0) {
			   canCopy = true;
			}
			//
			if(canCopy) {
			   fileBuffer = fileBuffer + cLine;
				fileBuffer += "\n";
			}
			//
			if(cLine.trim().compare("</AccountData>") == 0) {
			   canCopy = false;
			}
		}
		fObj->close();
		fObj->openForWrite(filePath.c_str(), false);
		// -Step 2: Re-make the Buffer
		fObj->writeLine((const U8 *)"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>");
		fObj->writeLine((const U8 *)"<AccountList>");
		fObj->writeLine((const U8 *)fileBuffer.c_str());
		fObj->writeLine((const U8 *)newBuffer.c_str());
		fObj->writeLine((const U8 *)"</AccountList>");
	}
	else {
		Con::printf("Writing New Cert. File");
	   //new certificate file
      fObj->openForWrite(filePath.c_str(), false);
      //write some generic XML necessary crapz0rs
		fObj->writeLine((const U8 *)"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>");
		fObj->writeLine((const U8 *)"<AccountList>");
		fObj->writeLine((const U8 *)newBuffer.c_str());
		fObj->writeLine((const U8 *)"</AccountList>");
	}
	fObj->close();
	fObj->destroySelf();
}