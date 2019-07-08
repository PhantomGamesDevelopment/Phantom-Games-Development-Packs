//topInformativeBar.cs
//Robert Fritzen
//(C) Phantom Games Development, 2012
//Controls the top info bar GUI

if(!isObject(TIBControl)) {
   new ScriptObject(TIBControl) {
      lastIndex = 0;
      visibleControl = false;
   };
}


function addInformativePush(%type, %image, %text) {
   %TIB = TIBControl;
   %last = %TIB.lastIndex;
   //
   %TIB.info[%last] = %type TAB %image TAB %text;
   %TIB.lastIndex++;

   if(!%TIB.visibleControl) {
      displayTIB();
   }
}

function displayTIB() {
   %TIB = TIBControl;
   if(!%TIB.visibleControl) {
      %TIB.visibleControl = true;
      Canvas.pushDialog(TopInformativeBar);
   }
   %type = getField(%TIB.info[0], 0);
   %image = getField(%TIB.info[0], 1);
   %text = getField(%TIB.info[0], 2);
   if(%type $= "unlock") {
      GeneralInfoBar.setVisible(false);
      WeaponUnlockBar.setVisible(true);
      
      GunPicture.setBitmap(%image);
      WeaponUnlockName.setText("<Font:Arial:26><color:FFFFFF><just:center>"@%text);
   }
   else {
      GeneralInfoBar.setVisible(true);
      WeaponUnlockBar.setVisible(false);
      
      InfoBar_Picture.setBitmap(%image);
      InfoBar_Text.setText("<Just:Center><Font:Arial:26><Color:FFFFFF>"@%text);
   }
   
   schedule(2500, 0, pushTIBStack);
}

function pushTIBStack() {
   echo("PushTIBStack()");
   %TIB = TIBControl;
   //
   if(%TIB.info[1] $= "") {
      echo("NO TIB 1");
      %TIB.info[0] = "";
      %TIB.lastIndex = 0;
      HideTIB();
   }
   else {
      for(%i = 1; %i < %TIB.lastIndex; %i++) {
         echo("SET TIB "@%i-1@": "@%TIB.info[%i]@"");
         %TIB.info[%i-1] = %TIB.info[%i];
         %TIB.info[%i] = "";
      }
      %TIB.lastIndex--;
      displayTIB();
   }
}

function HideTIB() {
   %TIB = TIBControl;
   Canvas.popDialog(TopInformativeBar);
   %TIB.visibleControl = false;
}
