//afpskClientOverlay.cs
//(C) Phantom Games Development - 2014
//By: Robert C. Fritzen
// Controls the client-side of the targeting overlay system
$AFPSK::OverlayObject = TargOverlay;

function clientCmdSetTargetingParameters(%enable, %image1, %image2, %maxRange, %neededTicks, %lockMask) {
   $AFPSK::OverlayObject.setupParams(%enable, %image1, %image2, %maxRange, %neededTicks, %lockMask);
}

function clientCmdforceLockBreak() {
   $AFPSK::OverlayObject.forceLockBreak();
}
