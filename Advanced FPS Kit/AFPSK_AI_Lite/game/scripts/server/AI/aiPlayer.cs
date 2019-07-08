//aiPlayer.cs
//Robert Fritzen (Phantom139)
//(C) 2014, Phantom Games Development
//Control scripts for the AIPlayer Class.

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

function AdvancedAIPlayer::pushTask(%this, %method) {
   if(!isObject(%this) || %this.getState() $= "dead") {
      return;
   }
   if (%this.taskIndex $= "") {
      %this.taskIndex = 0;
      %this.taskCurrent = -1;
   }
   %this.task[%this.taskIndex] = %method;
   %this.taskIndex++;
   if (%this.taskCurrent == -1) {
      %this.executeTask(%this.taskIndex - 1);
   }
}

function AdvancedAIPlayer::clearTasks(%this) {
   %this.taskIndex = 0;
   %this.taskCurrent = -1;
}

function AdvancedAIPlayer::nextTask(%this) {
   if (%this.taskCurrent != -1) {
      if (%this.taskCurrent < %this.taskIndex - 1) {
         %this.executeTask(%this.taskCurrent++);
      }
      else {
         %this.taskCurrent = -1;
      }
   }
}

function AdvancedAIPlayer::executeTask(%this,%index) {
   %this.taskCurrent = %index;
   eval(%this.getId() @"."@ %this.task[%index] @";");
}

//-----------------------------------------------------------------------------

function AdvancedAIPlayer::singleShot(%this) {
   // The shooting delay is used to pulse the trigger
   %this.setImageTrigger(0, true);
   %this.setImageTrigger(0, false);
   %this.trigger = %this.schedule(%this.shootingDelay, singleShot);
}

//-----------------------------------------------------------------------------

function AdvancedAIPlayer::wait(%this, %time) {
   %this.schedule(%time * 1000, "nextTask");
}

function AdvancedAIPlayer::done(%this,%time) {
   %this.schedule(0, "delete");
}

function AdvancedAIPlayer::fire(%this,%bool) {
   if (%bool) {
      cancel(%this.trigger);
      %this.singleShot();
   }
   else {
      cancel(%this.trigger);
   }
   %this.nextTask();
}

function AdvancedAIPlayer::aimAt(%this,%object) {
   %this.setAimObject(%object);
   %this.nextTask();
}

function AdvancedAIPlayer::animate(%this,%seq) {
   %this.setActionThread(%seq);
}

// ----------------------------------------------------------------------------
// Some handy getDistance/nearestTarget functions for the AI to use
// ----------------------------------------------------------------------------

function AdvancedAIPlayer::getTargetDistance(%this, %target) {
   $tgt = %target;
   %tgtPos = %target.getPosition();
   %eyePoint = %this.getWorldBoxCenter();
   %distance = VectorDist(%tgtPos, %eyePoint);

   return %distance;
}

function AdvancedAIPlayer::randomPath(%this) {
   if(!isObject(%this)) {
      return;
   }

   if(%this.getState() $= "Dead") {
      return;
   }

   %this.stuck = 0;
   //get a random goal, count available up
   %spawn = pickPlayerSpawnPoint($Game::DefaultPlayerSpawnGroups);
   %end = %spawn.getPosition();

   %this.setMoveDestination(%end);
}
