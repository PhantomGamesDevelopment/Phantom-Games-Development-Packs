//TCPConnections.cs
//Robert Fritzen
//[c] Phantom Games Development, 2012
//Handles all of the TCP Object connections.

//Generic Methods & Globals
$TCP::Mode::ServerTest = 1;
//$TCP::Mode::Register = 2;
//$TCP::Mode::Recover = 3;

$TCP::ActiveMode = 0;

$TCP::DefaultTimeout = 15000;

function TCPAuthConnection::onConnected(%this) {
   if($debugMode) {
      echo("Request: \n"@%this.request);
   }
   //delay it shortly so C++ can finish up the needed methods.
   %this.schedule(500, send, %this.request);
}

function TCPAuthConnection::onLine(%this, %line) {
   if(isEventPending($TCP::TransferTimeout)) {
      cancel($TCP::TransferTimeout);
      $TCP::TransferTimeout = schedule(700, 0, Authentication_transferTimeout);
   }

   if(trim(%line) !$= "") {
      %this.canRead = true;
   }
   if(%this.canRead) {
      if($debugMode) {
         echo("DEBUG: "@%line);
      }
      if(strstr(%line, "$") != -1) {
         $TCP::Buffer[$TCP::ActiveMode] = $TCP::Buffer[$TCP::ActiveMode] @ "\n" @ %line;
      }
   }
}

function Authentication_transferTimeout() {
   TCPAuthConnection.disconnect();
   //access and clean up the buffer
   %buffer = trim($TCP::Buffer[$TCP::ActiveMode]);
   %buffer = strReplace(%buffer, "$PGD", "");
   %buffer = stripchars(%buffer, "$\n");
   //handle the task based on the mode
   switch($TCP::ActiveMode) {
      //Server Test
      case $TCP::Mode::ServerTest:
         if(%buffer $= "SERVER_REPLY") {
            %server_index = $TCP::Variables["index"];

				setPGDServer($Authentication::ServerList[%server_index], $Authentication::AuthTestPort[%server_index]);
				$TCP::AuthServer = $Authentication::ServerList[%server_index];
            $TCP_ASI = %server_index;
         }
   }
   //clean the buffer and set the mode to 0
   $TCP::Buffer[$TCP::ActiveMode] = "";
   $TCP::ActiveMode = 0;
}

//==========================================================================================================================================
//TCP_TestServer: TCPObject for server test
function TCP_TestServer(%host, %path, %index) {
   if($TCP::AuthServer !$= "") {
      //already got a server, kill the other requests.
      return;
   }
   if($TCP::ActiveMode != 0) {
      schedule(10000, 0, TCP_TestServer, %host, %path, %index);
      return;
   }

   if(isObject(TCPAuthConnection)) {
      TCPAuthConnection.delete();
   }
   new TCPObject(TCPAuthConnection);
   //Store TCP Vars
   $TCP::Variables["index"] = %index;
   //Set the active mode
   $TCP::ActiveMode = $TCP::Mode::ServerTest;
   //Assemble HTTP POST Request
   %separator = getRandomSeperator(16);
   %header = assembleHTTP1_1Header(%host, %path, "POST", "PGD XXZ568 Client", "Content-Type: multipart/form-data; boundary="@%separator@"\r\n");
   %dispo = makeDisposition(%separator, authMode, "4", 1);
   %header = %header @ "Content-Length: " @ strLen(%dispo) @ "\r\n\r\n";
   %request = %header @ %dispo;
   //Send.
   TCPAuthConnection.request = %request;
   TCPAuthConnection.connect(%host @":80");
   $TCP::TransferTimeout = schedule($TCP::DefaultTimeout, 0, Authentication_transferTimeout);
}

//==========================================================================================================================================
//==========================================================================================================================================
//TCP_PerformRegister: TCPObject for account registration
function TCP_PerformRegister(%username, %password, %email, %key) {
   registerAccount(%username, %password, %email, %key);
}

//==========================================================================================================================================
//==========================================================================================================================================
//TCP_DoLoginRecovery: TCPObject for login recovery
function TCP_DoLoginRecovery(%username, %password) {
   recoverAccount(%username, %password);
}