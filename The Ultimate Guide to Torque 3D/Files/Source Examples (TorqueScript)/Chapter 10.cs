//Chapter 10 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

function evalExample() {
   eval("schedule(1000, 0, \"echo\", \"Hello World!\");");
}

function displayOurGUI() {
   Canvas.setContent(MyAwesomeNewGUI);
}

function displayOurGUI_pushDialog() {
   Canvas.pushDialog(MyAwesomeNewGUI); //pushDialog has 3 parameters in total, if we used all three here it would look like this:
   // Canvas.pushDialog(MyAwesomeNewGUI, 0, false);
}

function removeOurGUI() {
   Canvas.popDialog(MyAwesomeNewGUI);
}

function fetchParentOfCloseButton() {
   %parent = MyAwesomeNewGUI_Closebutton.getParent();
   echo("The Parent control is: "@nameToID(%parent).getName());
}

function MyAwesomeNewGUI_CloseButton::onAdd(%this) {
   echo("Close button has been added, ID: "@%this);
}

function MyAwesomeNewGUI_CloseButton::onRemove(%this) {
   echo("Close button being deleted, ID: "@%this);
}

function MyAwesomeNewGUI_CloseButton::onVisible(%this, %newVisStatus) {
   echo("Close button switching visibility to: "@%newVisStatus);
}

function MPSelectDialog::onAdd(%this) {
   //set initial option values.
   %this.selectedMap = "Chinatown";
   %this.selectedMode = "Deathmatch";
   %this.selectedTime = "20";
   %this.selectedScore = "25";
   //Populate GUI dropdown objects
   MPSelectDialog_GameDD.add("Deathmatch", 0);
   MPSelectDialog_GameDD.add("Team Deathmatch", 1);
   MPSelectDialog_GameDD.add("CTF", 2);
   MPSelectDialog_ScoreDD.add("25", 0);
   MPSelectDialog_ScoreDD.add("50", 1);
   MPSelectDialog_ScoreDD.add("Unlimited", 2);
   MPSelectDialog_TimeDD.add("20 Minutes", 0);
   MPSelectDialog_TimeDD.add("30 Minutes", 1);
   MPSelectDialog_TimeDD.add("Unlimited", 2);
   //Set GUI dropdowns to their initial values
   MPSelectDialog_GameDD.setSelected(0);
   MPSelectDialog_ScoreDD.setSelected(0);
   MPSelectDialog_TimeDD.setSelected(0);
   //Set the image.
   MPSelectDialog_MapPreview.setBitmap("Levels/Chinatown.png");
   //Set text fields.
   MPSelectDialog_TextMap.setText("Selected Map: Chinatown");
   MPSelectDialog_TextGame.setText("Game Mode: Deathmatch");
   MPSelectDialog_TextScore.setText("Score Limit: 25");
   MPSelectDialog_TextTime.setText("Time Limit: 20 Minutes");
}

function MPSelectDialog_GameDD::onSelect(%this) {
   //Called when a user changes the dropdown selected option
   %selected = %this.getText();
   %row = %this.getSelected();
   //Do stuff…
}

function fetchLightning() {
   %stormID = nameToID("MissionGroup/EnvironmentObjects/LightningObject");
   //Do Stuff
}

function LegaciesBitmapButton::onMouseEnter(%this) {
   $currentControl = %this;
   if(!skillTreeDlg.storeLocked) {
      //This is a power/sub-power button, push the item tooltip
      if(!%this.isLocked) {
         %this.hoverBuyTooltip = schedule(500, 0, pushStoreTooltip, %this);
      }
   }
}

function LegaciesBitmapButton::onMouseLeave(%this) {
   $currentControl = "";
   if(isEventPending(%this.hoverBuyTooltip)) {
      cancel(%this.hoverBuyTooltip);
   }
}

function StoreHoverTooltip::onWake(%this) {
   skillTreeDlg.makeFirstResponder(true);
   %this.sleepSched = Canvas.schedule(1500, popDialog, %this);
}

function StoreHoverTooltip::onSleep(%this) {
   skillTreeDlg.makeFirstResponder(false);
}

//Mouse Events
function StoreTooltipBox::onMouseEnter(%this) {
   cancel(StoreHoverTooltip.sleepSched);
   skillTreeDlg.storeLocked = true;
}

function StoreTooltipBox::onMouseLeave(%this) {
   StoreHoverTooltip.sleepSched = Canvas.schedule(100, popDialog, StoreHoverTooltip);
   skillTreeDlg.storeLocked = false;
}

function pushStoreTooltip(%button) {
   %cursorPos = Canvas.getCursorPos();
   %screenPos = Canvas.getWindowPosition();
   %screenRes = Canvas.getExtent();
   %boxExtent = StoreTooltipBox.extent;
   //Secondary Items:
   %posAdd["ItemOfInterest"] = "18 19";
   %posAdd["BuyItemButton"] = "285 210";
   %posAdd["ItemTitle"] = "87 22";
   %posAdd["ItemDescription"] = "10 101";
   //Determine the best location to place the box on the screen
   %xCursor = getWord(%cursorPos, 0);
   %yCursor = getWord(%cursorPos, 1);
   %xSPos = getWord(%screenPos, 0);
   %ySPos = getWord(%screenPos, 1);
   %xScreen = getWord(%screenRes, 0);
   %yScreen = getWord(%screenRes, 1);
   %xBox = getWord(%boxExtent, 0);
   %yBox = getWord(%boxExtent, 1);
   //Some basic math.
   %xPos = (%xCursor-%xSPos) + 5;
   %yPos = (%yCursor-%ySPos) + 5;
   if((%xPos + %xBox) > (%xScreen)) {
      %xPos = %xScreen - (%xBox+5);
   }
   if((%yPos + %yBox) > (%yScreen)) {
      %yPos = %yScreen - (%yBox+5);
   }
   //Set the box position.
   StoreTooltipBox.position = %xPos SPC %yPos;
   ItemOfInterest.position = vectorAdd(StoreTooltipBox.position, %posAdd["ItemOfInterest"]);
   BuyItemButton.position = vectorAdd(StoreTooltipBox.position, %posAdd["BuyItemButton"]);
   ItemTitle.position = vectorAdd(StoreTooltipBox.position, %posAdd["ItemTitle"]);
   ItemDescription.position = vectorAdd(StoreTooltipBox.position, %posAdd["ItemDescription"]);
   //Set the internal dialog stuff here.
   // .. Omitted Code ..
   //And lastly push the dialog itself.
   Canvas.pushDialog(StoreHoverTooltip);
}
