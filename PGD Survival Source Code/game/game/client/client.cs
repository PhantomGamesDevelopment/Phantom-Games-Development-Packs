//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Server Admin Commands
//-----------------------------------------------------------------------------

function SAD(%password)
{
   if (%password !$= "")
      commandToServer('SAD', %password);
}

function SADSetPassword(%password)
{
   commandToServer('SADSetPassword', %password);
}

//----------------------------------------------------------------------------
// Misc server commands
//----------------------------------------------------------------------------

function clientCmdSyncClock(%time)
{
   // Time update from the server, this is only sent at the start of a mission
   // or when a client joins a game in progress.
}

//-----------------------------------------------------------------------------
// Numerical Health Counter
//-----------------------------------------------------------------------------

function clientCmdSetNumericalHealthHUD(%curHealth)
{
   // Skip if the hud is missing.
   if (!isObject(numericalHealthHUD))
      return;

   // The server has sent us our current health, display it on the HUD
   numericalHealthHUD.setValue(%curHealth);

   // Ensure the HUD is set to visible while we have health / are alive
   if (%curHealth)
      HealthHUD.setVisible(true);
   else
      HealthHUD.setVisible(false);
}

//-----------------------------------------------------------------------------
// Damage Direction Indicator
//-----------------------------------------------------------------------------

function clientCmdSetDamageDirection(%direction)
{
   eval("%ctrl = DamageHUD-->damage_" @ %direction @ ";");
   if (isObject(%ctrl))
   {
      // Show the indicator, and schedule an event to hide it again
      cancelAll(%ctrl);
      %ctrl.setVisible(true);
      %ctrl.schedule(500, setVisible, false);
   }
}

//-----------------------------------------------------------------------------
// Teleporter visual effect
//-----------------------------------------------------------------------------

function clientCmdPlayTeleportEffect(%position, %effectDataBlock)
{
   if (isObject(%effectDataBlock))
   {
      new Explosion()
      {
         position = %position;
         dataBlock = %effectDataBlock;
      };
   }
}

// ----------------------------------------------------------------------------
// WeaponHUD
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
// Vehicle Support
// ----------------------------------------------------------------------------

function clientCmdtoggleVehicleMap(%toggle)
{
   if(%toggle)
   {
      moveMap.pop();
      vehicleMap.push();
   }
   else
   {
      vehicleMap.pop();
      moveMap.push();
   }
}

// ----------------------------------------------------------------------------
// Turret Support
// ----------------------------------------------------------------------------

// Call by the Turret class when a player mounts or unmounts it.
// %turret = The turret that was mounted
// %player = The player doing the mounting
// %mounted = True if the turret was mounted, false if it was unmounted
function turretMountCallback(%turret, %player, %mounted)
{
   //echo ( "\c4turretMountCallback -> " @ %mounted );

   if (%mounted)
   {
      // Push the action map
      turretMap.push();
   }
   else
   {
      // Pop the action map
      turretMap.pop();
   }
}






//Weapon EXP Modifier Code
//Connect to PGD to download the EXP Mod Rate
//Script Kiddies Be Warned, this system is secured via RSA-4092 Signature Verification... so don't even bother..
function downloadEXPModifier() {
   if(isObject(ERDownload)) {
      ERDownload.delete();
   }
   %http = new HTTPObject(ERDownload);
   
   %http.get("", ""); //Phantom139: Path Omitted.
}

function ERDownload::onConnected(%this) {
   //do nothing.
}

function ERDownload::onLine(%this, %line) {
   if(strstr(%line, "$PGD") != -1) {
      %line = strReplace(%line, "$PGD", "");
      %response = stripchars(%line, "$\n");
      //add the news to the news control
      %rate = getWord(%response, 1);
	   %sig = getWord(%response, 2);

		setEXPModifier(%rate, %sig);
   }

	if(getEXPModifier() > 1) {
	   %newRate = getEXPModifier();
		EXPEventIndic.setVisible(true);
		EXPEventIndic.tooltip = "EXP Event is Live! (Rate: "@%newRate@")";
	}
}

function ERDownload::onDisconnect(%this) {
   %this.delete();
}
