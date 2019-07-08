//ai_generic.cs
//Robert Fritzen (Phantom139)
//(C) 2014, Phantom Games Development
//Control scripts for the generic AI (used in MP)

function GenericAI::onReachDestination(%this, %obj) {
   %obj.aiDecide();
}

function GenericAI::onEndSequence(%this, %obj, %slot) {
   %obj.stopThread(%slot);
   %obj.nextTask();
}

function GenericAI::onMoveStuck(%this, %obj) {
   if(%obj.stuck > 20) {
      %obj.stop();
      %obj.randomPath();
   }
   else {
      %obj.stuck++;
   }
}

function GenericAI::onEndOfPath(%this, %obj, %path) {
   %obj.aiDecide();
}

function GenericAI::onDamage(%this, %obj, %delta) {
   if(%obj.getstate() $= "Dead") {
      %obj.setImageTrigger(0, 0);
      //remove the old AI from the list
      $AIList.remove(%obj);
      AIManager.respawnAI(%obj);
   }
}
