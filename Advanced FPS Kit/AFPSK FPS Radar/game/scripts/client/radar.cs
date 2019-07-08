//radar.cs
//Robert C. Fritzen - Phantom139
//Client-Side of Radar / Map Selector Controls, AFPSK
//This script is more of a guide than an actual edit, while one edit is done here, the
// rest is an example to learn from.

$AFPSK::Radar = TheRadar;        // <--- Replace with GuiAdvancedRadarCtrl name instance
$AFPSK::MapSe = MapSelector;     // <--- Replace with GuiMapSelectorCtrl name instance
$AFPSK::Radar::Initialized = 0;  // <--- Don't Touch Please

function initializeAFPSKRadar() {
   if($AFPSK::Radar::Initialized) {
      error("AFPSK Radar Already Initialized");
      return;
   }
   exec("art/gui/MapSelectionDlg.gui");
   exec("art/gui/MapViewDlg.gui");
   $AFPSK::Radar::Initialized = 1;
   //Set the default images, This is where you can adjust cursors and backgrounds...
   $AFPSK::Radar.setImageLayer(0, "art/gui/radar/maps/Background_Rect.png");
   $AFPSK::MapSe.setImageLayer(1, "art/gui/radar/Selector_Cursor.png");
   $AFPSK::MapSe.setImageLayer(2, "art/gui/radar/Selector_DirectionCursor.png");
}
initializeAFPSKRadar();

function clientCmdPushMapSelector(%type) {
   Canvas.pushDialog(MapSelectionDlg);                               //<--- Set here...
   $AFPSK::MapSe.setControlOptions(1, !%type ? false : true, false); //<--- Params: UAVFlag (Show Enemy Locations), Needs Direction (Requires both position & direction ie: airstrike)
}

function showMapDisplay(%type) {
   Canvas.pushDialog(MapViewDlg);                               //<--- Set here...
   MapViewer.setControlOptions(0, !%type ? false : true, true);
}

function closeMapDisplay() {
   Canvas.popDialog(MapViewDlg);
}

//Note: You'll need to bind this!!!
function pressMapDisplay(%val) {
   if(%val) {
      showMapDisplay(1); //< 1 == UAV, 0 == No UAV
   }
   else {
      closeMapDisplay();
   }
}

function GuiMapSelectorCtrl::onSelectPosition(%this, %worldPosition) {
   //Callback when the client selects a position.
   commandToServer('pushPositionSelection', %worldPosition);
}

function GuiMapSelectorCtrl::onSelectSecondPosition(%this, %worldPosition) {
   //Callback when the client selects the rotation (even though we actually get two positions)
   commandToServer('pushDirectionSelection', %worldPosition);
}

function clientCmdDoneWithSelections() {
   Canvas.popDialog(MapSelectionDlg);
}

//Not much going on here, excpet a few things
package AFPSK_Radar_Client {
   //Overload the existing handleLoadInfoMessage, see the server-side for the adjusted message
   // sent. This is basically to accomodate the fact we are now sending the map info to the radar
   // and map controls.
   function handleLoadInfoMessage( %msgType, %msgString, %mapName, %gameMode, %mapFileBase ) {
      // Make sure the LoadingGUI is displayed
      if (Canvas.getContent() != LoadingGui.getId()) {
         loadLoadingGui("LOADING MISSION FILE");
      }

      // Clear all of the loading info lines:
      for( %line = 0; %line < LoadingGui.qLineCount; %line++ ) {
         LoadingGui.qLine[%line] = "";
      }
      LoadingGui.qLineCount = 0;

      //
      LoadingGui.doMissionLoadSequence(%mapName, %gameMode, %mapFileBase);

      //Set the radar & map selector
      $AFPSK::Radar.setImageLayer(1, "art/gui/radar/maps/"@%mapFileBase@".png");
      $AFPSK::MapSe.setImageLayer(0, "art/gui/radar/maps/"@%mapFileBase@".png");
      MapViewer.setImageLayer(0, "art/gui/radar/maps/"@%mapFileBase@".png");
   }
};
activatePackage(AFPSK_Radar_Client);
