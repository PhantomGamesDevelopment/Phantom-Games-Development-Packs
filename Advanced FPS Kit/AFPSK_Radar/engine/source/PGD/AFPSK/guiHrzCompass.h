//-----------------------------------------------------------------------------
// guiHrzCompass.h
// (C) 2014 Robert Fritzen for Phantom Games Development
// Header file for the horizontal compass control for the Advanced FPS Kit for T3D MIT 3.5
//-----------------------------------------------------------------------------

#ifndef _GUIHRZCOMPASSCTRL_H_
#define _GUIHRZCOMPASSCTRL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#include "T3D/gameBase/gameConnection.h"
#include "T3D/shapeBase.h"

#define _COMPASS_MAXIMA 360 //Don't change this...

/// Renders a map with capabilities to select positions from it.
class guiHrzCompassCtrl : public GuiControl {
   public:
   
      typedef GuiControl Parent;

   private:
		F32 spinAngle;
		S32 mFOV;
	   S32 closest, degreeContainer, min, max, tmp;
	   F32 loc;
	   RectI temp1;
		F32 factor;
		Point2I textPt;
		static S32 majorTicks;
		static S32 minorTicks;

		S32 isTicked(S32 i) {
			if(i % majorTicks == 0) {
			   return 2;
			}
			if(i % minorTicks == 0 && i % majorTicks != 0) {
				return 1;
			}
			return 0;
		}

   protected:
		String mCompassImgName;    
		GFXTexHandle mCompassTO;
		GFont *font;
		ColorI mBGColor; //<-- if there is no mCompassTO, we use a standard BGColor
		ColorI mTickColor; //Color of the compass ticks & font.

   public:
      guiHrzCompassCtrl();
      static void initPersistFields();

		String tickToName(S32 tV);

      void setCompassImage(const char *name);

      // GuiControl.
      bool onWake();
      void onSleep();
      void inspectPostApply();

      void updateSizing();

      void onRender(Point2I offset, const RectI &updateRect);

		//Helper Functions
		F32 getTNorthRotation(VectorF v);

      DECLARE_CONOBJECT( guiHrzCompassCtrl );
      DECLARE_DESCRIPTION( "A control that allows you to render a horizontal compass.");
};

#endif
