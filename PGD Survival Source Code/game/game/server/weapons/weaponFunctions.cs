//weaponFunctions.cs
//Phantom139 (Robert Fritzen)
//[C] Phantom Games Development, 2012

$WeaponHeadshotBonus = 2; //yep...

//Player::switchWeapons
// * Change the player's gun
// %this - Obvious....
// %slot - Weapon Slot (1 - Primary, 2 - Secondary)
function Player::switchWeapons(%this, %slot) {
   if(%slot == %this.currentSlot) {
      return; //silly n0b...
   }
   switch(%slot) {
      case 1:
         if(%this.primaryWeapon $= "") {
            return;
         }
         %this.use(%this.primaryWeapon);
      case 2:
         if(%this.secondaryWeapon $= "") {
            return;
         }
         %this.use(%this.secondaryWeapon);
      default:
         error("Player::switchWeapons() - Attempting to access invalid slot");
         break;
   }
   %this.currentSlot = %slot;
   if(!%this.isAIControlled) {
      %this.client.updateWeaponHud();
   }
}

//GameConnection::updateWeaponHud()
// * Updates the weapon hud
// %this - Obvious....
function GameConnection::updateWeaponHud(%this) {
   %player = %this.getControlObject();
   %primary = %player.primaryWeapon;
   %secondary = %player.secondaryWeapon;
   //
   %Primary_item = nameToID(%primary);
   %Secondary_item = nameToID(%secondary);
   //
   %heldSlot = %player.currentSlot;
   if(%heldSlot == 1) {
      commandToClient(%this, 'updateWeaponHUD', DeTag(%Primary_item.PreviewImage), DeTag(%Secondary_item.PreviewImage));
      %this.updateAmmoImage();
   }
   else if(%heldSlot == 2) {
      commandToClient(%this, 'updateWeaponHUD', DeTag(%Secondary_item.PreviewImage), DeTag(%Primary_item.PreviewImage));
      %this.updateAmmoImage();
   }
   else {
      //ignore update
   }
}

//GameConnection::updateAmmoImage()
// * Updates the Little Ammo Images on the Weapon HUD
// %this - Obvious....
function GameConnection::updateAmmoImage(%this) {
   //get the held gun
   //%image = %this.getControlObject().getMountedImage($WeaponSlot);    //Glitches hud when ammo of equipment == 0
   %player = %this.getControlObject();
   %heldSlot = %player.currentSlot;
   %Primary_item = nameToID(%player.primaryWeapon);
   %Secondary_item = nameToID(%player.secondaryWeapon);
   if(%HeldSlot == 2) {
      %image = %Secondary_item.image;
   }
   else {
      %image = %Primary_item.image;
   }
   
   %ammoType = nameToID(%image).ammoImage;
   //get the clip size of the gun (if applicable)
   if(%image.isField("clip")) {
      %currentClip = %this.getControlObject().getInventory(%this.clip);
      %currentAmmo = %this.getControlObject().getInventory(%image.ammo);
      %ammoInClip = %player.maxInventory(%image.ammo);
      commandToClient(%this, 'updateAmmoCounter', %ammoType, %currentAmmo, %ammoInClip, %currentClip);
   }
   else {
      // Is this an energy weapon?
      if(%image.isEnergyWeapon) {
         commandToClient(%this, 'updateAmmoCounter', "E", "INF", "INF");
      }
      else {
         %currentAmmo = %this.getControlObject().getInventory(%image.ammo);
         %ammoInClip = %player.maxInventory(%image.ammo);
         commandToClient(%this, 'updateAmmoCounter', %ammoType, %currentAmmo, %ammoInClip, 0);
      }
   }
}

//GameConnection::setWeapons()
// * Set's and loads out a player (called on spawn)
// %this - Obvious
// %pri - Primary Weapon
// %sec - Secondary Weapon
function GameConnection::setWeapons(%this, %pri, %sec, %loadPri, %loadSec) {
   %player = %this.getControlObject();
   %player.primaryWeapon = %pri;
   %player.secondaryWeapon = %sec;

   %player.setInventory(%pri, 1);
	if(%loadPri) {
      %player.setInventory(%pri.image.clip, %player.maxInventory(%pri.image.clip));
      %player.setInventory(%pri.image.ammo, %player.maxInventory(%pri.image.ammo));    // Start the gun loaded
	}

   %player.setInventory(%sec, 1);
	if(%loadSec) {
      %player.setInventory(%sec.image.clip, %player.maxInventory(%sec.image.clip));
      %player.setInventory(%sec.image.ammo, %player.maxInventory(%sec.image.ammo));    // Start the gun loaded
	}
   
	if(!%player.currentSlot) {
      %player.use(%pri);
      %player.currentSlot = 1;
	}
	else {
		switch(%player.currentSlot) {
			case 1:
		      %player.use(%pri);
			case 2:
				%player.use(%sec);
		}
	}
   
   %this.updateWeaponHud();
}

function GameConnection::KillCheck(%client, %damageType) {
	eval("cancel("@%client@".feedReset"@%damageType@");");
   %client.damageFeed[%damageType]++;
	//echo("client damage feed: "@%client SPC %damageType@" = "@%client.damageFeed[%damageType]);

	//challenges!
	switch$(%damageType) {
		case "Disposition":
			if(%client.damageFeed[%damageType] >= 2) {
			   commandToClient(%client, 'addDispositionMultiKill');
            %client.damageFeed[%damageType] = 0;
			}

		case "Swarmer":
			if(%client.damageFeed[%damageType] >= 4 && %client.player.isBurstFire) {
			   commandToClient(%client, 'addSwarmerChainReaction');
			}
	}

	eval(""@%client@".feedReset"@%damageType@" = schedule(350, 0, clearDamageFeed, "@%client@", "@%damageType@");");
}

function clearDamageFeed(%client, %damageType) {
   %client.damageFeed[%damageType] = 0;
}

function serverCmdsendSpecializations(%client, %list) {
	for(%i = 0; %i < getFieldCount(%list); %i++) {
	   %client.spec[%i] = getField(%list, %i);
	}
}

function Player::spec(%this, %wID) {
   return %this.client.spec[%wID];
}

function Player::getWeaponSpread(%this, %weapon) {
	%client = %this.client;

	switch$(%weapon) {
		case "Ryder":
			%spread = "0.01";
			%wID = 0;

		case "Duke":
         %spread = "0.0095";
			%wID = 1;

		case "Lurker":
         %spread = "0.0175";
			%wID = 2;

		case "Disposition":
         %spread = "0.13";
			%wID = 3;

		case "Swarmer":
         %spread = "0.13";
			%wID = 4;

		case "Ender":
         %spread = "0.003";
			%wID = 5;

		case "Mamba":
         %spread = "0.001";
			%wID = 6;

		case "Kral":
         %spread = "0.0095";
			%wID = 7;

		case "Kraad":
         %spread = "0.015";
			%wID = 8;

		case "Kralmok":
         %spread = "0.003";
			%wID = 9;

		case "TeslaForcer":
         %spread = "0.008";
			%wID = 10;
	}

	if(%client.spec[%wID] $= "Accuracy I") {
	   %spread = %spread * 0.9;
	}
	if(%client.spec[%wID] $= "Accuracy II") {
	   %spread = %spread * 0.85;
	}
	if(%client.spec[%wID] $= "Accuracy III") {
	   %spread = %spread * 0.75;
	}

	if(%client.spec[%wID] $= "Mastery") {
	   %spread = %spread * 0.75;
	}

	return %spread;
}