#include "platform/platform.h"
#include "guiWaypointOverlay.h"

#include "console/console.h"
#include "console/consoleTypes.h"
#include "console/engineAPI.h"
#include "gfx/gfxDevice.h"
#include "gfx/gfxDrawUtil.h"

IMPLEMENT_CONOBJECT(GuiWaypointOverlay);

GuiWaypointOverlay::GuiWaypointOverlay() {
   mVerticalOffset = 0.5f;
	for(S32 i = 0; i < 64; i++) {
		mTeamColors[i].set(0, 0, 0);
	}
}

void GuiWaypointOverlay::initPersistFields() {
   addGroup("Misc");       
   addField( "verticalOffset", TypeF32, Offset( mVerticalOffset, GuiWaypointOverlay ), "Amount to vertically offset the control in relation to the ShapeBase object in focus." );
   endGroup("Misc");
   Parent::initPersistFields();
}

void GuiWaypointOverlay::onRender( Point2I offset, const RectI &updateRect) {
   // Must be in a TS Control
   GuiTSCtrl *parent = dynamic_cast<GuiTSCtrl*>(getParent());
   if (!parent) return;

   // Must have a connection and control object
   GameConnection* conn = GameConnection::getConnectionToServer();
   if (!conn) return;
   GameBase * control = dynamic_cast<GameBase*>(conn->getControlObject());
   if (!control) return;

	Point2F pos1, pos2, hold;
	Point3F shapePos, projPnt;
	GFXTextureObject *texture = NULL;
	RectF texRect, screenRect;
	String distString;

	GFX->getDrawUtil()->clearBitmapModulation();
   // Look for waypoints
   for (SimSetIterator itr(conn); *itr; ++itr) {
      ShapeBase* shape = dynamic_cast< ShapeBase* >(*itr);
      if ( shape ) {
         if (shape != control && shape->getShapeIcon() && dStrcmp(shape->getClassName(), "WayPoint") == 0) {
				shapePos = shape->getPosition();
            shapePos.z += mVerticalOffset;

				distString = String::ToString("%.3f M", (shapePos - control->getPosition()).len());

				if (!parent->project(shapePos, &projPnt)) {
               continue;          
				}
				hold.set(projPnt.x, projPnt.y);
				//
				if(shape->getTeam() < 0 || shape->getTeam() >= 64) {
					continue;
				}
				imagePath = shape->getShapeIcon();
				imageHandle.set(imagePath.c_str(), &GFXDefaultGUIProfile, avar("%s() - imageHandle (line %d)", __FUNCTION__, __LINE__));
				texture = imageHandle;
				//
				pos1.set(texture->mBitmapSize.x, texture->mBitmapSize.y);
				texRect.set(Point2F(0, 0), pos1);
				//
				pos1.set(hold.x - 10.0f, hold.y - 10.0f);
				pos2.set(20.0f, 20.0f);
				screenRect.set(pos1, pos2);
				//
				GFX->getDrawUtil()->setBitmapModulation(getTeamColor(shape->getTeam()));
				GFX->getDrawUtil()->drawBitmapStretchSR(texture, screenRect, texRect);
				GFX->getDrawUtil()->drawText(mProfile->mFont, Point2I(hold.x-20, hold.y+10+mProfile->mFont->getHeight()), distString.c_str());
         }
      }
   }
}

DefineEngineMethod(GuiWaypointOverlay, getTeamColor, const char *, (S32 team),, "(int) - gets a team's color") {
	String result = String::ToString("%f %f %f", object->getTeamColor(team).red, object->getTeamColor(team).green, 
		object->getTeamColor(team).blue);
	return result.c_str();
}

DefineEngineMethod(GuiWaypointOverlay, setTeamColor, void, (S32 team, F32 r, F32 g, F32 b),, "(int, float, float, float) - sets a team's color") {
	object->setTeamColor(team, ColorF(r, g, b));
}