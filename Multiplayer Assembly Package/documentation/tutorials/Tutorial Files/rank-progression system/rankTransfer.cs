//rankTransfer.cs
$Rank::Host = "www.somewebsite.com";
$Rank::Path = "/path/to/rankUpload.php";

function downloadRank() {
   $Rank::Buffer = "";
   if(isObject(TCPRankDownload)) {
      TCPRankDownload.delete();
   }
   %t = new TCPObject(TCPRankDownload);
   %separator = getRandomSeperator(16);
   %header = assembleHTTP1_1Header($Rank::Host, $Rank::Path, "POST", "PGD XXZ568 Client", "Content-Type: multipart/form-data; boundary="@%separator@"\r\n");
   //GET request to server
   %dispo = makeDisposition(%separator, guid, $ConnStore::guid);
   %dispo = %dispo @ makeDisposition(%separator, cert, $ConnStore::sig);
   %dispo = %dispo @ makeDisposition(%separator, data, "GET", 1);
   %header = %header @ "Content-Length: " @ strLen(%dispo) @ "\r\n\r\n";
   %request = %header @ %dispo;
   //Send.
   %t.request = %request;
   %t.connect(%host @":80");
}

function uploadRank() {
   $Rank::Buffer = "";
   %guid = $ConnStore::guid;
   %file = GetUserDataPath() @"save/"@%guid@"/data.xmle"; //where you save your rank files at
   
   AESEncFile(%file, %guid, "save.xmle");
   %fo = new FileObject();
   %fo.openForRead("save.xmle");
   while(!%fo.isEof()) {
      %fCont = %fCont @ %fo.readLine();
   }
   %fo.close();
   %fo.delete();
   
   deleteFile("save.xmle");

   if(isObject(TCPRankDownload)) {
      TCPRankDownload.delete();
   }
   %t = new TCPObject(TCPRankDownload);
   %separator = getRandomSeperator(16);
   %header = assembleHTTP1_1Header($Rank::Host, $Rank::Path, "POST", "PGD XXZ568 Client", "Content-Type: multipart/form-data; boundary="@%separator@"\r\n");
   //GET request to server
   %dispo = makeDisposition(%separator, guid, %guid);
   %dispo = %dispo @ makeDisposition(%separator, cert, $ConnStore::sig);
   %dispo = %dispo @ makeDisposition(%separator, data, %fCont, 1);
   %header = %header @ "Content-Length: " @ strLen(%dispo) @ "\r\n\r\n";
   %request = %header @ %dispo;
   //Send.
   %t.request = %request;
   %t.connect(%host @":80");
}

function TCPRankDownload::onConnected(%this) {
   //delay it shortly so C++ can finish up the needed methods.
   %this.schedule(500, send, %this.request);
}

function TCPRankDownload::onLine(%this, %line) {
   if(trim(%line) !$= "") {
      %this.canRead = true;
   }
   if(%this.canRead) {
      if(strstr(%line, "$") != -1) {
         $Rank::Buffer = $Rank::Buffer @ "\n" @ %line;
      }
   }
}

function TCPRankDownload::onDisconnect(%this) {
   %this.delete();
   if(firstWord($Rank::Buffer) $= "$MAP$INTERNAL_ERROR") {
      echo("Rank Download: The server encountered an internal error.");
   }
   else if(firstWord($Rank::Buffer) $= "$MAP$INVALID_ACCOUNT") {
      echo("Rank Download: Invalid account certificate sent.");
   }
   else if(firstWord($Rank::Buffer) $= "$MAP$NO_DATA_ON_SERVER") {
      echo("Rank Download: No Rank Data.");
   }
   else if(firstWord($Rank::Buffer) $= "$MAP$UPLOAD_FAIL") {
      echo("Rank Download: Rank Upload Failed, contact server admin.");
   }
   else if(firstWord($Rank::Buffer) $= "$MAP$UPLOAD_SUCCESS") {
      echo("Rank Download: Rank Upload Success.");
   }
   else if(firstWord($Rank::Buffer) $= "$MAP$DOWNLOAD") {
      //save the new file.
      %data = strReplace($Rank::Buffer, "$MAP$DOWNLOAD ", "");
      %guid = $ConnStore::guid;
      %file = GetUserDataPath() @"save/"@%guid@"/data.xmle"; //where you save your rank files at
      
      %fo = new FileObject();
      %fo.openForWrite("save.xmle");
      %fo.writeLine(%data);
      %fo.close();
      %fo.delete();
      
      AESDecFile("save.xmle", %guid, %file);
      deleteFile("save.xmle");
      echo("Rank Download: Rank Download Success.");
   }
}
