//serverRegister.cs
//Robert Fritzen (Phantom139)
//(c)Phantom Games Development 2011
//This script handles the master server list addition and re-addition

$Master::CGList = "/master/Survival/";

function startHeartbeat() {
   echo("Sending Request to Master Server");
   %sRTCP = new TCPObject(ServerRegisterTCP);

   %separator = getRandomSeperator(16);
   %header = assembleHTTP1_1Header("www.phantomdev.net", $Master::CGList, "POST", "PGD Survival Client", "Content-Type: multipart/form-data; boundary="@%separator@"\r\n");
   %dispo = makeDisposition(port, $Pref::Server::Port, 0);
   %dispo = %dispo @ makeDisposition(%separator, mode, 1, 1);
   %header = %header @ "Content-Length: " @ strLen(%dispo) @ "\r\n\r\n";

   %sRTCP.request = %header @ %dispo;
   %sRTCP.connect("www.phantomdev.net:80");

   $serverHeartbeat = schedule(3 * 60000, 0, "startHeartbeat");
}

function stopHeartbeat() {
   cancel($serverHeartbeat);
   echo("Canceling Master Server Request");
}

function ServerRegisterTCP::onConnected(%this) {
   %this.send(%this.request);
}

function ServerRegisterTCP::onLine(%this, %line) {
   closeMessagePopup();
   if(strstr(%line, "$PGD") != -1) {
      %line = strReplace(%line, "$PGD", "");
      %response = stripchars(%line, "$\n");
      switch$(firstWord(%response)) {
         case "REGISTER":
            echo("* Server Registered to Master Server");
         case "UPDATE":
            echo("* Server Updated on Master Server");
      }
   }
}

function ServerRegisterTCP::onDisconnect(%this) {
   %this.delete();
}
