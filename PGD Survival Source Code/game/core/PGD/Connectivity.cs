//Auth/Connectivity.cs
//Robert Fritzen (Phantom139)
//(c)Phantom Games Development 2012
//MODIFIED 11/27/2012: The purpose of this file is not longer to sercive the TCPConnectionList, which has been retired due to extensive
//amount of Access Violations, it now services individual TCPObject connections in general.

//Add servers to test here, ServerList is the direct HTML to the main page, AuthTestPort is the beyond path to index.php of the auth core of the project.
$Authentication::ServerList[0] = "";   //Phantom139: Soruce Code Omit
$Authentication::AuthTestPort[0] = ""; //Phantom139: Soruce Code Omit

$TCP::GenericServerPort = 80;
$TCP::AuthServer = "";
$TCP::AuthPath = "";
$TCP_ASI = "";

function getRandomSeperator(%length) {
   %alphanumeric = "abcdefghijklmnopqrstuvwxyz0123456789";
   for(%i = 0; %i < %length; %i++) {
      %index = getRandom(strLen(%alphanumeric));
      %letter = getSubStr(%alphanumeric, %index, 1);
      %UpperC = getRandom(0, 1);
      if(%UpperC) {
         %letter = strUpr(%letter);
      }
      else {
         %letter = strLwr(%letter);
      }
      %seq = %seq @ %letter;
   }

   return %seq;
}

function makeDisposition(%_sep, %_name, %_content, %_isEnd) {
   if(%_isEnd) {
      %dispo = "--" @ %_sep @ "\r\nContent-Disposition: form-data; name=\""@%_name@"\"\r\n\r\n"@%_content@"\r\n--" @ %_sep @ "--";
   }
   else {
      %dispo = "--" @ %_sep @ "\r\nContent-Disposition: form-data; name=\""@%_name@"\"\r\n\r\n"@%_content@"\r\n";
   }
   return %dispo;
}

function assembleHTTP1_1Header(%_host, %_page, %_command, %_userAgent, %_extra) {
   %header = %_command SPC %_page SPC "HTTP/1.1\r\n" @
                    "Host: "@%_host@"\r\n" @
                    "User-Agent: "@%_userAgent@"\r\n" @
                    "Connection: close\r\n";
	if(%_extra !$= "") {
	   %header = %header @ %_extra;
	}
   return %header;
}
