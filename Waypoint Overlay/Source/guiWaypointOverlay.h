#ifndef _GUIWAYPOINTOVERLAY_H_
#define _GUIWAYPOINTOVERLAY_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif
#include "platform/platform.h"
#include "gui/3d/guiTSControl.h"
#include "console/consoleTypes.h"
#include "scene/sceneManager.h"
#include "T3D/missionMarker.h"
#include "T3D/gameBase/gameConnection.h"
#include "T3D/shapeBase.h"
#include "gfx/gfxDrawUtil.h"
#include "console/engineAPI.h"

class GuiWaypointOverlay : public GuiControl {
   typedef GuiControl Parent;

   F32      mVerticalOffset;
	ColorF   mTeamColors[64];

	String       imagePath;
	GFXTexHandle imageHandle;

   public:
      GuiWaypointOverlay();

      // GuiControl
		void setTeamColor(S32 team, ColorF c) {
			if(team < 0 || team >= 64) {
				Con::errorf("::setTeamColor() - Cannot set color on a team out of overlay array bounds.");
				return;
			}
			mTeamColors[team] = c;
		}

		ColorF getTeamColor(S32 team) { 
			if(team < 0 || team >= 64) {
				Con::errorf("::getTeamColor() - Cannot get color on a team out of overlay array bounds.");
				return ColorF(0, 0, 0);
			}		
			return mTeamColors[team]; 
		}

      virtual void onRender(Point2I offset, const RectI &updateRect);
		static void initPersistFields();

      DECLARE_CONOBJECT( GuiWaypointOverlay );
      DECLARE_CATEGORY( "Gui Game" );
      DECLARE_DESCRIPTION( "A control used to show icons over waypoint objects in game" );
};

#endif