//-----------------------------------------------------------------------------
// Copyright (C) 2006 Sickhead Games, LLC.
// Portions Copyright (C) 2003 Pat Wilson and Flaming Duck Studio
// Portions Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _GUITEXTURECANVAS_H_
#define _GUITEXTURECANVAS_H_

#include "gfx/gfxTextureHandle.h"
#include "gui/core/guiCanvas.h"
//#include "core/tVector.h"
#include "scene/sceneObject.h"
#include "gfx\D3D9\gfxD3D9Device.h"
class ShapeBase;
struct TriRayInfo;

class GuiTextureCanvas : public GuiCanvas
{
   typedef GuiCanvas Parent;

private:

   GFXTexHandle mTextureHandle;

   Vector<StringTableEntry> mObjects;

   const char* mGuiControlName;
   GuiControl* mGuiControl;

   bool mDirty;

    bool mInteract;
    
   typedef Vector<GuiTextureCanvas*> TextureCanvasVector;
   static TextureCanvasVector smTextureCanvases;

   void init();
   void unregister();

public:

   DECLARE_CONOBJECT(GuiTextureCanvas);

   GuiTextureCanvas();
   ~GuiTextureCanvas();
   static void initPersistFields();

   static void updateCanvases();

   virtual bool onAdd();
   virtual void onRemove();
   virtual void inspectPostApply();

   virtual void renderFrame( bool preRenderOnly, bool bufferSwap = true );

   static bool castRay( ShapeBase* object, const Point3F& start, const Point3F& end, TriRayInfo* rayInfo ){return false;};

   void setControl( GuiControl* control );
   void setDirty();
   bool getInteract(){return mInteract;};
   bool setInteract(bool interact = true){mInteract = interact;};

   GFXTexHandle getTextureHandle() const;

    void doMouseClick(Point2I pt);
};


inline GFXTexHandle GuiTextureCanvas::getTextureHandle() const 
{
   return mTextureHandle;
}

inline void GuiTextureCanvas::setDirty()
{ 
	mDirty = true; 
}


#endif // _GUITEXTURECANVAS_H_