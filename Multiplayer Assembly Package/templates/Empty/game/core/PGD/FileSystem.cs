// retrieves the absolute path to the game's writable file storage directory
function GetUserDataPath() {
   %separator = "/";
   // retrieve the path to where programs should store their writable files
   if($platform $= "windows") {
      // try getting the local application data path (typically available only on Vista and above)
      %path = getEnvironmentVariable("LOCALAPPDATA");
      // if it failed then we're on Windows XP or below
      if(%path $= "") {
         // get Application Data path, note that this is not the Local Settings variant
         %path = getEnvironmentVariable("APPDATA");
      }
      %path = %path @ %separator;
   }
   else {
      // get home path
      %path = getEnvironmentVariable("HOME");

      if($platform $= "macos") {
         %path = %path @ "/Library/Application Support/";
      }
      else {
         %path = %path @ "/.";
      }
   }
   // append game specific directory name
   if($appDataDirName $= "") {
      %path = %path @ $appName @ %separator;
   }
   else {
      %path = %path @ $appDataDirName @ %separator;
   }
   // create the directory if it doesn't exist
   // Note: createPath() only accepts '/' (not '\') as a trailing slash
   if(!IsDirectory(%path)) {
      createPath(%path);
   }
   // return the full path to the game's writable directory
   return %path;
}
