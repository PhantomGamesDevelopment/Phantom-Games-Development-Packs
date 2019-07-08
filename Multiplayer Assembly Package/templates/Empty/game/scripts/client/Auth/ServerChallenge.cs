//ServerChallenge.cs
//Robert Fritzen (Phantom139)
//(c)Phantom Games Development 2011
//This script manages the clientside server authentication handshake

function clientCmdrequestClientCert() {
   if($DeveloperMode) {
      commandToServer('issueDeveloperMode');
      return;
   }

   %FullDetails = $ConnStore::guid@$ConnStore::username;
   GatherAccountDetails(%FullDetails, $ClientKey_E, $ClientKey_N, $ClientKey_S);

   %maxDetails = %FullDetails @ $AuthenticationDetails;
   $AuthenticationDetails = ""; //clear this field now to save more memory
   for(%i = 0; %i < strlen(%maxDetails); %i+= 128) {
      commandToServer('TransmitAccountDetails', getSubStr(%maxDetails, %i, 128));
   }

   for(%i = 0; %i < strlen($ConnStore::sig); %i+= 64) {
      commandToServer('TransmitAccountSignature', getSubStr($ConnStore::sig, %i, 64));
   }
   commandToServer('CertificateSent');
   $AuthenticationSignature = "";
   
   schedule(2000, 0, "CommandToServer", 'pingAuthInfo', $ConnStore::username TAB $ConnStore::guid TAB $ConnStore::email, $pref::Player::Name);
   //Server connection events proceed from here
}
