//-----------------------------------------------------------------------------
// * guiBitmapStringCtrl.h - Created by: Robert C Fritzen for T3D MIT
//
// Copyright (c) 2012 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#ifndef _GUIBITMAPSTRINGCTRL_H_
#define _GUIBITMAPSTRINGCTRL_H_

#ifndef _GUICONTROL_H_
#include "gui/core/guiControl.h"
#endif

#include <string>

class GuiBitmapStringCtrl : public GuiControl {
   public:
   
      typedef GuiControl Parent;

   protected:
      String mBitmapName;
		String mCutoffBitmapName;
      
      /// Loaded texture.
      GFXTexHandle mTextureObject;
		GFXTexHandle mCutoffTextureObject;

		S32 mCutoffPoint; //the cutoff point number
		S32 mImageCount;  //the amount of images to string

      static bool setBitmapName( void *object, const char *index, const char *data );
      static const char *getBitmapName( void *obj, const char *data );
      static bool setCutoffBitmapName( void *object, const char *index, const char *data );
      static const char *getCutoffBitmapName( void *obj, const char *data );

   public:
      
      GuiBitmapStringCtrl();
      static void initPersistFields();

      void setBitmap(const char *name,bool resize = false);
      void setBitmapHandle(GFXTexHandle handle, bool resize = false);
      void setCutoffBitmap(const char *name,bool resize = false);
      void setCutoffBitmapHandle(GFXTexHandle handle, bool resize = false);

      // GuiControl.
      bool onWake();
      void onSleep();
      void inspectPostApply();

      void updateSizing();

      void onRender(Point2I offset, const RectI &updateRect);

      void setImageCounts(S32 imageCount, S32 cutoffPoint);
		S32 getImageCount();
		S32 getCutoffPoint();

      DECLARE_CONOBJECT( GuiBitmapStringCtrl );
      DECLARE_CATEGORY( "Gui Images" );
      DECLARE_DESCRIPTION( "A control that strings together two images based on a position number." );
};

#endif
