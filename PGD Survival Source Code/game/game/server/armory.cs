//armory.cs
//Phantom139 (Robert Fritzen)
//[C] Phantom Games Development, 2012

//Datablocks
datablock StaticShapeData(ArmoryObject) {
   shapeFile = "art/shapes/GG_Graveyard/Graveyard_Bench.cached.dts";
   cameraMaxDist = "0.5";
};

datablock StaticShapeData(ArmoryWeapon) {
   shapeFile = "art/shapes/weapons/Mamba/TP_Mamba.cached.dts";
   cameraMaxDist = "0.841167";
};
//Functions

function placeMapArmories() {
   %group = ArmoryMarks;
	for(%i = 0; %i < %group.getCount(); %i++) {
	   %marker = %group.getObject(%i);

		spawnArmory(%marker.getTransform());
	}
}

function spawnArmory(%transform) {
	if(!isObject(ArmoryGroup)) {
	    new SimGroup(ArmoryGroup);
	}
	//create the armory
   %armoryBase = new StaticShape() {
      datablock = ArmoryObject;
      rotation = "0 0 1 0";
		position = getWords(%transform, 0, 2);
   };   
	%armoryBase.fakeGun = new StaticShape() {
	   datablock = ArmoryWeapon;
		rotation = "0 0 90 1";
		position = vectorAdd(getWords(%transform, 0, 2), "0 0 0.1");
	};

	ArmoryGroup.add(%armoryBase);
}

function getClosestArmory(%player) {
	%c = 999999;
	for(%i = 0; %i < ArmoryGroup.getCount(); %i++) {
	   %armory = ArmoryGroup.getObject(%i);
		if(vectorDist(%armory.getTransform(), %player.getTransform()) < %c) {
		   %c = vectorDist(%armory.getTransform(), %player.getTransform());
			%closest = %armory;
		}
	}
	return %closest;
}

function openArmory(%client) {
	//push down some info for the guy
   %player = %client.getControlObject();
   %primary = %player.primaryWeapon;
   %secondary = %player.secondaryWeapon;
   //
   %Primary_item = nameToID(%primary);
   %Secondary_item = nameToID(%secondary);
   //
   if(%Primary_item.image.isField("clip")) {
	   %currentCount_P = %player.getInventory(%Primary_item.image.clip);
		%maxCount_P = %player.maxInventory(%Primary_item.image.clip);
	}
	else {
	   %currentCount_P = %player.getInventory(%Primary_item.image.ammo);
		%maxCount_P = %player.maxInventory(%Primary_item.image.ammo);	
	}
	//
	if(%Secondary_item.image.isField("clip")) {
	   %currentCount_S = %player.getInventory(%Secondary_item.image.clip);
		%maxCount_S = %player.maxInventory(%Secondary_item.image.clip);	
	}
	else {
	   %currentCount_S = %player.getInventory(%Secondary_item.image.ammo);
		%maxCount_S = %player.maxInventory(%Secondary_item.image.ammo);		
	}
	//
	if(%secondary $= "") {
	   %sec = "";
	}
	else {
	   %sec = "<Color:ffffff>"@%Secondary_item.description@": "@%currentCount_S@"/"@%maxCount_S@" Clips";
	}
   //
	if(%player.currentSlot == 1) {
	   commandToClient(%client, 'PushInfo', deTag(%Primary_item.PreviewImage), deTag(%Secondary_item.PreviewImage)
			, "<Color:ffffff>"@%Primary_item.description@": "@%currentCount_P@"/"@%maxCount_P@" Clips"
			, %sec);
	}
	else {
	   commandToClient(%client, 'PushInfo', deTag(%Secondary_item.PreviewImage), deTag(%Primary_item.PreviewImage)
			, "<Color:ffffff>"@%Secondary_item.description@": "@%currentCount_S@"/"@%maxCount_S@" Clips"
			, "<Color:ffffff>"@%Primary_item.description@": "@%currentCount_P@"/"@%maxCount_P@" Clips");	
	}
	for(%i = 0; %client.spec[%i] !$= ""; %i++) {
	   %output = %output @ %client.spec[%i] @ "\t";
	}
	commandToClient(%client, 'armorySpecs', %output);
	//then issue the open armory command
   commandToClient(%client, 'openArmory');
}

function serverCmdBuyItem(%client, %item) {
	%player = %client.getControlObject();
   %currentSlot = %player.currentSlot;
	%pgun = %player.primaryWeapon;
	%sgun = %player.secondaryWeapon;
	%points = %client.points;

	//some repitition from above
	if(%player.currentSlot == 1) {
      %Primary_item = nameToID(%pgun);
      %Secondary_item = nameToID(%sgun);
	}
	else {
      %Primary_item = nameToID(%sgun);
      %Secondary_item = nameToID(%pgun);	
	}
	//----
   if(%Primary_item.image.isField("clip")) {
	   %currentCount_P = %player.getInventory(%Primary_item.image.clip);
		%maxCount_P = %player.maxInventory(%Primary_item.image.clip);
	}
	else {
	   %currentCount_P = %player.getInventory(%Primary_item.image.ammo);
		%maxCount_P = %player.maxInventory(%Primary_item.image.ammo);	
	}
	//Secondary Weapon
	if(%Secondary_item !$= "") {
	   if(%Secondary_item.image.isField("clip")) {
	      %currentCount_S = %player.getInventory(%Secondary_item.image.clip);
		   %maxCount_S = %player.maxInventory(%Secondary_item.image.clip);
	   }
	   else {
	      %currentCount_S = %player.getInventory(%Secondary_item.image.ammo);
		   %maxCount_S = %player.maxInventory(%Secondary_item.image.ammo);
	   }
	}

	switch$(%item) {
		//cost 750
		case "ammo1":
			if(%points >= 750) {
				if(%currentCount_P < %maxCount_P) {
				   %client.points -= 750;
               %player.setInventory(%Primary_item.image.ammo, %player.maxInventory(%Primary_item.image.ammo));
					%player.setInventory(%Primary_item.image.clip, %player.maxInventory(%Primary_item.image.clip));
				   commandToClient(%client, 'ReturnArmory', 0);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 750
		case "ammo2":
			if(%points >= 750) {
				if(%currentCount_S < %maxCount_S) {
				   %client.points -= 750;
               %player.setInventory(%Secondary_item.image.ammo, %player.maxInventory(%Secondary_item.image.ammo));
					%player.setInventory(%Secondary_item.image.clip, %player.maxInventory(%Secondary_item.image.clip));
				   commandToClient(%client, 'ReturnArmory', 0);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 500
		case "ryder":
			%iDB = "Ryder";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 500) {
				%client.points -= 500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 2500
		case "duke":
			%iDB = "Duke";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 2500) {
				%client.points -= 2500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 2000
		case "lurker":
			%iDB = "Lurker";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 2000) {
				%client.points -= 2000;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 4500
		case "disposition":
			%iDB = "Disposition";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 4500) {
				%client.points -= 4500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

      //cost 7500
		case "swarmer":
			%iDB = "Swarmer";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 7500) {
				%client.points -= 7500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

      //cost 12500
		case "rocketlauncher":
			%iDB = "Ender";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 12500) {
				%client.points -= 12500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 12500
		case "mamba":
			%iDB = "Mamba";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 12500) {
				%client.points -= 12500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 17500
		case "kral":
			%iDB = "Kral";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 17500) {
				%client.points -= 17500;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 25000
		case "kraad":
			%iDB = "Kraad";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 25000) {
				%client.points -= 25000;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 50000
		case "kralmok":
			%iDB = "Kralmok";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 50000) {
				%client.points -= 50000;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}

		//cost 75000
		case "teslaforcer":
			%iDB = "TeslaForcer";
			if(%pgun $= %iDB || %sgun $= %iDB) {
			   //already have it bud... not getting another
				return;
			}
			if(%points >= 75000) {
				%client.points -= 75000;
			   commandToClient(%client, 'ReturnArmory', 0);
				
				if(%currentSlot == 1 && %sgun !$= "") {
				   %client.setWeapons(%iDB, %sgun, 1, 0);
				}
				else {
				   %client.setWeapons(%pgun, %iDB, 0, 1);
				}
			}
			else {
			   commandToClient(%client, 'ReturnArmory', -1);
			}
	}
	commandToClient(%client, 'updateClientPoints', %client.points);
}