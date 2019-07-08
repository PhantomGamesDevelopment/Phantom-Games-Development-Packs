//weaponSystem.cs
//Robert C. Fritzen

//Player::switchWeapons
// * Change the player's gun
// %this - Obvious....
// %slot - Weapon Slot (1 - Primary, 2 - Secondary, 3 - Equipment)
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
      case 3:
         if(%this.equipment $= "") {
            return;
         }
         %this.use(%this.equipment);
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
   %equipment = %player.equipment;
   //
   %Primary_item = nameToID(%primary);
   %Secondary_item = nameToID(%secondary);
   %Equipment_item = nameToID(%equipment);
   //
   %heldSlot = %player.currentSlot;
   //echo("updateWeaponHud: "@%primary SPC %secondary SPC %equipment);
   if(%heldSlot == 1) {
      //echo("serv: "@%primary@" => "@%Primary_item@" "@%Primary_item.image.getHeldName() @" "@ DeTag(%Primary_item.PreviewImage));
      commandToClient(%this, 'updateWeaponHUD', %Primary_item.image.getHeldName(), DeTag(%Primary_item.PreviewImage), DeTag(%Secondary_item.PreviewImage), DeTag(%Equipment_item.PreviewImage));
      %this.updateAmmoImage();
   }
   else if(%heldSlot == 2) {
      commandToClient(%this, 'updateWeaponHUD', %Secondary_item.image.getHeldName(), DeTag(%Secondary_item.PreviewImage), DeTag(%Primary_item.PreviewImage), DeTag(%Equipment_item.PreviewImage));
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
      %ammoInClip = %image.ammo.maxInventory;
      commandToClient(%this, 'updateAmmoCounter', %ammoType, %currentAmmo, %ammoInClip, %currentClip);
   }
   else {
      // Is this an energy weapon?
      if(%image.isEnergyWeapon) {
         commandToClient(%this, 'updateAmmoCounter', "E", "INF", "INF");
      }
      else {
         %currentAmmo = %this.getControlObject().getInventory(%image.ammo);
         %ammoInClip = %image.ammo.maxInventory;
         commandToClient(%this, 'updateAmmoCounter', %ammoType, %currentAmmo, %ammoInClip, 0);
      }
   }
   //Update the equipment hud
   %equipment = %this.getControlObject().equipment;
   %amount = %this.getControlObject().getInventory(%equipment);
   commandToClient(%this, 'updateEquipmentCounter', %amount);
}

//GameConnection::setWeapons()
// * Set's and loads out a player (called on spawn)
// %this - Obvious
// %pri - Primary Weapon
// %sec - Secondary Weapon
// %equ - Equipment
function GameConnection::setWeapons(%this, %pri, %sec, %equ) {
   %player = %this.getControlObject();
   %player.primaryWeapon = %pri;
   %player.secondaryWeapon = %sec;
   %player.equipment = %equ;

   %player.setInventory(%pri, 1);
   %player.setInventory(%pri.image.clip, %player.maxInventory(%pri.image.clip));
   %player.setInventory(%pri.image.ammo, %player.maxInventory(%pri.image.ammo));    // Start the gun loaded

   %player.setInventory(%sec, 1);
   %player.setInventory(%sec.image.clip, %player.maxInventory(%sec.image.clip));
   %player.setInventory(%sec.image.ammo, %player.maxInventory(%sec.image.ammo));    // Start the gun loaded

   %player.setInventory(%equ, %player.maxInventory(%equ));
   
   %player.use(%pri);
   %player.currentSlot = 1;
   
   //echo("setWeapons: "@%this SPC %pri SPC %sec SPC %equ);
   %this.updateWeaponHud();
}

//WeaponImage::getHeldName()
// * Gets the full held name of the item
// %this - Obvious enough
function WeaponImage::getHeldName(%this) {
   %item = %this.item;
   %heldName = %item.pickUpName;
   //Attachments!
   // * TODO: Code this portion when ready.
   //End!
   return %heldName;
}

//GameCore::getLoadout()
// * Gets a Game's Pre-Defined Loadout for %index, if one exists
function GameCore::getLoadout(%this, %index) {
   return %this.predefLoadout[%index];
}

//Package updatedSystem: I could have had you just edit the file, but using a package
// allows you to keep the existing code while simply applying the new code.
package afpsk_UpdatedSystem_Server {
   function serverCmdCycleWeapon(%client, %dir) {
      %player = %client.getControlObject();
      //
      if(%player.currentSlot == 1) {
         %player.switchWeapons(2);
      }
      else {
         %player.switchWeapons(1);
      }
   }

   function GameConnection::onConnect(%client, %name) {
      Parent::onConnect(%client, %name);
      //Send the loadout request
      commandToClient(%client, 'RequestLoadouts');
   }

   function GameCore::onClientEnterGame(%game, %client) {
      Parent::onClientEnterGame(%game, %client);
      //
      if(%game.noLoadouts) {
         commandToClient(%client, 'SetIGCSMode', 1);
         if(%game.predefLoadout[0] !$= "") {
            %PDL = %game.predefLoadout[0];
            commandToClient(%client, 'SetNLIGCS', getField(%PDL, 0), getField(%PDL, 1), getField(%PDL, 2));
         }
      }
      else {
         commandToClient(%client, 'SetIGCSMode', 0);
      }
   }
   
   function GameCore::loadOut(%game, %player) {
      if(%player.client.loadoutIndex $= "") {
         %player.client.loadoutIndex = 0;
      }
      //
      if(%game.customLoadoutsDefined || %game.noLoadouts) {
         %loadout = %game.getLoadout(%player.client.loadoutIndex);
         %pri = getField(%loadout, 0);
         %sec = getField(%loadout, 1);
         %equ = getField(%loadout, 2);
      }
      else {
         %loadout = %player.client.loadoutData[%player.client.loadoutIndex];
         %pri = getField(%loadout, 1);
         %sec = getField(%loadout, 2);
         %equ = getField(%loadout, 3);
      }
      //===============================
      %player.client.setWeapons(%pri, %sec, %equ);
   }

   function GameConnection::RefreshWeaponHud(%this, %amount, %preview, %ret, %zoomRet, %amountInClips) {
      //Stubbed Out
   }
   
   function GameConnection::setAmmoAmountHud(%this, %amount, %amountInClips ) {
      //Stubbed Out
   }

   function WeaponImage::onUnmount(%this, %obj, %slot) {
      if (%obj.client !$= "" && !%obj.isAiControlled) {
         %obj.client.lastItem = %this;
         %obj.client.updateWeaponHud();
      }
   }

   function Ammo::onInventory(%this, %obj, %amount) {
      // The ammo inventory state has changed, we need to update any
      // mounted images using this ammo to reflect the new state.
      for (%i = 0; %i < 8; %i++) {
         if ((%image = %obj.getMountedImage(%i)) > 0) {
            if (isObject(%image.ammo) && %image.ammo.getId() == %this.getId()) {
               %obj.setImageAmmo(%i, %amount != 0);
               %currentAmmo = %obj.getInventory(%this);
               if (%obj.getClassname() $= "Player") {
                  if ( isObject( %this.clip ) ) {
                     %amountInClips = %obj.getInventory(%this.clip);
                     %amountInClips *= %this.maxInventory;
                     %amountInClips += %obj.getFieldValue( "remaining" @ %this.getName() );
                  }
                  else {
                     %amountInClips = %currentAmmo;
                     %currentAmmo = 1;
                  }
                  //Phantom139: The following code is modified regarding the hud calls.
                  if (%obj.client !$= "" && !%obj.isAiControlled) {
                     %obj.client.updateAmmoImage();
                  }
                  //%obj.client.setAmmoAmountHud(%currentAmmo, %amountInClips);
               }
            }
         }
      }
   }
   
   function ProxMine::onInventory( %this, %obj, %amount ) {
      if ( !%amount && ( %slot = %obj.getMountSlot( %this.image ) ) != -1 )
         %obj.switchWeapons(1);

      if (%obj.client !$= "" && !%obj.isAiControlled && %amount > 0) {
         %obj.client.updateAmmoImage();
      }
      //%obj.client.RefreshEquipentHud(%this.item.previewImage, %this.item.reticle, %this.item.zoomReticle, %amount);
   }

   function ProxMineImage::onMount( %this, %obj, %slot ) {
      // The mine doesn't use ammo from a player's perspective.
      %obj.setImageAmmo( %slot, true );
      %numMines = %obj.getInventory(%this.item);
      if (%obj.client !$= "" && !%obj.isAiControlled) {
         %obj.client.updateAmmoImage();
      }
      //%obj.client.RefreshEquipentHud(%this.item.previewImage, %this.item.reticle, %this.item.zoomReticle, %numMines);
   }

   function ProxMineImage::onUnmount( %this, %obj, %slot ) {
      //%obj.client.RefreshEquipentHud(0, "", "", 0);
   }
   
   function DeployableTurretWeapon::onInventory(%this, %obj, %amount) {
      if (%obj.client !$= "" && !%obj.isAiControlled) {
         //%obj.client.setAmmoAmountHud( 1, %amount );
      }
      // Cycle weapons if we are out of ammo
      if ( !%amount && ( %slot = %obj.getMountSlot( %this.image ) ) != -1 ) {
         //%obj.cycleWeapon( "prev" );
         %obj.switchWeapons(1);
      }
      if (%obj.client !$= "" && !%obj.isAiControlled && %amount > 0) {
         //%obj.client.RefreshEquipentHud(%this.item.previewImage, %this.item.reticle, %this.item.zoomReticle, %amount);
         %obj.client.updateAmmoImage();
      }
   }

   function DeployableTurretWeaponImage::onMount(%this, %obj, %slot) {
      // The turret doesn't use ammo from a player's perspective.
      %obj.setImageAmmo(%slot, true);
      %numTurrets = %obj.getInventory(%this.item);
      if (%obj.client !$= "" && !%obj.isAiControlled) {
         //%obj.client.RefreshEquipentHud(%this.item.previewImage, %this.item.reticle, %this.item.zoomReticle, %numTurrets);
         %obj.client.updateAmmoImage();
      }
   }

   function DeployableTurretWeaponImage::onUnmount(%this, %obj, %slot) {
      //%obj.client.RefreshEquipentHud(0, "", "", 0);
   }
};
activatePackage(afpsk_UpdatedSystem_Server);

package afpsk_Server_Loader {
   function GameCore::createGame() {
      Parent::createGame();
      deactivatePackage(afpsk_UpdatedSystem_Server);
      activatePackage(afpsk_UpdatedSystem_Server);
   }
};
activatePackage(afpsk_Server_Loader);
