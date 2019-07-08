//playerLoadouts.cs
//Robert C. Fritzen
//core script for player loadouts

function initialize_afpsk_server() {
   if(!$AFPSK::Server::Loaded) {
      exec("./weaponSystem.cs");
      $AFPSK::Server::Loaded = 1;
   }
}
initialize_afpsk_server();

$LoadoutSystem::MaxClasses = 10; //This must match the value in client/playerLoadouts.cs

function serverCmdtransmitLoadout(%client, %loadoutIndex, %data) {
   %client.loadoutData[%loadoutIndex] = %data;
   if(%loadoutIndex == $LoadoutSystem::MaxClasses-1) {
      if(game.customLoadoutsDefined || game.noLoadouts) {
         //send game defined loads, You will code this :)
      }
      else {
         for(%i = 0; %i < $LoadoutSystem::MaxClasses; %i++) {
            //Basically this is a verification step, you can check if the client can use the gun here then send the laodout back down.
            commandToClient(%client, 'DropLoadout', %i, %client.loadOutData[%i]);
         }
      }
   }
}

function serverCmdplayeruseloadout(%client, %index) {
   %client.loadoutIndex = %index;
}

function serverCmdplayeruseEquipment(%client) {
   %player = %client.getControlObject();
   if(!isObject(%player) || %player.getState() $= "Dead") {
      return;
   }
   
   if(%player.equipment $= "") {
      return;
   }
   
   //Phantom139: Modified this
   %player.switchWeapons(3);
}

function serverCmdRequestLoadoutSelector(%client) {
   %client.pushIGCS(game);
}

//
function GameConnection::pushIGCS(%client, %game) {
   if(%game.noLoadouts) {
      //push a modified IGCS with no loadout selector
   }
   commandToClient(%client, 'PushIGCS');
}
