//playerLoadouts.cs
//Robert Fritzen - AFPSK
//Client-Side of the custom loadout system
// NOTE: There's a LOT of stuff in here, it could just as easily be split into multiple script files based
//  on the categories below. IF you want to do that, make sure to execute the new files in initialize_afpsk_client();

//==============================================================================
//==============================================================================
//  Asset Functions
//==============================================================================
//==============================================================================


// Credit for the below C++ / TS Functions go to Nathan Martin.
// http://www.garagegames.com/community/forums/viewthread/131068/1#comment-831510
// retrieves the absolute path to the game's writable file storage directory
//NOTE: YOU NEED TO ADD THE FOLLOWING TO THE SOURCE CODE:
//DefineConsoleFunction( getEnvironmentVariable, const char*, (const char *variableName),,
//                "@brief Returns the value of the requested operating system's environment variable.\n"
//                "@param variableName Name of the environment variable\n"
//                "@return String value of the requested environment variable.\n"
//                "@ingroup Console") {
//    // get requested environment variable value
//    const char *result = getenv(variableName);
//
//    // verify successful operation
//    if(result) {
//       // success, allocate return space and return the result
//       char *ret = Con::getReturnBuffer(dStrlen(result) +1);
//       dStrcpy(ret, result);
//       return ret;
//    }
//    // fail, environment variable doesn't exist
//    return "";
//}

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

//==============================================================================
//==============================================================================
//  Auto-Load
//==============================================================================
//==============================================================================
function initialize_afpsk_client() {
   if(!$AFPSK::Client::Loaded) {
      exec("art/gui/ClassCreator.gui");
      exec("art/gui/InGame_ClassSelect.gui");
      exec("art/gui/WeaponHUD.gui");
      $AFPSK::Client::Loaded = 1;
   }
}
initialize_afpsk_client();

//==============================================================================
//==============================================================================
//  Globals, Constants
//==============================================================================
//==============================================================================
$LoadoutSystem::XMLPath = "game/xml/"; //Make sure to set this
$LoadoutSystem::SaveDataPath = GetUserDataPath() @ "saveData/MultiLoadouts.xml";  //MAP Owners: Remove this and use GUID instead (IE: GetUserDataPath() @ %guid % "/path/";)
$LoadoutSystem::MaxClasses = 10; //How many custom classes do players get?
$LoadoutSystem::GuiPopulated = 0;  //Don't Touch
$LoadoutSystem::FirstBook = ""; //Don't Touch
$LoadoutSystem::WHTag = "art/gui/weaponHud/"; //Where weapon images are stored (this will be used for icons)
$LoadoutSystem::WHTag_Gun = $LoadoutSystem::WHTag @ "hud_guns/"; //and where the gun icons are stored
//Default Loadout Options, Be sure to add custom fields, ect, here
$LoadoutDefaults::PrimaryGun = "Lurker";
$LoadoutDefaults::SecondaryGun = "Ryder";
$LoadoutDefaults::Equipment = "ProxMine";
//Loadout Object - Used by a few of our functions
$LoadoutSystem::Object = isObject(CustomLoadouts) ? CustomLoadouts : new ScriptObject(CustomLoadouts);

//==============================================================================
//==============================================================================
//  Package Overloads
//==============================================================================
//==============================================================================
//You are more than welcome to just apply these changes directly to your code, this is just here in the
// event that you don't want to have to change the existing code.
package afpsk_UpdatedSystem_Client {
   function PlayGui::onWake(%this) {
      Parent::onWake(%this);
      //Unload the old Weapon Hud
      if(isObject(WeaponHUD)) {
         WeaponHUD.delete();
      }
      Canvas.pushDialog(NewWeaponHUD);
   }
   
   function PlayGui::onSleep(%this) {
      Parent::onSleep(%this);
      //Added call for the new HUD
      Canvas.popDialog(NewWeaponHUD);
   }
   
   function MainMenuGui::onWake(%this) {
      Parent::onWake(%this);
      loadPlayerLoadouts();
   }
   
   function clientCmdSetAmmoAmountHud(%amount, %amountInClips) {
      //Nope...
   }

   function clientCmdRefreshWeaponHUD(%amount, %preview, %ret, %zoomRet, %amountInClips) {
      //Nope 2.0...
   }
};
activatePackage(afpsk_UpdatedSystem_Client);
//==============================================================================
//==============================================================================
//  GUI Code - Class Creator
//==============================================================================
//==============================================================================
function ClassCreator::onWake(%this) {
   //Handle general OnWake tasks first.
   classList.clear();

   for(%i = 0; %i < $LoadoutSystem::MaxClasses; %i++) {
      %data = $LoadoutSystem::Object.customClass[%i];
      %className = getField(%data, 0);
      //add it to the line
      classList.addRow(%i, %className);
   }

   classList.setSelectedRow(0);
   //Populate the GUI if it needs to be.
   if($LoadoutSystem::GuiPopulated) {
      SEL_MainTabBook.selectPage(0);
      $LoadoutSystem::FirstBook.selectPage(0);
      return;
   }
   $LoadoutSystem::GuiPopulated = 1;
   %MainBook = SEL_MainTabBook;
   //first load up our XML data for the loadout selector
   %xml = new SimXMLDocument() {};
   %xml.loadFile($LoadoutSystem::XMLPath @ "ClientLoadoutSelector.xml");

   //And start constructing the GUI using the items in the list
   %xml.pushChildElement("LOS");
   %xml.pushFirstChildElement("Class");
   while(true) {
      %category = %xml.attribute("category");
      %display = %xml.attribute("IGN");
      
      %newCategory = new GuiTabPageCtrl("SEL_"@%category);
      %newCategory.position = "0 20";
      %newCategory.extent = getWord(%MainBook.extent, 0) @" "@ getWord(%MainBook.extent, 0) - 20;
      %newCategory.text = %display;
      %MainBook.add(%newCategory);
      
      %newBook = new GuiTabBookCtrl("SEL_BOOK_"@%category);
      %newBook.extent = %newCategory.extent;
      %newCategory.add(%newBook);
      if($LoadoutSystem::FirstBook $= "") {
         $LoadoutSystem::FirstBook = %newBook;
      }

      %xml.pushFirstChildElement("SubCategory");
      while(true) {
         %subCategory = %xml.attribute("category");
         %subDisplay = %xml.attribute("IGN");

         %newSubCategory = new GuiTabPageCtrl("SEL_SUBCAT_"@%subCategory);
         %newSubCategory.position = "0 20";
         %newSubCategory.extent = getWord(%newBook.extent, 0) @" "@ getWord(%newBook.extent, 0) - 20;
         %newSubCategory.text = %subDisplay;
         %newBook.add(%newSubCategory);
         
         //NOTE! If your gun counter extends about 10 for a sub-class, you should implement code here to place the below in a
         //GuiScrollCtrl otherwise you'll have buttons you cannot see.

         %xml.pushFirstChildElement("Item");
         %itemCounter = 0;
         while (true) {
            %xml_item = %xml.getData();
            %itemName = %xml.attribute("IGN");
            //Create the selector button, TO ALL, Customize this code to change the look of the selection buttons.
            %offset = "3 5"; //Offset of all buttons from the tab page.
            %button = new GuiButtonCtrl("Select"@%xml_item);
            %button.text = %itemName;
            %button.position = getWords(vectorAdd("0 "@((%itemCounter * 30) + (%itemCounter == 0 ? 0 : 10)), %offset), 0, 1);
            %button.extent = "150 30";
            %button.command = "selectItem("@%xml_item@");";
            %newSubCategory.add(%button);
            
            %itemCounter++;
            if (!%xml.nextSiblingElement("Item")) break;
         }
         %xml.popElement();
         if (!%xml.nextSiblingElement("SubCategory")) break;
      }
      %xml.popElement();
      if (!%xml.nextSiblingElement("Class")) break;
   }
   SEL_MainTabBook.selectPage(0);
   $LoadoutSystem::FirstBook.selectPage(0);
}

function ClassCreator::saveLoadout(%this) {
   //This button acts as a "save the current" loadout option
   %file = $LoadoutSystem::SaveDataPath;
   %los = $LoadoutSystem::Object;
   if(!isObject(%los)) {
      error("saveLoadout() - No Loadout System Object Found.");
      return;
   }
   //Get the name of the class:
   %name = className.getText();
   if(%name $= "" || getFieldCount(%name) > 1) {
      MessageBoxOk("Invalid Name", "You have entered an invalid class name");
      return;
   }
   //Get the slot
   %slot = classList.getSelectedRow();
   if(%slot $= "") {
      %slot = 0;
   }
   //And apply the new class:
   updateSingle(%slot, %name, getField(%los.customclass[%slot], 1), getField(%los.customclass[%slot], 2), getField(%los.customclass[%slot], 3));
   //Then save it.
   SaveMPLoadouts(%file);
   //Apply GUI Changes
   classList.clear();

   for(%i = 0; %i < $LoadoutSystem::MaxClasses; %i++) {
      %data = %los.customclass[%i];
      %className = getField(%data, 0);
      //add it to the line
      classList.addRow(%i, %className);
   }

   classList.setSelectedRow(%slot);
}

function classList::onSelect(%this, %row, %text) {
   %los = $LoadoutSystem::Object;
   //update the bottom control with the current items
   CSCName.setText("<Font:Arial:18><just:center>"@%text);
   CSC_PrimaryWeapon.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField(%los.customclass[%row], 1))@".png");
   CSC_SecondaryWeapon.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField(%los.customclass[%row], 2))@".png");
   CSC_Equipment.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField(%los.customclass[%row], 3))@".png");
}

//==============================================================================
//==============================================================================
//  GUI Code - Weapon Hud
//==============================================================================
//==============================================================================
function clientCmdupdateWeaponHUD(%name, %pImg, %sImg, %eImg) {
   //Make visible/invisible the ones without items
   if(%pImg $= "") {
      WH_Primary.setVisible(false);
   }
   else {
      WH_Primary.setVisible(true);
   }
   //
   if(%sImg $= "") {
      WH_Secondary.setVisible(false);
   }
   else {
      WH_Secondary.setVisible(true);
   }
   //
   if(%eImg $= "") {
      WH_Equipment.setVisible(false);
   }
   else {
      WH_Equipment.setVisible(true);
   }
   // No guns?
   if(%pImg $= "" && %sImg $= "") {
      WeaponName.setVisible(false);
      AmmoImageString.setVisible(false);
   }
   else {
      WeaponName.setVisible(true);
      AmmoImageString.setVisible(true);
   }
   //
   WH_Primary.setBitmap($LoadoutSystem::WHTag_Gun @ %pImg);
   WH_Secondary.setBitmap($LoadoutSystem::WHTag_Gun @ %sImg);
   WH_Equipment.setBitmap($LoadoutSystem::WHTag_Gun @ %eImg);
   WeaponName.setText("<Just:Center><Font:Arial:14><Color:FFFFFF>"@%name);
}

function clientCmdupdateEquipmentCounter(%amount) {
   if(%amount <= 0) {
      WH_Equipment.setVisible(false);
      EquipmentCounter.setVisible(false);
   }
   else {
      WH_Equipment.setVisible(true);
      EquipmentCounter.setVisible(true);
      EquipmentCounter.setText("<Font:Arial:14><Just:Center><Color:FFFFFF>"@%amount);
   }
}

function clientCmdupdateAmmoCounter(%type, %current_ammo, %clip_size, %clip_count) {
   if(%type $= "E") {
      AmmoImageString.setVisible(false);
      return;
   }
   else {
      AmmoImageString.setVisible(true);
   }

   if(%type !$= $lastWeaponType) {
      $lastWeaponType = %type;

      %first = $LoadoutSystem::WHTag @ "bullet_";
      %next = %first @ %type;
      %realAmmo = %next @"_O.png";
      %usedAmmo = %next @"_U.png";

      AmmoImageString.setBitmap(%realAmmo);
      AmmoImageString.setCutoffBitmap(%usedAmmo);
   }

   AmmoImageString.setImageCount(%clip_size);
   AmmoImageString.setCutoffPoint(%current_ammo);

   if(%clip_count <= 0) {
      ClipCount.setVisible(false);
   }
   else {
      ClipCount.setVisible(true);
      ClipCount.setText("<Just:Center><Font:Arial:12><Color:FFFFFF>"@%clip_count);
   }
}

//==============================================================================
//==============================================================================
//  GUI Code - In-Game Class Selector
//==============================================================================
//==============================================================================
function InGame_ClassSelect::onWake(%this) {
   if($lastload $= "") {
      $lastload = classSelectionList.getSelectedRow();
      if($lastload $= "" || $lastload == -1) {
         classSelectionList.setSelectedRow(0);
         $lastload = 0;
         commandToServer('playeruseloadout', $lastload);
         SelectedCCName.setText("<Font:Arial:18><just:center><Color:FFFFFF>"@getField($clientUseableLoadout[$lastload], 0));
         CCN_Primary.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 1)) @".png");
         CCN_Secondary.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 2)) @".png");
         CCN_Equipment.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 3)) @".png");
         return;
      }
   }
   SelectedCCName.setText("<Font:Arial:18><just:center><Color:FFFFFF>"@getField($clientUseableLoadout[$lastload], 0));
   CCN_Primary.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 1)) @".png");
   CCN_Secondary.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 2)) @".png");
   CCN_Equipment.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 3)) @".png");
}

function clientCmdRequestLoadouts() {
   echo("Client Request for Loadouts recieved:");
   for(%i = 0; %i < $LoadoutSystem::MaxClasses; %i++) {
      commandToServer('transmitLoadout', %i, $LoadoutSystem::Object.customclass[%i]);
   }
}

function clientCmdDropLoadout(%i, %data) {
   $clientUseableLoadout[%i] = %data;
   classSelectionList.addRow(%i, getField(%data, 0));
}

function clientCmdSetIGCSMode(%mode) {
   switch(%mode) {
      case 0:
         IGCC_Container.setHidden(false);
         IGCC_noGui_Container.setHidden(true);
      case 1:
         IGCC_Container.setHidden(true);
         IGCC_noGui_Container.setHidden(false);
   }
}

function clientCmdSetNLIGCS(%p, %s, %e) {
   if(%p $= "") {
      Predef_pri.setVisible(false);
   }
   else {
      Predef_pri.setVisible(true);
      Predef_pri.setBitmap($LoadoutSystem::WHTag_Gun @ %p @".png");
   }

   if(%s $= "") {
      Predef_sec.setVisible(false);
   }
   else {
      Predef_sec.setVisible(true);
      Predef_sec.setBitmap($LoadoutSystem::WHTag_Gun @ %s @".png");
   }

   if(%e $= "") {
      Predef_eqi.setVisible(false);
   }
   else {
      Predef_eqi.setVisible(true);
      Predef_eqi.setBitmap($LoadoutSystem::WHTag_Gun @ %e @".png");
   }
}

function clientCmdPushIGCS(%resTime) {
   canvas.pushDialog(InGame_ClassSelect);

   classSelectionList.setSelectedRow(0);
}

function useNewLoadoutCommand() {
   $lastload = classSelectionList.getSelectedRow();
   commandToServer('playeruseloadout', $lastload);

   SelectedCCName.setText("<Font:Arial:18><just:center><Color:FFFFFF>"@getField($clientUseableLoadout[$lastload], 0));
   CCN_Primary.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 1)) @".png");
   CCN_Secondary.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 2)) @".png");
   CCN_Equipment.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField($clientUseableLoadout[$lastload], 3)) @".png");
   
   MessageHud.AddLine("\c3Your default spawn loadout has changed.");
   Canvas.popDialog(InGame_ClassSelect);
}

//==============================================================================
//==============================================================================
//  Loadout System
//==============================================================================
//==============================================================================
function loadClass(%name, %primary, %secondary, %equipment) {
   %los = $LoadoutSystem::Object;
   
   //Attain the last entry.
   for(%i = 0; %los.customclass[%i] !$= ""; %i++) {}
   %los.customclass[%i] = %name @"\t"@ %primary @"\t"@ %secondary @"\t"@ %equipment;
}

function updateSingle(%slot, %name, %primary, %secondary, %equipment) {
   if(%slot < 0 || %slot > $LoadoutSystem::MaxClasses-1) {
      error("updateSingle() - Cannot update out of class range");
      return;
   }
   
   %los = $LoadoutSystem::Object;
   %los.customclass[%slot] = %name @"\t"@ %primary @"\t"@ %secondary @"\t"@ %equipment;
}

function setClassItem(%finalCategory, %item) {
   //This is the code that actually applies the loadout option
   %los = $LoadoutSystem::Object;
   %slot = classList.getSelectedRow();
   if(%slot $= "") {
      %slot = 0;
   }
   //get current data
   %current_class = %los.customclass[%slot];
   //set the class items: Be sure to add all of your custom selections here!
   switch$(%finalCategory) {
      case "PrimaryWeapons":
         %los.customclass[%slot] = setField(%current_class, 1, %item);
      case "SecondaryWeapons":
         %los.customclass[%slot] = setField(%current_class, 2, %item);
      case "Equipment":
         %los.customclass[%slot] = setField(%current_class, 3, %item);
   }
   //Update the gui
   CSCName.setText("<Font:Arial:18><just:center>"@getField(%los.customclass[%slot], 0));
   CSC_PrimaryWeapon.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField(%los.customclass[%slot], 1))@".png");
   CSC_SecondaryWeapon.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField(%los.customclass[%slot], 2))@".png");
   CSC_Equipment.setBitmap($LoadoutSystem::WHTag_Gun @ strlwr(getField(%los.customclass[%slot], 3))@".png");
}

//==============================================================================
//==============================================================================
//  Saving / Loading
//==============================================================================
//==============================================================================
function loadPlayerLoadouts() {     //Call this at the game's load up
   %file = $LoadoutSystem::SaveDataPath;
   if(!isFile(%file)) {
      WriteNewMPLoadouts(%file);
   }
   else {
      LoadMPLoadouts(%file);
   }
}

function WriteNewMPLoadouts(%filePath) {
   for(%i = 0; %i < $LoadoutSystem::MaxClasses; %i++) {
      %LName = "Custom Loadout "@%i+1;
      loadClass(%LName, $LoadoutDefaults::PrimaryGun, $LoadoutDefaults::SecondaryGun, $LoadoutDefaults::Equipment);
   }
   SaveMPLoadouts(%filePath);
}

function SaveMPLoadouts(%FilePath) {
   %los = $LoadoutSystem::Object;

   %fileO = new FileObject();
   %fileO.openForWrite(%filePath);
   %fileO.writeLine("<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"yes\" ?>");
   %fileO.writeLine("<MPLoadouts>");
   %spa = "    ";
   for(%i = 0; %i < $LoadoutSystem::MaxClasses; %i++) {
      %fileO.writeLine("  <Group name=\"Loadout"@%i+1@"\">");
      %fileO.writeLine(%spa@"<Inventory name=\"LoadoutName\">"@getField(%los.customclass[%i], 0)@"</Inventory>");
      %fileO.writeLine(%spa@"<Inventory name=\"WeaponSlot1\">"@getField(%los.customclass[%i], 1)@"</Inventory>");
      %fileO.writeLine(%spa@"<Inventory name=\"WeaponSlot2\">"@getField(%los.customclass[%i], 2)@"</Inventory>");
      %fileO.writeLine(%spa@"<Inventory name=\"Equipment\">"@getField(%los.customclass[%i], 3)@"</Inventory>");
      %fileO.writeLine("  </Group>");
   }
   %fileO.writeLine("</MPLoadouts>");
   %fileO.close();
   %fileO.delete();
}

function LoadMPLoadouts(%FilePath) {
   if(!isFile(%FilePath)) {
      error("Critical: Attempting file load on non-existant MP Loadout");
      error("Forcing call to WriteNewMPLoadouts()");
      WriteNewMPLoadouts(%FilePath);
      return;
   }
   %loadout = 1;
   %xml = new SimXMLDocument() {};
   %xml.loadFile(%FilePath);

   %xml.pushChildElement("MPLoadouts");
   %xml.pushFirstChildElement("Group");
   while(true) {

      %xml.pushFirstChildElement("Inventory");
      while (true) {
         %id = %xml.attribute("name");
         %desc = %xml.getData();
         //load the data into a readable format
         switch$(%id) {
            case "LoadoutName":
               %Lname = %desc;
            case "WeaponSlot1":
               %wep1 = %desc;
            case "WeaponSlot2":
               %wep2 = %desc;
            case "Equipment":
               %equip = %desc;
         }

         if (!%xml.nextSiblingElement("Inventory")) break;
      }
      loadClass(%Lname, %wep1, %wep2, %equip);

      %xml.popElement();
      if (!%xml.nextSiblingElement("Group")) break;

      %loadout++;
   }
   %xml.delete();
}

//==============================================================================
//==============================================================================
//  Updated Client Binds
//==============================================================================
//==============================================================================
function useEquipment(%val) {
   if (%val) {
      commandToServer('playeruseEquipment');
   }
}
moveMap.bind(keyboard, e, useEquipment);

function callLoadoutGUI(%val) {
   if (%val) {
      commandToServer('RequestLoadoutSelector');
   }
}
moveMap.bind(keyboard, m, callLoadoutGUI);

//==============================================================================
//==============================================================================
//  XML Interfacing
//==============================================================================
//==============================================================================
function checkItemUse(%item) {
   //load the XML data for the item requirements
   %xml = new SimXMLDocument() {};
   %xml.loadFile($LoadoutSystem::XMLPath @ "ItemRequirements.xml");
   //
   %pass = 0;
   //now we need to locate the selected item and it's category scheme
   %xml.pushChildElement("LOS");
   %xml.pushFirstChildElement("Item");
   while(true) {
      %name = %xml.attribute("name");
      %req = %xml.getData();

      if(%name $= %item) {
         //Add all of your checks here.
         switch$(%req) {
            //case "beta":
            //   %pass = 0;
            //   MessageBoxOk("Cannot Use", "This item is locked in the beta.");
            //   break;
            case "none":
               %pass = 1;
               break;
            default:
               %pass = 0;
               break;
         }
         %found = 1;
      }
      if(%found) {
         break;
      }

      if (!%xml.nextSiblingElement("Item")) break;
   }

   if(%pass) {
      return true;
   }
   return false;
}

function selectItem(%item) {
   //first load up our XML data for the loadout selector
   %xml = new SimXMLDocument() {};
   %xml.loadFile($LoadoutSystem::XMLPath @ "ClientLoadoutSelector.xml");

   //now we need to locate the selected item and it's category scheme
   %xml.pushChildElement("LOS");
   %xml.pushFirstChildElement("Class");
   while(true) {
      %category = %xml.attribute("category");

      %xml.pushFirstChildElement("SubCategory");
      while(true) {
         %subCategory = %xml.attribute("category");  //not really used ATM, but I might at some point...

         %xml.pushFirstChildElement("Item");
         while (true) {
            %xml_item = %xml.getData();
            if(%xml_item $= %item) {
               //perform all checks now
               if(checkItemUse(%item)) {
                  //we found the item!
                  %finalCategory = %category;
                  %finalSubCategory = %subCategory;
                  break;
               }
               else {
                  return; //kill it here.
               }
            }
            if (!%xml.nextSiblingElement("Item")) break;
         }
         if(%finalCategory !$= "") {
            break;
         }
         %xml.popElement();
         if (!%xml.nextSiblingElement("SubCategory")) break;
      }

      if(%finalCategory !$= "") {
         break;
      }
      %xml.popElement();
      if (!%xml.nextSiblingElement("Class")) break;
   }

   if(%finalCategory $= "") {
      error("Invalid Item Error! No Category Found!");
      return;
   }
   //This is where we actually send the new item to our loadout
   setClassItem(%finalCategory, %item);
}
