//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// AIPlayer callbacks
// The AIPlayer class implements the following callbacks:
//
//    PlayerData::onStop(%this,%obj)
//    PlayerData::onMove(%this,%obj)
//    PlayerData::onReachDestination(%this,%obj)
//    PlayerData::onMoveStuck(%this,%obj)
//    PlayerData::onTargetEnterLOS(%this,%obj)
//    PlayerData::onTargetExitLOS(%this,%obj)
//    PlayerData::onAdd(%this,%obj)
//
// Since the AIPlayer doesn't implement it's own datablock, these callbacks
// all take place in the PlayerData namespace.
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Demo Pathed AIPlayer.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// AIPlayer static functions
//-----------------------------------------------------------------------------

function AIPlayer::spawn(%name,%spawnPoint)
{
   // Create the demo player object
   %player = new AiPlayer()
   {
      dataBlock = DemoPlayer;
      path = "";
   };
   MissionCleanup.add(%player);
   %player.setShapeName(%name);
   %player.setTransform(%spawnPoint);
   return %player;
}

function AIPlayer::spawnOnPath(%name,%path)
{
   // Spawn a player and place him on the first node of the path
   if (!isObject(%path))
      return 0;
   %node = %path.getObject(0);
   %player = AIPlayer::spawn(%name, %node.getTransform());
   return %player;
}

//-----------------------------------------------------------------------------
// AIPlayer methods
//-----------------------------------------------------------------------------

function AIPlayer::followPath(%this,%path,%node)
{
   // Start the player following a path
   %this.stopThread(0);
   if (!isObject(%path))
   {
      %this.path = "";
      return false;
   }

   if (%node > %path.getCount() - 1)
      %this.targetNode = %path.getCount() - 1;
   else
      %this.targetNode = %node;

   if (%this.path $= %path)
      %this.moveToNode(%this.currentNode);
   else
   {
      %this.path = %path;
      %this.moveToNode(0);
   }
   
   return true;
}

function AIPlayer::moveToNextNode(%this)
{
   if (%this.targetNode < 0 || %this.currentNode < %this.targetNode)
   {
      if (%this.currentNode < %this.path.getCount() - 1)
         %this.moveToNode(%this.currentNode + 1);
      else
         %this.moveToNode(0);
   }
   else
      if (%this.currentNode == 0)
         %this.moveToNode(%this.path.getCount() - 1);
      else
         %this.moveToNode(%this.currentNode - 1);
}
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------

function AIPlayer::pushTask(%this,%method)
{
   if (%this.taskIndex $= "")
   {
      %this.taskIndex = 0;
      %this.taskCurrent = -1;
   }
   %this.task[%this.taskIndex] = %method;
   %this.taskIndex++;
   if (%this.taskCurrent == -1)
      %this.executeTask(%this.taskIndex - 1);
}

function AIPlayer::clearTasks(%this)
{
   %this.taskIndex = 0;
   %this.taskCurrent = -1;
}

function AIPlayer::nextTask(%this)
{
   if (%this.taskCurrent != -1)
      if (%this.taskCurrent < %this.taskIndex - 1)
         %this.executeTask(%this.taskCurrent++);
      else
         %this.taskCurrent = -1;
}

function AIPlayer::executeTask(%this,%index)
{
   %this.taskCurrent = %index;
   eval(%this.getId() @"."@ %this.task[%index] @";");
}

//-----------------------------------------------------------------------------

function AIPlayer::singleShot(%this)
{
   // The shooting delay is used to pulse the trigger
   %this.setImageTrigger(0, true);
   %this.setImageTrigger(0, false);
   %this.trigger = %this.schedule(%this.shootingDelay, singleShot);
}

//-----------------------------------------------------------------------------

function AIPlayer::wait(%this, %time)
{
   %this.schedule(%time * 1000, "nextTask");
}

function AIPlayer::done(%this,%time)
{
   %this.schedule(0, "delete");
}

function AIPlayer::fire(%this,%bool)
{
   if (%bool)
   {
      cancel(%this.trigger);
      %this.singleShot();
   }
   else
      cancel(%this.trigger);
   %this.nextTask();
}

function AIPlayer::aimAt(%this,%object)
{
   //echo("Aim: "@ %object);
   %this.setAimObject(%object);
   %this.nextTask();
}

function AIPlayer::animate(%this,%seq)
{
   //%this.stopThread(0);
   //%this.playThread(0,%seq);
   %this.setActionThread(%seq);
}

function AIPlayer::moveToNode(%this, %index) {
   // Move to the given path node index
   %this.currentNode = %index;
   %node = %this.path.getObject(%index);
   %this.setMoveDestination(%node.getTransform(), %index == %this.targetNode);
   
   //echo("Call moveToNode "@%this@"("@%index@"): -> \n"@%this.getTransform()@"\n"@%node.getTransform()@"/"@%index TAB %this.targetNode@"");
}

// ----------------------------------------------------------------------------
// Some handy getDistance/nearestTarget functions for the AI to use
// ----------------------------------------------------------------------------

function AIPlayer::getTargetDistance(%this, %target)
{
   //echo("\c4AIPlayer::getTargetDistance("@ %this @", "@ %target @")");
   $tgt = %target;
   %tgtPos = %target.getPosition();
   %eyePoint = %this.getWorldBoxCenter();
   %distance = VectorDist(%tgtPos, %eyePoint);
   //echo("Distance to target = "@ %distance);
   return %distance;
}

function AIPlayer::getNearestPlayerTarget(%this) {
   %index = -1;
   %botPos = %this.getPosition();
   %closest = 9999999;
   for(%i = 0; %i < ClientGroup.getCount(); %i++) {
      %client = ClientGroup.getObject(%i);
      %player = %client.player;
      if(isObject(%player) && %player.getState() !$= "dead") {
         %tP = %player.getPosition();
         %dst = vectorDist(%botPos, %tP);
         if(%dst < %closest) {
            %closest = %dst;
            %index = %player;
         }
      }
   }
   return %index;
}

function AIPlayer::playerLOS(%this, %them)
{
    //from our eye to the player's eye
    %ai = %this.getEyePoint();
    %player = %them.getEyePoint();

    //things to get in the way
    %mask = $TypeMasks::TerrainObjectType | $TypeMasks::StaticObjectType;

    // see if anything gets hit
    %collision = containerRayCast(%ai, %player, %mask, %this);

    //nothing hit, this happens when EyeNode is outside of boundsBox
    //and thus raycast has no collision to record against
    //still means that they are viewable
    if(!%collision)
        return true;

    %hit = firstWord(%collision);

    //we can see them
    if(%hit == %them)
        return true;
    else
        return false;

}

// Poses ; 0 = stand, 1 = sprint, 2 = crouch, 3 = prone, 4 = swim //
function AiPlayer::doStand(%this) {
   //echo(%this.getname() @ " AiStand");
   if(%this.getPose() != 0) {
      %this.setPose(0);
      %this.schedule(50, "kickStartPose", 0);
   }
}

function AiPlayer::doSprint(%this) {
   //echo(%this.getname() @ " AiSprint");
   if(%this.getPose() != 1) {
      %this.setPose(1);
      %this.schedule(50, "kickStartPose", 1);
   }
}

function AiPlayer::doCrouch(%this) {
   //echo(%this.getname() @ " AiCrouch");
   if(%this.getPose() != 2) {
      %this.setPose(2);
      %this.schedule(50, "kickStartPose", 2);
   }
}

function AiPlayer::doProne(%this) {
   //echo(%this.getname() @ " AiProne");
   if(%this.getPose() != 3) {
      %this.setPose(3);
      %this.schedule(50, "kickStartPose", 3);
   }
}

function AiPlayer::doSwim(%this) {
   //echo(%this.getname() @ " AiSwim");
   if(%this.getPose() != 4) {
      %this.setPose(4);
      %this.schedule(50, "kickStartPose", 4);
   }
}

//and our kickstart bugfix workaround solution
function AiPlayer::kickStartPose(%this, %pose) {
   //echo("kickstart pose " @ %pose);
   %this.setPose(%pose);

   %this.setTransform(%this.getTransform());
}
