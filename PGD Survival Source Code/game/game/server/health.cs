//health.cs
//Phantom139
//Handles the end of wave HP Restore function

function WaveEndHP() {
	for(%i = 0; %i < ClientGroup.getCount(); %i++) {
		%client = ClientGroup.getObject(%i);
		%player = %client.getControlObject();
		if(isObject(%player) && %player.getState() !$= "Dead") {
		   RestoreHealthWaveComplete(%player);
		}
	}
}

function RestoreHealthWaveComplete(%obj) {
   if (%obj.getDamageLevel() != 0 && %obj.getState() !$= "Dead") {
		%obj.healthTimer = 0;
      %obj.applyRepair(100);
      doHealthUpdate(%obj);
      serverPlay3D(HealthUseSound, %obj.getTransform());
   }
}

function doHealthUpdate(%obj) {
   if (%obj.healthTimer < 40) {
      %obj.UpdateHealth();
      schedule(250, doHealthUpdate, %obj);
      %obj.healthTimer++;
   }
	else  {
      %obj.healthTimer = 0;
	}
}

function doMedicUpdate(%obj) {
	if(isEventPending(%obj.medicUpdate)) {
	   cancel(%obj.medicUpdate);
	}

   if (%obj.medicTimer < 40) {
      %obj.UpdateHealth();
      %obj.medicUpdate = schedule(250, doMedicUpdate, %obj);
      %obj.medicTimer++;
   }
	else  {
      %obj.medicTimer = 0;
	}
}