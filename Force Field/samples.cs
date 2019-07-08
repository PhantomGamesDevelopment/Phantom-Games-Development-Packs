function spawnTestField(%player, %team, %color, %teampass, %allpass) {
   if(%color $= "") {
      %color = "0 0.25 0 0.5";
   }
   if(%teampass $= "") {
      %teampass = false;
   }
   if(%allpass $= "") {
      %allpass = false;
   }

   %test = new ForceField() {
      position = vectorAdd(getWords(%player.getTransform(), 0, 2), "10 0 0");
      rotation = getWords(%player.getTransform(), 2, 3);
      scale = "5 1 5";
      //Is this field enabled? set to false to disable collision & rendering.
      enabled = true;
      //What color does this field have?
      color = %color;
      //Does this field allow friendly objects who pass the allowX fields through?
      isTeamPass = %teampass;
      //Does this field allow any object that passes the allowX fields through?
      isAllPass = %allpass;
      //Can player objects pass through this field?
      allowPlayers = true;
      //Can vehicle objects pass through this field?
      allowVehicles = false;
      //Can non-sticky item objects pass through this field?
      allowItems = false;
      //What team is this field on?
      team = %team;
   };

   MissionGroup.add(%test);
   echo(%test);
}
