//ClientChallenge.cs
//Robert Fritzen (Phantom139)
//(c)Phantom Games Development 2011
//This script manages the serverside client authentication handshake

function serverCmdTransmitAccountDetails(%client, %stream) {
   %client.certChk = %client.certChk @ %stream; //keep downloading.
}
function serverCmdTransmitAccountSignature(%client, %sig) {
   %client.sigChk = %client.sigChk @ %sig;
}

function serverCmdCertificateSent(%client) {
   //Ok, the client is expecting a really quick response from us, so lets make this
   //really quick. We verify the client here using the fields they have sent us
   %crt = %client.certChk;
   %client.verifyClientSignature(%client.sigChk, %crt);
   //if we haven't dropped the client yet, let the client send the rest to us!
}

//this is the final check, we ask for the challenge, and our auth (game info)
function serverCmdpingAuthInfo(%client, %authInfo, %name) {
   %client.doneAuthing = true;
   %client.authInfo = %authInfo;
   %client.onConnect(%name);
}

function GameConnection::getAuthenticationInfo(%client) {
   //username TAB guid TAB email
   return %client.authInfo;
}

function serverCmdissueDeveloperMode(%client) {
   //this command tells the server to bypass auth if and only if the server detects the client is the localhost
   if (%client.getAddress() $= "local") {
      %client.doneAuthing = true;
      %client.authInfo = "Developer\t0\t0";
      %client.onConnect("Developer");
   }
   else {
      %client.delete("The server has denied your developer mode request.");
   }
}
