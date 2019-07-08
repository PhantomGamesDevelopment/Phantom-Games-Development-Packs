//preGameLobby.cs
//Robert Fritzen (Phantom139)
//[C] Phantom Games Development, 2012
//Controls the clientside aspect of the pre-game lobby

function clientCmdpushOpenPGL() {
   Canvas.pushDialog(LobbyGui);
}

function clientCmdPGLPostLaunch() {
   CSPreGameControl.delete(); //kill now!

   // Show the loading screen immediately.
	Canvas.popDialog(LobbyGui);
   if ( isObject( LoadingGui ) ) {
      Canvas.setContent("LoadingGui");
      LoadingProgress.setValue(1);
      LoadingProgressTxt.setValue("LOADING MISSION FILE");
      Canvas.repaint();
   }
}

function clientCmdpushPGLInfo(%index, %details) {
   if(!isObject(CSPreGameControl)) {
	   new ScriptObject(CSPreGameControl) {
	      class = "CSPreGame";
	   };
   }
   CSPreGameControl.info[%index] = %details;

	CSPreGameControl.updateGUI();
}

function clientCmdpushLobbyInfo(%name, %game, %prev) {
   LobbyGui_MapName.setText("<Font:Arial:22>Mission: <br><Font:Arial:26>"@%name@"");
	LobbyGui_GameType.setText("<Font:Arial:22>Game Type: <br><Font:Arial:26>"@%game@"");
	LobbyGui_MapPeview.setBitmap("levels/"@%prev@"");
}

function CSPreGame::updateGUI(%this) {
   %scroller = PlayerListScroll;
	%sExtent = %scroller.extent;

	%index = 0;
	while(%this.info[%index] !$= "") {
		%name = getField(%this.info[%index], 1);
		%rank = getField(%this.info[%index], 2);
      %cmLV = getField(%this.info[%index], 3);

		%guiObjectID = "CSPlayer_"@getField(%this.info[%index], 0);
		if(nameToID(%guiObjectID) == -1) {
			%container = new GuiContainer(%guiObjectID) {
			   position = "0 "@50 * %index;
				extent = getWord(%sExtent, 0)-5 SPC "45";
			};
			if(%cmLV > 0) {
			   %this.cmB[%index] = new GuiBitmapCtrl() {
               bitmap = "game/data/image/guiIcons/commod_badge.png";
               wrap = "0";
               imageColor = "0 0 0 0";
               position = "0 0";
               extent = "40 40";	
					tooltip = %cmLV == 1 ? "Community Moderator" : "Developer";
				};
				%container.add(%this.cmB[%index]);
			   %this.rnk[%index] = new GuiBitmapCtrl() {
               bitmap = "game/data/image/ranks/rank_"@%rank@".png";
               wrap = "0";
               imageColor = "0 0 0 0";
               position = "42 0";
               extent = "40 40";	
					tooltip = "Level "@%rank@"";
				};
				%container.add(%this.rnk[%index]);
			   %this.plName[%index] = new GuiMLTextCtrl() {
					text = "<Font:Arial:22><Color:4B0082>"@%name;
               useURLMouseCursor = "0";
               position = "84 9";
               extent = "150 40";			   
			   };
				%container.add(%this.plName[%index]);
			}
			else {
			   %this.rnk[%index] = new GuiBitmapCtrl() {
               bitmap = "game/data/image/ranks/rank_"@%rank@".png";
               wrap = "0";
               imageColor = "0 0 0 0";
               position = "0 0";
               extent = "40 40";	
					tooltip = "Level "@%rank@"";
				};		
				%container.add(%this.rnk[%index]);
			   %this.plName[%index] = new GuiMLTextCtrl() {
               text = "<Font:Arial:22>"@%name;
               useURLMouseCursor = "0";
               position = "42 9";
               extent = "150 40";			   
			   };
				%container.add(%this.plName[%index]);
			}
			%scroller.add(%container);
		}
		else {
		   //update the container
			%container = nameToID(%guiObjectID);
			if(isObject(%this.rnk[%index])) {
			   %this.rnk[%index].setBitmap("game/data/image/ranks/rank_"@%rank@".png");
				%this.rnk[%index].tooltip = "Level "@%rank@"";
			}
		}
	   %index++;
	}
}

//LobbyGui
function LobbyGui_Disconnect() {
   //clean out the CSPGL
	if(isObject(CSPreGameControl)) {
      CSPreGameControl.delete();
	}
	new ScriptObject(CSPreGameControl) {
	   class = "CSPreGame";
	};
	//
   Canvas.popDialog(LobbyGui);
	disconnect();
}

function LobbyGui_StartGame() {
   commandToServer('startGameButton');
}

function LobbyGui_SendMessage() {
	%message = LobbyChat_Text.getText();
   commandToServer('messageSent', %message);
	LobbyChat_Text.setText("");
}

function LobbyGui::onWake(%this) {
	//clear the scroller
	PlayerListScroll.clear();
   commandToServer('getLobbyInfo');

	LobbyChatHud.attach(HudMessageVector2);
}

//LOBBY GUI CHAT
new MessageVector(HudMessageVector2);
$LastHudTarget2 = 0;

function LobbyChatHud::addLine(%this, %text) {
   //first, see if we're "scrolled up"...
   %textHeight = %this.profile.fontSize + %this.lineSpacing;
   if (%textHeight <= 0)
      %textHeight = 12;
      
   %scrollBox = %this.getGroup();
   %chatScrollHeight = getWord(%scrollBox.extent, 1) - 2 * %scrollBox.profile.borderThickness;
   %chatPosition = getWord(%this.extent, 1) - %chatScrollHeight + getWord(%this.position, 1) - %scrollBox.profile.borderThickness;
   %linesToScroll = mFloor((%chatPosition / %textHeight) + 0.5);
   if (%linesToScroll > 0)
      %origPosition = %this.position;

   //remove old messages from the top only if scrolled down all the way
   while(HudMessageVector2.getNumLines() && (HudMessageVector2.getNumLines() >= $pref::HudMessageLogSize)) {
      %tag = HudMessageVector2.getLineTag(0);
      if(%tag != 0)
         %tag.delete();
      HudMessageVector2.popFrontLine();
   }

   //add the message...
   HudMessageVector2.pushBackLine(%text, $LastHudTarget);
   $LastHudTarget2 = 0;

   //now that we've added the message, see if we need to reset the position
   if (%linesToScroll > 0) {
      %this.position = %origPosition;
   }
}

function LobbyChatHud::setChatHudLength( %this, %length ) {
   LobbyGui_Chat.resize(firstWord(LobbyGui_Chat.Position), getWord(LobbyGui_Chat.Position, 1),
                 firstWord(LobbyGui_Chat.Extent), $outerChatLenY[%length]);
   LobbyChatScrollHud.scrollToBottom();
}

//Return Functions
function clientCmdReturnLobby() {
   returnToPregameLobby();
}

function returnToPregameLobby() {
   // We need to stop the client side simulation
   // else physics resources will not cleanup properly.
   physicsStopSimulation( "client" );
  
   if( $Client::missionRunning )
      clientEndMission();

   $lightingMission = false;
   $sceneLighting::terminateLighting = true;
   
   //save client data
   %guid = $ConnStore::guid;
   %file = GetUserDataPath() @"save/"@%guid@"/data.xmle";
   SaveClientData(%guid, %file);

	$LocalPoints = 0;

   LagIcon.setVisible(false);
   PlayerListGui.clear();
   
   // Clear all print messages
   clientCmdclearBottomPrint();
   clientCmdClearCenterPrint();

	Canvas.setContent(MainMenuGui);
	Canvas.pushDialog(LobbyGui);

   // We can now delete the client physics simulation.
   physicsDestroyWorld( "client" );    
}