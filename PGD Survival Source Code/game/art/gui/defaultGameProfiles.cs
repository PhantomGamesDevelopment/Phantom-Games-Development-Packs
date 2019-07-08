//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Override base controls
//GuiMenuButtonProfile.soundButtonOver = "AudioButtonOver";
//GuiMenuButtonProfile.soundButtonDown = "AudioButtonDown";

//-----------------------------------------------------------------------------
// Chat Hud profiles


singleton GuiControlProfile (ChatHudEditProfile)
{
   opaque = false;
   fillColor = "255 255 255";
   fillColorHL = "128 128 128";
   border = false;
   borderThickness = 0;
   borderColor = "40 231 240";
   fontColor = "40 231 240";
   fontColorHL = "40 231 240";
   fontColorNA = "128 128 128";
   textOffset = "0 2";
   autoSizeWidth = false;
   autoSizeHeight = true;
   tab = true;
   canKeyFocus = true;
};

singleton GuiControlProfile (ChatHudTextProfile)
{
   opaque = false;
   fillColor = "255 255 255";
   fillColorHL = "128 128 128";
   border = false;
   borderThickness = 0;
   borderColor = "40 231 240";
   fontColor = "40 231 240";
   fontColorHL = "40 231 240";
   fontColorNA = "128 128 128";
   textOffset = "0 0";
   autoSizeWidth = true;
   autoSizeHeight = true;
   tab = true;
   canKeyFocus = true;
};

singleton GuiControlProfile ("ChatHudMessageProfile")  
{  
   opaque = true;  
   fillColor = "0 0 0 0";  
     
   fontType = "Arial";  
   fontSize = 16;  
   fontColor = "44 172 181 0";    // default color (death msgs, scoring, inventory)  
   fontColors[1] = "4 235 105";   // client join/drop, tournament mode  
   fontColors[2] = "219 200 128"; // gameplay, admin/voting, pack/deployable  
   fontColors[3] = "77 253 95";   // team chat, spam protection message, client tasks  
   fontColors[4] = "40 231 240";  // global chat  
   fontColors[5] = "200 200 50 200";  // used in single player game  
     
   // WARNING! Colors 6-9 are reserved for name coloring  
   autoSizeWidth = true;  
   autoSizeHeight = true;  
};  

singleton GuiControlProfile ("ChatHudScrollProfile")
{
   opaque = false;
   borderThickness = 0;
   borderColor = "0 255 0";
   bitmap = "core/art/gui/images/scrollBar";
   hasBitmapArray = true;
};


//-----------------------------------------------------------------------------
// Core Hud profiles

singleton GuiControlProfile ("HudScrollProfile")
{
   opaque = false;
   border = true;
   borderColor = "0 255 0";
   bitmap = "core/art/gui/images/scrollBar";
   hasBitmapArray = true;
};

singleton GuiControlProfile ("HudTextProfile")
{
   opaque = false;
   fillColor = "128 128 128";
   fontColor = "0 255 0";
   border = true;
   borderColor = "0 255 0";
};

singleton GuiControlProfile ("ChatHudBorderProfile")
{
   bitmap = "core/art/gui/images/chatHudBorderArray";
   hasBitmapArray = true;
   opaque = false;
};


//-----------------------------------------------------------------------------
// Center and bottom print

singleton GuiControlProfile ("CenterPrintProfile")
{
   opaque = false;
   fillColor = "128 128 128";
   fontColor = "0 255 0";
   border = true;
   borderColor = "0 255 0";
};

singleton GuiControlProfile ("CenterPrintTextProfile")
{
   opaque = false;
   fontType = "Arial";
   fontSize = 12;
   fontColor = "0 255 0";
};

// -----------------------------------------------------------------------------
// HUD text
// -----------------------------------------------------------------------------

singleton GuiControlProfile (HudTextNormalProfile)
{
   opaque = false;
   fontType = "Arial";
   fontSize = 14;
   fontColor = "255 255 255";
};

singleton GuiControlProfile (HudTextItalicProfile : HudTextNormalProfile)
{
   fontType = "ArialItalic";
};

singleton GuiControlProfile (HudTextBoldProfile : HudTextNormalProfile)
{
   fontType = "ArialBold";
};

// -----------------------------------------------------------------------------
// Numerical health text
// -----------------------------------------------------------------------------

singleton GuiControlProfile (NumericHealthProfile)
{
   opaque = true;
   justify = "center";
   fontType = "ArialBold";
   fontSize = 32;
   fontColor = "255 255 255";
};

singleton GuiControlProfile(TransMenuButton)
{
   fillColor = "242 241 240 0";
   fillColorHL ="228 228 235 0";
   fillColorSEL = "98 100 137 0";
   fillColorNA = "255 255 255";
};

singleton GuiControlProfile( GuiArmoryRadioButton )
{
   fontSize = 14;
   fillColor = "232 232 232";
	/*fontColor = "200 200 200";
   fontColorHL = "255 255 255";*/
   fontColor = "255 255 255";
   fontColorHL = "255 255 255";
   fixedExtent = true;
   bitmap = "./images/radioButton";
   hasBitmapArray = true;
};
