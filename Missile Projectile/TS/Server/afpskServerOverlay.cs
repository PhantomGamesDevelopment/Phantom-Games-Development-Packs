//afpskServerOverlay.cs
//(C) Phantom Games Development - 2014
//By: Robert C. Fritzen
// Controls the server-side of the targeting overlay system

// Called when someone is locking onto you
function serverCmdsendLockWarnEvent(%client, %id) {
   //Send an alert to the object?
   %obj = resolveGTCTarget(%client, %id);
   if(%obj) {
      echo("Locking... "@%client@" => "@%obj);
   }
}

// Called when someone has locked on, and can now fire missiles of doom.
function serverCmdsendCompleteLockEvent(%client, %id) {
   //Send an alert to the object?
   %obj = resolveGTCTarget(%client, %id);
   if(%obj) {
      //Apply the target to the source.
      echo("Locked! "@%client@" => "@%obj);
      %client.lockedTarget = %obj;
   }
}

// Called when a lock is broken
function serverCmdsendLockBreakEvent(%client, %id) {
   //Clear the alert
   %client.lockedTarget = "";
   %obj = resolveGTCTarget(%client, %id);
   if(%obj) {
      echo("Lock Break. "@%sourceClient@" =/> "@%obj);
   }
}

//Update For the Server
package afpsk_overlay_Server {
   //onMount override - set up the locking overlay
   function WeaponImage::onMount(%this, %obj, %slot) {
      parent::onMount(%this, %obj, %slot);
      //lock overlay
      %enable = false;
      if(%this.usesLock) {
         %enable = true;
         %lockImages = %this.lockingImageStr;
         %lockedImage = %this.lockedImage;
         %range = %this.lockRange;
         %ticks = %this.lockTicks;
         %targMask = %this.targMask;
      }
      
      //Apply
      if(%obj.client) {
         if(%enable) {
            commandToClient(%obj.client, 'SetTargetingParameters', %enable, %lockImages, %lockedImage, %range, %ticks, %targMask);
         }
         else {
            commandToClient(%obj.client, 'SetTargetingParameters', false, "", "", 0, 0, 0);
         }
      }
   }
   
   //onFire override - sets target's if necessary.
   function WeaponImage::onFire(%this, %obj, %slot) {
      if (!isObject(%this.projectile)) {
         error("WeaponImage::onFire() - Invalid projectile datablock");
         return;
      }
      
      if(%obj.client) {
         //Once we fire, force a lock break to reset the control.
         commandToClient(%obj.client, 'forceLockBreak');
      }

      // Decrement inventory ammo. The image's ammo state is updated
      // automatically by the ammo inventory hooks.
      if ( !%this.infiniteAmmo )
         %obj.decInventory(%this.ammo, 1);

      // Get the player's velocity, we'll then add it to that of the projectile
      %objectVelocity = %obj.getVelocity();

      %numProjectiles = %this.projectileNum;
      if (%numProjectiles == 0)
         %numProjectiles = 1;

      for (%i = 0; %i < %numProjectiles; %i++) {
         if (%this.projectileSpread) {
            // We'll need to "skew" this projectile a little bit.  We start by
            // getting the straight ahead aiming point of the gun
            %vec = %obj.getMuzzleVector(%slot);

            // Then we'll create a spread matrix by randomly generating x, y, and z
            // points in a circle
            %matrix = "";
            for(%j = 0; %j < 3; %j++)
               %matrix = %matrix @ (getRandom() - 0.5) * 2 * 3.1415926 * %this.projectileSpread @ " ";
            %mat = MatrixCreateFromEuler(%matrix);

            // Which we'll use to alter the projectile's initial vector with
            %muzzleVector = MatrixMulVector(%mat, %vec);
         }
         else {
            // Weapon projectile doesn't have a spread factor so we fire it using
            // the straight ahead aiming point of the gun
            %muzzleVector = %obj.getMuzzleVector(%slot);
         }

         // Add player's velocity
         %muzzleVelocity = VectorAdd(
            VectorScale(%muzzleVector, %this.projectile.muzzleVelocity),
            VectorScale(%objectVelocity, %this.projectile.velInheritFactor));

         // Create the projectile object
         %p = new (%this.projectileType)() {
            dataBlock = %this.projectile;
            initialVelocity = %muzzleVelocity;
            initialPosition = %obj.getMuzzlePoint(%slot);
            sourceObject = %obj;
            sourceSlot = %slot;
            client = %obj.client;
            sourceClass = %obj.getClassName();
         };
         MissionCleanup.add(%p);
         
         if(%this.projectileType $= "MissileProjectile") {
            if(%obj.client) {
               %p.setTarget(%obj.client.lockedTarget);
            }
         }
      }
   }
};
activatePackage(afpsk_overlay_Server);

package afpsk_overlay_Server_Loader {
   function GameCore::createGame() {
      Parent::createGame();
      
      deactivatePackage(afpsk_overlay_Server);
      activatePackage(afpsk_overlay_Server);
   }
};
activatePackage(afpsk_overlay_Server_Loader);

/**
   Sample Weapon Image Datablock With Locking Params
   
   datablock ShapeBaseImageData(AAMissileWeaponImage) {
      // Basic Item properties
      shapeFile = "art/shapes/weapons/Ender/TP_Ender.DAE";
      shapeFileFP = "art/shapes/weapons/Ender/FP_Ender.DAE";
      emap = true;

      imageAnimPrefix = "Ender";
      imageAnimPrefixFP = "Ender";

      // Specify mount point & offset for 3rd person, and eye offset
      // for first person rendering.
      mountPoint = 0;
      firstPerson = true;
      useEyeNode = true;
      animateOnServer = true;

      correctMuzzleVector = true;

      // Add the WeaponImage namespace as a parent, WeaponImage namespace
      // provides some hooks into the inventory system.
      class = "WeaponImage";
      className = "WeaponImage";

	   ammoImage = "Ender";

      // Projectiles and Ammo.
      item = Ender;
      ammo = EnderAmmo;
      clip = EnderClip;

      projectile = MissileLauncherProjectile;
      wetProjectile = MissileLauncherProjectile;
      projectileType = MissileProjectile;
      projectileSpread = "0.005";
      
      //Set Missile Parameters
      usesLock = true;
      lockingImageStr = "art/gui/Locking_1.png art/gui/Locking_2.png";         //You can use spaced images to do an animation effect.
      lockedImage = "art/gui/Locked.png";                                      //Same with this if you want.
      lockRange = 350;                                                         //Self Explanitory :)
      lockTicks = 75;                                                          //1 Tick is ~30 - ~50 ms so you can use some simple math to obtain times.
      targMask = $TypeMasks::VehicleObjectType | $TypeMasks::TurretObjectType; //You can use multiple masks with the bitwise or (|) operator

**/
