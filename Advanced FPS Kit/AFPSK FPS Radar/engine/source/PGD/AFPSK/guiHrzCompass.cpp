//-----------------------------------------------------------------------------
// guiHrzCompass.cpp
// (C) 2014 Robert Fritzen for Phantom Games Development
// Source file for the horizontal compass control for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "PGD/AFPSK/guiHrzCompass.h"
#include "gui/3d/guiTSControl.h"
#include "scene/sceneManager.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"
#include "math/mathUtils.h"
#include "gfx/gfxFontRenderBatcher.h"
#include "gfx/gfxTransformSaver.h"
#include "gfx/gfxPrimitiveBuffer.h"

/**
Additional Functions
**/
F32 guiHrzCompassCtrl::getTNorthRotation(VectorF v1) {  
   // v1 must be normalized  
   v1.z = 0.0f;  
   if(!v1.isUnitLength()) {    
      v1.normalizeSafe();    
   }    
   F32 ang = atan2(v1.y, v1.x) - atan2(1.0f, 0.0f);
	return ang < 0 ? ang + M_2PI : ang;
} 
/**
End
**/

IMPLEMENT_CONOBJECT(guiHrzCompassCtrl);

//majorTicks - Points where we render a strong line w/ chars
S32 guiHrzCompassCtrl::majorTicks = 45; //360 == 0
//minorTicks - Points where we render smaller dash marks
S32 guiHrzCompassCtrl::minorTicks = 15;

String guiHrzCompassCtrl::tickToName(S32 tV) {
	if(tV == 0) {
		return "N";
	}
	else if(tV == 45) {
		return "NW";
	}
	else if(tV == 90) {
		return "W";
	}
	else if(tV == 135) {
		return "SW";
	}
	else if(tV == 180) {
		return "S";
	}
	else if(tV == 225) {
		return "SE";
	}
	else if(tV == 270) {
		return "E";
	}
	else if(tV == 315) {
		return "NE";
	}
	else {
	   return "";
	}
}

guiHrzCompassCtrl::guiHrzCompassCtrl(void) : 
   mCompassImgName(),
	spinAngle(0),
	factor(0),
	mFOV(45) {
		mBGColor.set(0, 0, 0);
		mTickColor.set(255, 255, 255);
		textPt.set(0, 0);
}

void guiHrzCompassCtrl::initPersistFields() {
   addGroup( "Compass" );

      addField( "compassLayer",   TypeImageFilename,     Offset( mCompassImgName, guiHrzCompassCtrl),
         "The compass image, use setCompassImage(name) to set. CHANGING THIS HAS NO EFFECT!!!" );
		addField( "backgroundColor", TypeColorI, Offset( mBGColor, guiHrzCompassCtrl), 
			"If you don't specify a compassLayer, render the compass with this background color instead.");
		addField( "tickColor", TypeColorI, Offset( mTickColor, guiHrzCompassCtrl), 
			"Color of the compass tick marks & the text of the locale font.");
      addField( "FOV",  TypeS32,         Offset( mFOV, guiHrzCompassCtrl ),
         "Defines the range of the compass (in degrees).  The default is 45." );

   endGroup( "Compass" );

   Parent::initPersistFields();
}

bool guiHrzCompassCtrl::onWake() {
	if (! Parent::onWake()) {
      return false;
	}
   if( !mProfile->mFont ) {
      Con::errorf( "guiHrzCompassCtrl::onWake() - no valid font in profile '%s'", mProfile->getName() );
      return false;
   }

   setActive(true);
   setCompassImage(mCompassImgName);
	//
   return true;
}

void guiHrzCompassCtrl::onSleep() {
	if ( !mCompassImgName.equal("texhandle", String::NoCase) ) {
      mCompassTO = NULL;
	}

   Parent::onSleep();
}

void guiHrzCompassCtrl::setCompassImage(const char *name) {
	mCompassImgName = name;
	if ( !isAwake() )
		return;
	if ( mCompassImgName.isNotEmpty() ) 
	   mCompassTO.set( mCompassImgName, &GFXDefaultGUIProfile, avar("%s() - mCompassTO (line %d)", __FUNCTION__, __LINE__) );
	else
		mCompassTO = NULL;

	setUpdate();
}

void guiHrzCompassCtrl::inspectPostApply() {
   // if the extent is set to (0,0) in the gui editor and appy hit, this control will
   // set it's extent to be exactly the size of the bitmap (if present)
   Parent::inspectPostApply();

   if ((getExtent().x == 0) && (getExtent().y == 0) && mCompassTO) {
      setExtent( mCompassTO->getWidth(), mCompassTO->getHeight());
   }
}

void guiHrzCompassCtrl::updateSizing() {
	if(!getParent()) {
      return;
	}

   // updates our bounds according to our horizSizing and verSizing rules
   RectI fakeBounds( getPosition(), getParent()->getExtent());

	factor = (F32)(getExtent().x) / (F32)(mFOV*2);

   parentResized( fakeBounds, fakeBounds);
}

void guiHrzCompassCtrl::onRender(Point2I offset, const RectI &updateRect) {
   // Must have a connection and control object for positioning purposes
   GameConnection* conn = GameConnection::getConnectionToServer();
	if (!conn) { 
		return;
	}
   GameBase * control = conn->getControlObject();
	if (!control) {
	   return;
	}
	//FOV Corrections
	if(mFOV >= _COMPASS_MAXIMA) {
	   mFOV = _COMPASS_MAXIMA;
	}
	if(mFOV <= 0) {
	   mFOV = 45;
	}

	//Factor == 0 is not allowed.
	if(factor <= 0) {
	   factor = (F32)(updateRect.extent.x) / (F32)(mFOV*2);
	}
	//Definitions
	spinAngle = getTNorthRotation(control->getTransform().getForwardVector());
	//Convert to Degrees
	degreeContainer = mFloor(mRadToDeg(spinAngle));
	min = degreeContainer - mFOV;
	max = degreeContainer + mFOV;
	//Start by rendering the Background
	if(mCompassTO) {
		temp1.set( 0, 0, mCompassTO->mBitmapSize.x, mCompassTO->mBitmapSize.y );
		GFX->getDrawUtil()->drawBitmapStretchSR(mCompassTO, updateRect, temp1);
	}
	else {
		GFX->getDrawUtil()->drawRectFill(updateRect, mBGColor);
	}
	closest = min % minorTicks;
	if(closest != 0) {
	   //flip to point in the correct direction
		closest = minorTicks - closest;
	}
	//Render Tick Marks.
	for(S32 i = 0; i < (mFOV*2); i += minorTicks) {
		tmp = (min + i) + closest;
		if(tmp < 0) {
		   tmp += _COMPASS_MAXIMA;
		}
		if(tmp >= _COMPASS_MAXIMA) {
		   tmp -= _COMPASS_MAXIMA;
		}
		//
		loc = updateRect.point.x + closest + (i * factor);
		if(isTicked(tmp) == 1) {
		   //Minor Tick & Location
			GFX->getDrawUtil()->drawRectFill(Point2I(loc-1, updateRect.point.y), Point2I((loc+1), updateRect.point.y + (updateRect.extent.y / 4)), mTickColor); 
		}
		if(isTicked(tmp) == 2) {
		   //Major Tick & Location
			GFX->getDrawUtil()->drawRectFill(Point2I(loc-2, updateRect.point.y), Point2I((loc+2), updateRect.point.y + (updateRect.extent.y / 2)), mTickColor); 
         if( !mProfile->mFont ) {
            mProfile->loadFont();
				if( !mProfile->mFont ) {
               return;
				}
         }
			//
			textPt.set(loc - (mProfile->mFont->getStrWidth(tickToName(tmp)) / 2), 
				(updateRect.point.y + updateRect.extent.y) - (mProfile->mFont->getHeight() + 4));
         GFX->getDrawUtil()->setBitmapModulation( mProfile->mFontColor );
         renderJustifiedText(textPt,
				Point2I(mProfile->mFont->getStrWidth(tickToName(tmp)), mProfile->mFont->getHeight() + 4), tickToName(tmp));
		}
	}
	//Phantom139: I have no idea why you'd event want "child" controls on the compass. It's more or less it's own entity
	//that can handle rendering everything it needs by itself, and thus, we ignore this generic T3D code portion.
	//Feel free to uncomment if you feel the need to add controls to your compass, I highly doubt it. :)
   //renderChildControls(offset, updateRect);
}

DefineEngineMethod(guiHrzCompassCtrl, setCompassImage, void, (const char *file),, "(String) Set the image layer of the map") {
   char filename[1024];
   Con::expandScriptFilename(filename, sizeof(filename), file);
	object->setCompassImage(filename);
}