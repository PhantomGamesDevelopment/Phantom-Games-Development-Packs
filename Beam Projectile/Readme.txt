Beam Projectile Pack

To Install: Drop these two source files somewhere in Engine/Source and compile!

The following are Beam Projectile Fields:

materialList - String Array: A list of materials that the tracer projectile will cycle through.

coneRange - F32: The range (in degrees) that a target may be selected, this is the angle from center.

beamWidth - F32: The width of the tracer projectile

numControlPoints - S32: The amount of bezier points used to draw the arc

maximumBeamRange - F32: The maximum range of the beam

drainHealth - F32: The amount of health drained / restored per tick

drainEnergy - F32: Same as above, but with energy

amountIsHealing - bool: If true, the drain values will restore, if false, they will drain

interval - S32: The interval (in milliseconds) that the texture used in materialList will cycle.

beamColor - ColorF: The color modulation applied to the material textures.

Here's a sample datablock:

datablock BeamProjectileData( BeamTestProj ) {
   projectileShapeName = "";

   numControlPoints    = 8;
   beamWidth           = 0.05;
   coneRange           = 35;
   maximumBeamRange    = 35.0;
   interval            = 6.0;

   drainHealth         = "0.3";
   drainEnergy         = "0.3";
   amountIsHealing     = false;

   materialList[0]     = "art/shapes/weapons/LaserRifle/ELFBeam.png";

   beamColor           = "0 0 1";
};

For the weapon Image, I recommend using a state along the following to make it work the best:

   stateName[0]                     = "Activate";
   stateSequence[0]                 = "Activate";
   stateSound[0]                    = SSENSwitchinSound;
   stateTimeoutValue[0]             = 0.5;
   stateTransitionOnTimeout[0]      = "ActivateReady";

   stateName[1]                     = "ActivateReady";
   stateTransitionOnAmmo[1]         = "Ready";
   stateTransitionOnNoAmmo[1]       = "NoAmmo";

   stateName[2]                     = "Ready";
   stateTransitionOnNoAmmo[2]       = "NoAmmo";
   stateTransitionOnTriggerDown[2]  = "CheckWet";

   stateName[3]                     = "Fire";
   stateEnergyDrain[3]              = 5;
   stateFire[3]                     = true;
   stateAllowImageChange[3]         = false;
   stateScript[3]                   = "onFire";
   stateTransitionOnTriggerUp[3]    = "Deconstruction";
   stateTransitionOnNoAmmo[3]       = "Deconstruction";
   //stateSound[3]                    = ElfFireWetSound;

   stateName[4]                     = "NoAmmo";
   stateTransitionOnAmmo[4]         = "Ready";

   stateName[5]                     = "Deconstruction";
   stateScript[5]                   = "deconstruct";
   stateTransitionOnTimeout[5]      = "Ready";
   stateTransitionOnNoAmmo[6]       = "NoAmmo";

   stateName[6]                     = "DryFire";
   stateSound[6]                    = MachineGunDryFire;
   stateTimeoutValue[6]             = 0.5;
   stateTransitionOnTimeout[6]      = "Ready";

   stateName[7]                     = "CheckWet";
   stateTransitionOnWet[7]          = "DryFire";
   stateTransitionOnNotWet[7]       = "Fire";