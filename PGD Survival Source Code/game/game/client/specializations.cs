//specializations.cs
//Phantom139 (Robert Fritzen)
//[C] Phantom Games Development, 2012

//GVar List
//$InArmory[Index] = Image;                                              <-- Armory Internal
//$Upgrade[Index] = "Name\tDescription";                                 <-- Specialization Definition
//$Upgrade_Overide[New] = Old;                                           <-- Overriding Specializations (IE: I -> II)
//$UpgradeButton[Index] = Button\tBitmap;                                <-- Upgrade Button in Armory
//$Specialization[WeaponID, Index] = "Specialization ID\tNeeded Level";  <-- Specialization / Weapon Definition
//$AntiSpecialization[WeaponID, Index] = "Specialization ID";            <-- Disabled Specializations For This Gun

$InArmory[0] = armory_spec_ryder;
$InArmory[1] = armory_spec_duke;
$InArmory[2] = armory_spec_lurker;
$InArmory[3] = armory_spec_disposition;
$InArmory[4] = armory_spec_swarmer;
$InArmory[5] = armory_spec_rocketlauncher;
$InArmory[6] = armory_spec_mamba;
$InArmory[7] = armory_spec_kral;
$InArmory[8] = armory_spec_kraad;
$InArmory[9] = armory_spec_kralmok;
$InArmory[10] = armory_spec_teslaforcer;

$Upgrade[0] = "Accuracy I\tIncreases Accuracy by 10%\tspecialization_accuracy.png";
$Upgrade[1] = "Accuracy II\tIncreases Accuracy by 15%\tspecialization_accuracy.png";
$Upgrade[2] = "Accuracy III\tIncreases Accuracy by 25%\tspecialization_accuracy.png";
$Upgrade[3] = "Bandolier I\tIncreases Clip Capacity by 2\tspecialization_bandolier.png";
$Upgrade[4] = "Bandolier II\tIncreases Clip Capacity by 5\tspecialization_bandolier.png";
$Upgrade[5] = "Bandolier III\tArmor Regenerates Clips (Infinite Clips)\tspecialization_bandolier.png";
$Upgrade[6] = "Firepower I\tIncrease Damage by 25%\tspecialization_firepower.png";
$Upgrade[7] = "Firepower II\tIncrease Damage by 50%\tspecialization_firepower.png";
$Upgrade[8] = "Firepower III\tIncrease Damage by 100%\tspecialization_firepower.png";
$Upgrade[9] = "Incindiary Rounds\tRounds can incinerate zombies, causing them to die in a firey combustion\tspecialization_incendiaryrounds.png";
$Upgrade[10] = "Ion Storm\tTesla Forcer creates a point electrical storm that lasts 15 seconds\tspecialization_ionstorm.png";
$Upgrade[11] = "Mastery\tArm a wide variety of the weapon's specializations\tspecialization_master_x.png";
$Upgrade[12] = "Medic\tRounds heal teammates, regular damage on zombies\tspecialization_medic.png";
$Upgrade[13] = "Mini-Nuke\tMissile explodes in a miniature nuke causing explosive and incindiary damage\tspecialization_mininuke.png";
$Upgrade[14] = "Overload I\tIncrease Ammo Capacity by 25%\tspecialization_overload.png";
$Upgrade[15] = "Overload II\tIncrease Ammo Capacity by 50%\tspecialization_overload.png";
$Upgrade[16] = "Overload III\tIncrease Ammo Capacity by 100%\tspecialization_overload.png";
$Upgrade[17] = "Pulse Rounds I\tRounds have a small chance to disintegrate zombies\tspecialization_pulserounds.png";
$Upgrade[18] = "Pulse Rounds II\tRounds have a chance to disintegrate zombies\tspecialization_pulserounds.png";
$Upgrade[19] = "Shockwave I\tRounds become more explosive, explosive damage and force by 10%\tspecialization_shockwave.png";
$Upgrade[20] = "Shockwave II\tRounds become more explosive, explosive damage and force by 25%\tspecialization_shockwave.png";
$Upgrade[21] = "Shockwave III\tRounds become more explosive, explosive damage and force by 50%\tspecialization_shockwave.png";

$Upgrade_Overide[1] = 0;
$Upgrade_Overide[2] = 1;
$Upgrade_Overide[4] = 3;
$Upgrade_Overide[5] = 4;
$Upgrade_Overide[7] = 6;
$Upgrade_Overide[8] = 7;
$Upgrade_Overide[15] = 14;
$Upgrade_Overide[16] = 15;
$Upgrade_Overide[18] = 17;
$Upgrade_Overide[20] = 19;
$Upgrade_Overide[21] = 20;

$UpgradeButton[0] = SelectAccuracy TAB Spec_Accuracy;
$UpgradeButton[1] = SelectAccuracy TAB Spec_Accuracy;
$UpgradeButton[2] = SelectAccuracy TAB Spec_Accuracy;
$UpgradeButton[3] = SelectBandolier TAB Spec_Bandolier;
$UpgradeButton[4] = SelectBandolier TAB Spec_Bandolier;
$UpgradeButton[5] = SelectBandolier TAB Spec_Bandolier;
$UpgradeButton[6] = SelectFirepower TAB Spec_Firepower;
$UpgradeButton[7] = SelectFirepower TAB Spec_Firepower;
$UpgradeButton[8] = SelectFirepower TAB Spec_Firepower;
$UpgradeButton[9] = SelectIncendiary TAB Spec_Incendiary;
$UpgradeButton[10] = SelectIonStorm TAB Spec_IonStorm;
$UpgradeButton[11] = SelectMaster TAB Spec_Master;
$UpgradeButton[12] = SelectMedic TAB Spec_Medic;
$UpgradeButton[13] = SelectMiniNuke TAB Spec_MiniNuke;
$UpgradeButton[14] = SelectOverload TAB Spec_Overload;
$UpgradeButton[15] = SelectOverload TAB Spec_Overload;
$UpgradeButton[16] = SelectOverload TAB Spec_Overload;
$UpgradeButton[17] = SelectPulseRounds TAB Spec_PulseRounds;
$UpgradeButton[18] = SelectPulseRounds TAB Spec_PulseRounds;
$UpgradeButton[19] = SelectShockwave TAB Spec_Shockwave;
$UpgradeButton[20] = SelectShockwave TAB Spec_Shockwave;
$UpgradeButton[21] = SelectShockwave TAB Spec_Shockwave;

//------------------------------------
//Ryder
$Specialization[0, 0] = "0\t4";
$Specialization[0, 1] = "14\t7";
$Specialization[0, 2] = "3\t10";
$Specialization[0, 3] = "1\t13";
$Specialization[0, 4] = "6\t16";
$Specialization[0, 5] = "15\t20";
$Specialization[0, 6] = "4\t21";
$Specialization[0, 7] = "2\t25";
$Specialization[0, 8] = "7\t29";
$Specialization[0, 9] = "17\t33";
$Specialization[0, 10] = "16\t37";
$Specialization[0, 11] = "9\t40";
$Specialization[0, 12] = "18\t45";
$Specialization[0, 13] = "5\t48";
$Specialization[0, 14] = "8\t49";
$Specialization[0, 15] = "12\t49";
$Specialization[0, 16] = "11\t50";
$AntiSpecialization[0, 0] = "10";
$AntiSpecialization[0, 1] = "13";
$AntiSpecialization[0, 2] = "19";
$AntiSpecialization[0, 3] = "20";
$AntiSpecialization[0, 4] = "21";
//------------------------------------
//Duke
$Specialization[1, 0] = "0\t4";
$Specialization[1, 1] = "14\t7";
$Specialization[1, 2] = "3\t10";
$Specialization[1, 3] = "1\t13";
$Specialization[1, 4] = "6\t16";
$Specialization[1, 5] = "15\t20";
$Specialization[1, 6] = "4\t21";
$Specialization[1, 7] = "2\t25";
$Specialization[1, 8] = "7\t29";
$Specialization[1, 9] = "17\t33";
$Specialization[1, 10] = "19\t35";
$Specialization[1, 11] = "16\t37";
$Specialization[1, 12] = "20\t41";
$Specialization[1, 13] = "18\t45";
$Specialization[1, 14] = "21\t47";
$Specialization[1, 15] = "5\t48";
$Specialization[1, 16] = "8\t49";
$Specialization[1, 17] = "11\t50";
$AntiSpecialization[1, 0] = "9";
$AntiSpecialization[1, 1] = "10";
$AntiSpecialization[1, 2] = "12";
$AntiSpecialization[1, 3] = "13";
//------------------------------------
//Lurker
$Specialization[2, 0] = "0\t4";
$Specialization[2, 1] = "14\t7";
$Specialization[2, 2] = "3\t10";
$Specialization[2, 3] = "1\t13";
$Specialization[2, 4] = "6\t16";
$Specialization[2, 5] = "15\t20";
$Specialization[2, 6] = "4\t21";
$Specialization[2, 7] = "2\t25";
$Specialization[2, 8] = "7\t29";
$Specialization[2, 9] = "9\t30";
$Specialization[2, 10] = "16\t37";
$Specialization[2, 11] = "5\t48";
$Specialization[2, 12] = "8\t49";
$Specialization[2, 13] = "11\t50";
$AntiSpecialization[2, 0] = "10";
$AntiSpecialization[2, 1] = "12";
$AntiSpecialization[2, 2] = "13";
$AntiSpecialization[2, 3] = "17";
$AntiSpecialization[2, 4] = "18";
$AntiSpecialization[2, 5] = "19";
$AntiSpecialization[2, 6] = "20";
$AntiSpecialization[2, 7] = "21";
//------------------------------------
//Disposition
$Specialization[3, 0] = "0\t4";
$Specialization[3, 1] = "14\t7";
$Specialization[3, 2] = "3\t10";
$Specialization[3, 3] = "1\t13";
$Specialization[3, 4] = "6\t16";
$Specialization[3, 5] = "15\t20";
$Specialization[3, 6] = "4\t21";
$Specialization[3, 7] = "2\t25";
$Specialization[3, 8] = "7\t29";
$Specialization[3, 9] = "9\t30";
$Specialization[3, 10] = "16\t37";
$Specialization[3, 11] = "5\t48";
$Specialization[3, 12] = "8\t49";
$Specialization[3, 13] = "11\t50";
$AntiSpecialization[3, 0] = "10";
$AntiSpecialization[3, 1] = "12";
$AntiSpecialization[3, 2] = "13";
$AntiSpecialization[3, 3] = "17";
$AntiSpecialization[3, 4] = "18";
$AntiSpecialization[3, 5] = "19";
$AntiSpecialization[3, 6] = "20";
$AntiSpecialization[3, 7] = "21";
//------------------------------------
//Swarmer
$Specialization[4, 0] = "0\t4";
$Specialization[4, 1] = "14\t7";
$Specialization[4, 2] = "3\t10";
$Specialization[4, 3] = "1\t13";
$Specialization[4, 4] = "6\t16";
$Specialization[4, 5] = "15\t20";
$Specialization[4, 6] = "4\t21";
$Specialization[4, 7] = "2\t25";
$Specialization[4, 8] = "7\t29";
$Specialization[4, 9] = "9\t30";
$Specialization[4, 10] = "19\t34";
$Specialization[4, 11] = "16\t37";
$Specialization[4, 12] = "20\t40";
$Specialization[4, 13] = "21\t45";
$Specialization[4, 14] = "5\t48";
$Specialization[4, 15] = "8\t49";
$Specialization[4, 16] = "11\t50";
$AntiSpecialization[4, 0] = "10";
$AntiSpecialization[4, 1] = "12";
$AntiSpecialization[4, 2] = "13";
$AntiSpecialization[4, 3] = "17";
$AntiSpecialization[4, 4] = "18";
//------------------------------------
//Ender
$Specialization[5, 0] = "3\t10";
$Specialization[5, 1] = "6\t16";
$Specialization[5, 2] = "13\t21";
$Specialization[5, 3] = "4\t23";
$Specialization[5, 4] = "7\t29";
$Specialization[5, 5] = "19\t34";
$Specialization[5, 6] = "20\t40";
$Specialization[5, 7] = "21\t45";
$Specialization[5, 8] = "5\t48";
$Specialization[5, 9] = "8\t49";
$Specialization[5, 10] = "11\t50";
$AntiSpecialization[5, 0] = "0";
$AntiSpecialization[5, 1] = "1";
$AntiSpecialization[5, 2] = "2";
$AntiSpecialization[5, 3] = "9";
$AntiSpecialization[5, 4] = "10";
$AntiSpecialization[5, 5] = "12";
$AntiSpecialization[5, 6] = "14";
$AntiSpecialization[5, 7] = "15";
$AntiSpecialization[5, 8] = "16";
$AntiSpecialization[5, 9] = "17";
$AntiSpecialization[5, 10] = "18";
//------------------------------------
//Mamba
$Specialization[6, 0] = "14\t7";
$Specialization[6, 1] = "3\t10";
$Specialization[6, 2] = "6\t16";
$Specialization[6, 3] = "15\t20";
$Specialization[6, 4] = "4\t21";
$Specialization[6, 5] = "7\t29";
$Specialization[6, 6] = "9\t30";
$Specialization[6, 7] = "17\t35";
$Specialization[6, 8] = "16\t37";
$Specialization[6, 9] = "18\t45";
$Specialization[6, 10] = "5\t48";
$Specialization[6, 11] = "8\t49";
$Specialization[6, 12] = "11\t50";
$AntiSpecialization[6, 0] = "0";
$AntiSpecialization[6, 1] = "1";
$AntiSpecialization[6, 2] = "2";
$AntiSpecialization[6, 3] = "10";
$AntiSpecialization[6, 4] = "12";
$AntiSpecialization[6, 5] = "13";
$AntiSpecialization[6, 6] = "19";
$AntiSpecialization[6, 7] = "20";
$AntiSpecialization[6, 8] = "21";
//------------------------------------
//Kral
$Specialization[7, 0] = "0\t4";
$Specialization[7, 1] = "14\t7";
$Specialization[7, 2] = "3\t10";
$Specialization[7, 3] = "1\t13";
$Specialization[7, 4] = "6\t16";
$Specialization[7, 5] = "15\t20";
$Specialization[7, 6] = "4\t21";
$Specialization[7, 7] = "2\t25";
$Specialization[7, 8] = "7\t29";
$Specialization[7, 9] = "19\t35";
$Specialization[7, 10] = "16\t37";
$Specialization[7, 11] = "20\t41";
$Specialization[7, 12] = "21\t47";
$Specialization[7, 13] = "5\t48";
$Specialization[7, 14] = "8\t49";
$Specialization[7, 15] = "11\t50";
$AntiSpecialization[7, 0] = "9";
$AntiSpecialization[7, 1] = "10";
$AntiSpecialization[7, 2] = "12";
$AntiSpecialization[7, 3] = "13";
$AntiSpecialization[7, 4] = "17";
$AntiSpecialization[7, 5] = "18";
//------------------------------------
//Kraad
$Specialization[8, 0] = "0\t4";
$Specialization[8, 1] = "14\t7";
$Specialization[8, 2] = "3\t10";
$Specialization[8, 3] = "1\t13";
$Specialization[8, 4] = "6\t16";
$Specialization[8, 5] = "15\t20";
$Specialization[8, 6] = "4\t21";
$Specialization[8, 7] = "2\t25";
$Specialization[8, 8] = "7\t29";
$Specialization[8, 9] = "16\t37";
$Specialization[8, 10] = "5\t48";
$Specialization[8, 11] = "8\t49";
$Specialization[8, 12] = "11\t50";
$AntiSpecialization[8, 0] = "9";
$AntiSpecialization[8, 1] = "10";
$AntiSpecialization[8, 2] = "12";
$AntiSpecialization[8, 3] = "13";
$AntiSpecialization[8, 4] = "17";
$AntiSpecialization[8, 5] = "18";
$AntiSpecialization[8, 6] = "19";
$AntiSpecialization[8, 7] = "20";
$AntiSpecialization[8, 8] = "21";
//------------------------------------
//Kralmok
$Specialization[9, 0] = "3\t10";
$Specialization[9, 1] = "6\t16";
$Specialization[9, 2] = "4\t23";
$Specialization[9, 3] = "7\t29";
$Specialization[9, 4] = "19\t34";
$Specialization[9, 5] = "20\t40";
$Specialization[9, 6] = "21\t45";
$Specialization[9, 7] = "5\t48";
$Specialization[9, 8] = "8\t49";
$Specialization[9, 9] = "11\t50";
$AntiSpecialization[9, 0] = "0";
$AntiSpecialization[9, 1] = "1";
$AntiSpecialization[9, 2] = "2";
$AntiSpecialization[9, 3] = "9";
$AntiSpecialization[9, 4] = "10";
$AntiSpecialization[9, 5] = "12";
$AntiSpecialization[9, 6] = "13";
$AntiSpecialization[9, 7] = "14";
$AntiSpecialization[9, 8] = "15";
$AntiSpecialization[9, 9] = "16";
$AntiSpecialization[9, 10] = "17";
$AntiSpecialization[9, 11] = "18";
//------------------------------------
//Tesla Forcer
$Specialization[10, 0] = "6\t16";
$Specialization[10, 1] = "7\t29";
$Specialization[10, 2] = "10\t30";
$Specialization[10, 3] = "8\t49";
$Specialization[10, 4] = "11\t50";
$AntiSpecialization[10, 0] = "0";
$AntiSpecialization[10, 1] = "1";
$AntiSpecialization[10, 2] = "2";
$AntiSpecialization[10, 3] = "3";
$AntiSpecialization[10, 4] = "4";
$AntiSpecialization[10, 5] = "5";
$AntiSpecialization[10, 6] = "9";
$AntiSpecialization[10, 7] = "12";
$AntiSpecialization[10, 8] = "13";
$AntiSpecialization[10, 9] = "14";
$AntiSpecialization[10, 10] = "15";
$AntiSpecialization[10, 11] = "16";
$AntiSpecialization[10, 12] = "17";
$AntiSpecialization[10, 13] = "18";
$AntiSpecialization[10, 14] = "19";
$AntiSpecialization[10, 15] = "20";
$AntiSpecialization[10, 16] = "21";

function getSpecializations(%weaponID) {
	for(%i = 0; $Specialization[%weaponID, %i] !$= ""; %i++) {
		if(%list $= "") {
		   %list = getField($Specialization[%weaponID, %i], 0) @ " " @ getField($Specialization[%weaponID, %i], 1);
		}
		else {
		   %list = %list @ "\t" @ getField($Specialization[%weaponID, %i], 0) @ " " @ getField($Specialization[%weaponID, %i], 1);
		}
	}
	return %list;
}

function getAntiSpecializations(%weaponID) {
	for(%i = 0; $AntiSpecialization[%weaponID, %i] !$= ""; %i++) {
		if(%list $= "") {
		   %list = $AntiSpecialization[%weaponID, %i];
		}
		else {
		   %list = %list @ "\t" @ $AntiSpecialization[%weaponID, %i];
		}
	}
	return %list;
}

function specializations_OnSelectArmoryWeapon(%weaponID) {
   //get the list of specializations
	%specs = getSpecializations(%weaponID);
	%antiS = getAntiSpecializations(%weaponID);

   %info = getWeaponInfo(%weaponID); 
	%nameForButton = strlwr(strReplace(getField(%info, 0), " ", ""));
   %level = getField(%info, 4);
	
	for(%i = 0; %i < GetFieldCount(%specs); %i++) {
	   %spec = getField(%specs, %i);
		%sID = getWord(%spec, 0);
		%lNeed[%sID] = getWord(%spec, 1);

		//take it down to the base specialization
		%current = %sID;
		while($Upgrade_Overide[%current] !$= "") {
		   %current = $Upgrade_Overide[%current];
		}

		%GUIPair = $UpgradeButton[%sID];
		%button = getField(%GUIPair, 0);
		%image = getField(%GUIPair, 1);
		if(%sID == 11) {
		   %image.setBitmap("game/data/image/guiIcons/guns/specialization_master_"@%nameForButton@".png");
		}

		%button.setVisible(true);
		%image.setVisible(true);
		if(%level >= %lNeed[%sID]) {
		   %button.tooltip = getField($Upgrade[%sID], 0) @" - "@getField($Upgrade[%sID], 1);
			%image.tooltip = getField($Upgrade[%sID], 0) @" - "@getField($Upgrade[%sID], 1);
			%image.deleteImageColor();
		}
		else {
			if(%level < %lNeed[%current]) {
		      %button.tooltip = "Reach Affinity Level "@%lNeed[%current]@" to Unlock.";
		   	%image.tooltip = "Reach Affinity Level "@%lNeed[%current]@" to Unlock.";
			   %image.setImageColor(11, 11, 11, 255);
			}
		}
	}

	//Anti-Specs
	for(%i = 0; %i < GetFieldCount(%antiS); %i++) {
	   %spec = getField(%antiS, %i);
		%GUIPair = $UpgradeButton[%spec];
		%button = getField(%GUIPair, 0);
		%image = getField(%GUIPair, 1);

		%button.setVisible(false);
		%image.setVisible(false);
	}
}

function SelectSpecialization(%sel) {
   %weapon = $SelectedGun;
   %info = getWeaponInfo(%weapon); 
	//%cSpec = getField(%info, 1);
   %level = getField(%info, 4);

	switch$(%sel) {
		case "Accuracy":
			%upgrades = 0 SPC 1 SPC 2;
		case "Bandolier":
			%upgrades = 3 SPC 4 SPC 5;
		case "Firepower":
			%upgrades = 6 SPC 7 SPC 8;
		case "Incendiary":
			%upgrades = 9;
		case "IonStorm":
			%upgrades = 10;
		case "Master":
			%upgrades = 11;
		case "Medic":
			%upgrades = 12;
		case "MiniNuke":
			%upgrades = 13;
		case "Overload":
			%upgrades = 14 SPC 15 SPC 16;
		case "PulseRounds":
			%upgrades = 17 SPC 18;
		case "Shockwave":
			%upgrades = 19 SPC 20 SPC 21;
	}
	//-------------------------------
	//Are we being a little script hack kiddie?
	%aspec = getAntiSpecializations(%weapon);
	for(%i = 0; %i < getFieldCount(%aspec); %i++) {
	   %block = getField(%aspec, %i);
		for(%j = 0; %j < getWordCount(%upgrades); %j++) {
		   %get = getWord(%upgrades, %j);
			if(%get == %block) {
			   MessageBoxOk("Invalid Action", "This Specialization is not usable with this weapon.");
				return;
			}
		}
	}
	//--------------------------------
	//Test the guy's affinity level, ensure he's not clicking one of those pretty black buttons
	%specs = getSpecializations(%weapon);
   for(%i = 0; %i < getWordCount(%upgrades); %i++) {
	   %spec = getWord(%upgrades, %i);
		//Define the weapon's Affinity Level Restrictions
		for(%j = 0; %j < getFieldCount(%specs); %j++) {
	      %testing = getField(%specs, %j);
		   %sID = getWord(%testing, 0);
		   %lNeed[%sID] = getWord(%testing, 1);
		}		
      
      %theBase = getWord(%upgrades, 0);
		if(%level < %lNeed[%theBase]) {
		   //Not today good sir!
			MessageBoxOk("Affinity Restriction", "This specialization class is locked until affinity level "@%lNeed[%theBase]@".");
			return;
		}
		else {
	      //--------------------------------
	      //All tests have passed, last thing to do is to run through each upgrade tier and find the correct level/upgrade match and apply it.	
			if(%level >= %lNeed[%spec]) {
				%lastUpgrade = %spec;
			}
		}
	}
	//We've got it!
	setWeaponSpec(%weapon, %lastUpgrade);
	//and let's update our GUI as well.
	GunActiveSpec.setText("<Font:Arial:16>Active: "@getField($Upgrade[%lastUpgrade], 0)@"");
}

function clientCmdrequestSpecializations() {
	for(%i = 0; %i < 11; %i++) {
	   %info = getWeaponInfo(%i); 
	   %cSpec = getField(%info, 1);
		if(%sList $= "") {
		   %sList = getField($Upgrade[%cSpec], 0);
		}
		else {
		   %sList = %sList TAB getField($Upgrade[%cSpec], 0);
		}
	}

	commandToServer('sendSpecializations', %sList);
}