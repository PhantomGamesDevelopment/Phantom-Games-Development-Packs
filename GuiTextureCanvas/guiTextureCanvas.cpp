//-----------------------------------------------------------------------------
// Copyright (C) 2006 Sickhead Games, LLC.
// Portions Copyright (C) 2003 Pat Wilson and Flaming Duck Studio
// Portions Copyright (C) GarageGames.com, Inc.
// Portions (Porting to T3D) Copyright (C) 2009 Lethal Concept, LLC
//-----------------------------------------------------------------------------

#include "gui/core/guiTextureCanvas.h"
#include "gui/core/guiControl.h"
#include "gfx/gfxDevice.h"
#include "console/consoleTypes.h"
#include "T3D/shapeBase.h"
#include "ts/tsShapeInstance.h"
#include "materials/materialList.h"
#include "materials/matInstance.h"



GuiTextureCanvas::TextureCanvasVector GuiTextureCanvas::smTextureCanvases;

IMPLEMENT_CONOBJECT(GuiTextureCanvas);

extern ColorI gCanvasClearColor;

GuiTextureCanvas::GuiTextureCanvas()
{
   mGuiControlName = NULL;
   mGuiControl = NULL;
   mDirty = false;
   mInteract = true;
   smTextureCanvases.push_back( this );
}


GuiTextureCanvas::~GuiTextureCanvas() 
{
	unregister();
}


void GuiTextureCanvas::unregister()
{
   TextureCanvasVector::iterator i = smTextureCanvases.begin();
   for( ; i != smTextureCanvases.end(); i++ )
   {
      if( (*i) == this )
      {
         smTextureCanvases.erase( i );
         return;
      }
   }
}


void GuiTextureCanvas::initPersistFields()
{
   Parent::initPersistFields();

   addField( "guiControl", TypeString, Offset( mGuiControlName, GuiTextureCanvas ) );
}


bool GuiTextureCanvas::onAdd()
{
   /* bad idea, jeans
   if(!Parent::onAdd())
      return false; */

   //this isnt perfect either, but our direct Parent::onAdd() wants to
   //register another window, which is silly... 
   Parent::Parent::onAdd();

   setControl( dynamic_cast<GuiControl*>( Sim::findObject( mGuiControlName ) ) );

   return true;
}


void GuiTextureCanvas::onRemove()
{
   Parent::onRemove();

   mGuiControl = NULL;
   mTextureHandle = NULL;
}


void GuiTextureCanvas::inspectPostApply()
{
   Parent::inspectPostApply();

   setControl( dynamic_cast<GuiControl*>( Sim::findObject( mGuiControlName ) ) );
}


void GuiTextureCanvas::setControl( GuiControl* control )
{
   if ( mGuiControl != control ) {
      mGuiControl = control;
   }

   if ( !mGuiControl ) 
   {
      mTextureHandle = NULL;
      return;
   }

   if (  !mTextureHandle || 
         mTextureHandle->getWidth() != mGuiControl->getExtent().x ||
         mTextureHandle->getHeight() != mGuiControl->getExtent().y ) 
   {
      mTextureHandle.set(  mGuiControl->getExtent().x, 
                           mGuiControl->getExtent().y, 
                           GFXFormatR8G8B8A8, 
                           &GFXDefaultRenderTargetProfile, 
                           "" );
   }
}


void GuiTextureCanvas::renderFrame( bool preRenderOnly, bool bufferSwap )
{
   if ( !mDirty || !mGuiControl || !mTextureHandle )
      return;

    if(mPlatformWindow)
    {
        mPlatformWindow->hide();
        mPlatformWindow->setCursorVisible(false);
    }

   if ( mGuiControl->getGroup() != this )
      pushDialogControl( mGuiControl );

   iterator i;
   for(i = begin(); i != end(); i++)
   {
      GuiControl *contentCtrl = static_cast<GuiControl*>(*i);
      contentCtrl->preRender();
   }

   if (preRenderOnly)
      return;

    // rendersurface was changed to render target
    
   GFX->pushActiveRenderTarget();
   
   GFXTextureTarget * mRenderTarget;
   // get us some new off-screen rendering target
   mRenderTarget = GFX->allocRenderToTextureTarget();  
   mRenderTarget->attachTexture(GFXTextureTarget::Color0, mTextureHandle);
   GFX->setActiveRenderTarget( mRenderTarget);
   GFX->clear( GFXClearZBuffer | GFXClearStencil | GFXClearTarget, gCanvasClearColor, 1.0f, 0 );
   
   for(i = begin(); i != end(); i++)
   {
      GuiControl *contentCtrl = static_cast<GuiControl*>(*i);
      contentCtrl->onRender(contentCtrl->getPosition(), contentCtrl->getBounds());
   }

   GFX->popActiveRenderTarget();

   mDirty = false;
}


void GuiTextureCanvas::updateCanvases()
{
   TextureCanvasVector::iterator i = smTextureCanvases.begin();
   for( ; i != smTextureCanvases.end(); i++ )
      (*i)->renderFrame( false, false );
}

ConsoleMethod( GuiTextureCanvas, doMouseClick, void, 3, 4, "(Point2F pos)")
{
   Point2F pos(0,0);
   if(argc == 4)
      pos.set(dAtof(argv[2]), dAtof(argv[3]));
   else
      dSscanf(argv[2], "%g %g", &pos.x, &pos.y);
    
    object->doMouseClick(Point2I(pos.x, pos.y));

}

void GuiTextureCanvas::doMouseClick(Point2I pt)
{
   const Point2I& extent = getExtent();
   GuiEvent event;
   event.mousePoint = Point2I( pt.x * extent.x, pt.y * extent.y );
   rootMouseDown( event );
   rootMouseUp( event );
}
// castRay was removed in this version