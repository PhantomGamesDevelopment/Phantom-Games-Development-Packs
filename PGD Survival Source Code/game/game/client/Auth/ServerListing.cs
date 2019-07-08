//ServerListing.cs
//Robert C. Fritzen (Phantom139)
//(c) Phantom Games Development, 2011
//This script manages the listing server query calls

$Master::CGList = ""; //Phantom139: SC Omit

$Master::ListMode = 2;
$_PERFORMINGLOOKUP = 0;

function QueryPGDMasterServer() {
   if(!$_PERFORMINGLOOKUP) {
      generateQueryServers();
   }
}

function generateQueryServers(%this) {
   $_PERFORMINGLOOKUP = 1;

   %sQTCP = new TCPObject(ServerQueryTCP);

   %separator = getRandomSeperator(16);
   %header = assembleHTTP1_1Header("www.phantomdev.net", $Master::CGList, "POST", "PGD Survival Client", "Content-Type: multipart/form-data; boundary="@%separator@"\r\n");
   %dispo = makeDisposition(%separator, mode, $Master::ListMode, 1);
   %header = %header @ "Content-Length: " @ strLen(%dispo) @ "\r\n\r\n";

   %sQTCP.request = %header @ %dispo;
   %sQTCP.connect("www.phantomdev.net:80");
}

function ServerQueryTCP::onConnected(%this) {
   %this.send(%this.request);
}

function ServerQueryTCP::onLine(%this, %line) {
   closeMessagePopup();
   if(strstr(%line, "$PGD") != -1) {
      %line = strReplace(%line, "$PGD", "");
      %response = stripchars(%line, "$\n");
      switch$(firstWord(%response)) {
         case "LIST":
				%list = strReplace(%response, "LIST ", "");
				for(%i = 0; %i < getWordCount(%list); %i++) {
               %server = getWord(%list, %i);
				   echo("Query Server: "@%server);
               querySingleServer( %server, 0 );
				}
            return;
      }
   }
}

function ServerQueryTCP::onDisconnect(%this) {
   $_PERFORMINGLOOKUP = 0;
   %this.delete();
}

function onQueryTimeOut(%address, %remove) {
   echo("Request to "@%address@" timed out, removal: "@%remove);
}
