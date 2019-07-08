//weaponHud.cs
//[C] Phantom Games Development, 2012
//Controls for the weapon hud

//$WHTag - Global Path Variable to the weapon hud
$WHTag_Ammo = "game/data/image/weapons/";
$WHTag_Gun = "art/gui/weaponHud/";

function clientCmdupdateWeaponHUD(%pImg, %sImg) {
	echo(%pImg SPC %sImg);
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
   // No guns?
   if(%pImg $= "" && %sImg $= "") {
      AmmoImageString.setVisible(false);
   }
   else {
      AmmoImageString.setVisible(true);
   }
   //
   WH_Primary.setBitmap($WHTag_Gun @ %pImg);
   WH_Secondary.setBitmap($WHTag_Gun @ %sImg);
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

      %first = $WHTag_Ammo @ "bullet_";
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
