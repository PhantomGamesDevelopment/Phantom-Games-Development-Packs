//Chapter 8 Datablock Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

datablock ItemData(AK47) {
   category = "Weapon";
   className = "Weapon";

   // Basic Item properties
   shapeFile = "art/shapes/weapons/AK47/TP_AK47.DAE";
   mass = 1;
   elasticity = 0.2;
   friction = 0.6;
   emap = true;

   // Dynamic properties defined by the scripts
   PreviewImage = "AK47.png";
   pickUpName = "AK47";
   description = "AK47";
   image = AK47WeaponImage;
   reticle = "crossHair";
   heldWeaponName = "AK47";
};

//Define inherited block
datablock ItemData(AK47_VersionTwo : AK47) {
   // Dynamic properties defined by the scripts
   PreviewImage = "AK47_V2.png";
   pickUpName = "AK47 2.0";
   description = "AK47 2.0";
   heldWeaponName = "AK47 2.0";
};

