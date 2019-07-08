$RKL::maxRecentDisplays = 8;
$RKL::ImagePath = "game/data/image/killGui/";

function clientCmdAddGameKill(%killer, %killerWeapon, %victim, %kt, %vt) {
   addToKillList(%killer, %killerWeapon, %victim, %kt, %vt);
}

function addToKillList(%killer, %killerWeapon, %victim, %kt, %vt) {
   %this = RecentKillList;
   %element = %this.firstElement;

   cancel(%this.hideSched);
   %this.setVisible(true);
   %this.hideSched = schedule(10000, 0, removeKillList);
   //code for deploying elements - We start by pushing down the existing elements
   while(%this.killLine[%element] !$= "") {
      if(%element > $RKL::maxRecentDisplays) {
         %this.killLine[%element].delete();
         %this.firstElement++;
      }
      else {
         //move down
         %this.killLine[%element].setPosition(0, getWord(%this.killLine[%element].getPosition(), 1)+50);
         %element++;
      }
   }
   //code to add the new line to the top
   if(%killer $= "") {
      //this is the case if special deaths occured, such as falling, suicide, ect.
      AddKillElement(%element, %victim, %killerWeapon, "", %vt, "");
   }
   else {
      AddKillElement(%element, %killer, %killerWeapon, %victim, %kt, %vt);
   }
}

function removeKillList() {
   %this = RecentKillList;
   %this.setVisible(false);
   %element = 0;
   while(%this.killLine[%element] !$= "") {
      %this.killLine[%element].delete();
      %element++;
   }
   
   %this.firstElement = 0;
}

function AddKillElement(%kID, %killer, %killerWeapon, %victim, %killerTeam, %victimTeam) {
   %this = RecentKillList;
   %image = $RKL::ImagePath@%killerWeapon@".png";

   %kCol = %killerTeam == 0 ? "<color:ffffff>" : (%killerTeam == 1 ? "<color:660198>" : (%killerTeam == 2 ? "<color:3D5229>" : (%killerTeam == 3 ? "<color:00688B>" : "<color:990000>")));
   %vCol = %victimTeam == 0 ? "<color:ffffff>" : (%victimTeam == 1 ? "<color:660198>" : (%victimTeam == 2 ? "<color:3D5229>" : (%victimTeam == 3 ? "<color:00688B>" : "<color:990000>")));

   %this.killLine[%kID] = new GuiContainer() {
      margin = "0 0 0 0";
      padding = "0 0 0 0";
      anchorTop = "1";
      anchorBottom = "0";
      anchorLeft = "1";
      anchorRight = "0";
      position = "0 0";
      extent = "334 50";
      minExtent = "8 2";
      horizSizing = "right";
      vertSizing = "bottom";
      profile = "GuiDefaultProfile";
      visible = "1";
      active = "1";
      tooltipProfile = "GuiToolTipProfile";
      hovertime = "1000";
      isContainer = "1";
      canSave = "1";
      canSaveDynamicFields = "0";
      
      new GuiMLTextCtrl() {
         lineSpacing = "2";
         allowColorChars = "0";
         maxChars = "-1";
         text = "<Just:Center><Font:Arial:18>"@%kCol@""@%killer;
         useURLMouseCursor = "0";
         position = "5 16";
         extent = "122 18";
         minExtent = "8 2";
         horizSizing = "right";
         vertSizing = "bottom";
         profile = "GuiMLTextProfile";
         visible = "1";
         active = "1";
         tooltipProfile = "GuiToolTipProfile";
         hovertime = "1000";
         isContainer = "0";
         canSave = "1";
         canSaveDynamicFields = "0";
      };
      new GuiBitmapCtrl() {
         bitmap = %image;
         wrap = "0";
         position = "129 2";
         extent = "67 46";
         minExtent = "8 2";
         horizSizing = "right";
         vertSizing = "bottom";
         profile = "GuiDefaultProfile";
         visible = "1";
         active = "1";
         tooltipProfile = "GuiToolTipProfile";
         hovertime = "1000";
         isContainer = "0";
         canSave = "1";
         canSaveDynamicFields = "0";
      };
      //if(%victim !$= "") {
         new GuiMLTextCtrl() {
            lineSpacing = "2";
            allowColorChars = "0";
            maxChars = "-1";
            text = "<Just:Center><Font:Arial:18>"@%vCol@""@%victim;
            useURLMouseCursor = "0";
            position = "204 16";
            extent = "122 18";
            minExtent = "8 2";
            horizSizing = "right";
            vertSizing = "bottom";
            profile = "GuiMLTextProfile";
            visible = "1";
            active = "1";
            tooltipProfile = "GuiToolTipProfile";
            hovertime = "1000";
            isContainer = "0";
            canSave = "1";
            canSaveDynamicFields = "0";
         };
      //}
   };
   %this.add(%this.killLine[%kID]);
}
