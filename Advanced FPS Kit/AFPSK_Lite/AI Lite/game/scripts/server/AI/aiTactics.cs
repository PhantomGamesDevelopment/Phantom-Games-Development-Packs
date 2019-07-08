//aiTactics.cs
//Phantom139
//(C) 2014, Phantom Games Development
//Main file for controlling specific AI modes
//NOTE: I have adapted this from Steve Acaster's AI DM Resource, all credit goes to Steve
// For typing this all up and giving a great basic AI Example. There are numerous changes
// and updates, so be sure to check the original resource versus the changes here.

function AdvancedAIPlayer::aiStartUp(%this, %goal) {
   if(!isObject(%this)) {
      return;
   }

   if(%this.getState() $= "Dead") {
      return;
   }
   //here the Ai is going to decide what to do when it spawns

   //and let's randomize his "Level Of Aggression"
   //1 = low, will shoot at player but won't deviate from original goal
   //2 = medium, will attack player and follow player when he sees him
   //3 = high, is telepathically drawn to the player's location at all times
   %aggro = getRandom(1, 3);
   %this.LoA = %aggro;

   //get an initial path
   if(%aggro < 3) {
      %end = %goal.getPosition();
   }
   else {
      %enemy = %this.getClosestEnemyObject($TypeMasks::PlayerObjectType);
      if(!isObject(%enemy)) {
         //echo("Looking for but can't find a player to hunt, original goal");
         %end = %goal.getPosition();
      }
      else {
         %end = %enemy.getPosition();
      }
   }

   //check to see if we can shoot
   %this.canFire();

   //and go down our new path
   %this.setMoveDestination(%end);
}

function AdvancedAIPlayer::aiDecide(%this) {
   if(!isObject(%this)) {
      return;
   }

   if(%this.getState() $= "Dead") {
      return;
   }

   %path = %this.path;
   %start = %this.getPosition();

   //if we are super aggressive just go at them
   if(%this.LoA == 3) {
      //Check for enemies that may be in view.
      %enemy = %this.getClosestEnemyObject($TypeMasks::PlayerObjectType, true);
      if(isObject(%enemy)) {
         %end = %enemy.getPosition();
         %this.setMoveDestination(%end);
         return;
      }
      
      //If there's nobody in view, search out any target
      %enemy = %this.getClosestEnemyObject($TypeMasks::PlayerObjectType);
      if(isObject(%enemy)) {
         %end = %enemy.getPosition();
         %this.setMoveDestination(%end);
         return;
      }
   }

   //can we see the player?
   if(%this.LoA == 2) {
      //find enemy in view
      %enemy = %this.getClosestEnemyObject($TypeMasks::PlayerObjectType, true);
      if(isObject(%enemy)) {
         %end = %enemy.getPosition();
         %this.setMoveDestination(%end);
         return;
      }
   }

   //randomPath
   %this.randomPath();
}

function AdvancedAIPlayer::canFire(%this) {
   if(!isObject(%this)) {
      return;
   }

   if(%this.getState() $= "Dead") {
      return;
   }

   %path = %this.path;

   //if we're out of rounds reload!
   %image = %this.getMountedImage($WeaponSlot);
   if(%this.getInventory(%image.ammo) < 1) {
      %this.setInventory(%image.ammo, %this.maxInventory(%image.ammo));
      if(%this.attack || (%image.isField("clip") && %this.getInventory(%image.clip) < 1)) {
         //if we're attacking right now, or completely out of ammo, quickly flip off to the offhand weapon (if we can)
         if(%this.secondaryWeapon !$= "") {
            %this.use(%this.secondaryWeapon);
            %this.currentSlot = 2;
         }
      }
   }
   //Am I holding my secondary?
   if(!%this.attack && %this.currentSlot == 2) {
      %pI = nameToID(%this.primaryWeapon).image;
      if(%this.getInventory(%pI.ammo) > 0 || (%image.isField("clip") && %this.getInventory(%image.clip) > 0)) {
         %this.use(%this.primaryWeapon);
         %this.currentSlot = 1;
      }
   }
   //the main Ai loop checking for shooting
   %attack = %this.attack;
   %loa = %this.LoA;

   //find that player!
   %enemy = %this.getClosestEnemyObject($TypeMasks::PlayerObjectType, true);

   if(isObject(%enemy) && %enemy.getState() !$= "dead") {
      if(%this.hasLOSTo(%enemy)) {
         %this.setAimObject(%enemy, "0 0 1.5");
         if(%attack == 0) {
            %this.attack = 1;

            %this.setMoveDestination(%enemy.getPosition());
         }
         %this.aiShoot();
         %this.schedule(500, "canFire");
         return;
      }
   }
   if(%loa < 3 && %attack == 1) {
      %this.attack = 0;
   }
   %this.aiStopShoot();
   %this.clearAim();
   %this.schedule(500, "canFire");
}

function AdvancedAIPlayer::aiShoot(%this) {
   %this.setImageTrigger(0, true);
}

function AdvancedAIPlayer::aiStopShoot(%this) {
   %this.setImageTrigger(0, false);
}
