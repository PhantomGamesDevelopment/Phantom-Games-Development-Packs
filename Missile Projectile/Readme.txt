Missile Projectile & Overlay.

Simply drop these in your Engine/Source somewhere and the engine will compile these files.

Missile Datablock Parameters:

accuracy - F32 Value between 1 and 100 stating how accurate the missile is. This essentially makes a vector
scaled between the missile and the target and declares that the missile will follow the path along it.

flareTypes - String (Field Tabbed): Creates a list of projectiles that will act as flares towards the
missile.

flareDistance - F32 And this declares how far from said flareTypes the missile must be to be deflected.

terrainScanAhead - F32 How far ahead the missile will scan for terrain, if terrain is spotted out to this
distance between the missile and the projectile, the terrain dodge will kick in and the missile will bank 
upward to dodge the terrain.

terrainHeightFail - F32 This field is used to tell the missile how high above the terrain it must go to be
"safe".

Missile Commands:

* Missile.setTarget(ObjectID): Sets the active missile target
* %obj = Missile.getTarget(): returns the active missile target

** For more information on the missile & the included GUI overlay, please see the included scripts in the
TS folder, or simply browse through the C++/H files to see the list of declarations and accepted variables