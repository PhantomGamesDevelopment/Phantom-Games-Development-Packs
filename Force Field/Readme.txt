Force Field Addon for Torque 3D MIT 3.5.1
By: Phantom Games Development

Intro: This pack is a small addon that aims to re-add the ForceField object used by the game Tribes 2, which
was built on an early version of the engine.

Installation:

1) Install the pack files into the engine, add the files to your project's solution.

2) Make the following changes to the engine:



   source/collision/collision.h:



    * Around line 122, you will see a method named void clear(), which is part of the CollisionList class. After 
      this method, add the following:

	//Phantom139: Added Here (Thanks to Dan Buckmater for helping out!)
	void remove(const U32 i) {  
		if (i < mCount) {  
			for (U32 j = i; j < mCount; j++) {  
				mCollision[j] = mCollision[j+1];  
			}  
			mCount--;
		}  
	} 
	//Phantom139: End



   source/T3D/shapeBase.cpp:



    * Scroll down to void ShapeBase::queueCollision(), located around line 2940. Right above the definition line,
      add:

      #include "PGD/AFPSK/forceField.h"

    * Then, in the same method, right after the opening brace [ { ], add this block of code:

      //Phantom139: Added Here For FF
	
      if(dStrcmp(obj->getClassName(), "ForceField") == 0) {
		
         //Fork this off to the ForceField
		
         static_cast<ForceField *>(obj)->queueCollision(static_cast<ForceField *>(obj), this, vec);
		
         return;
	
      }
	
      //Phantom139: End

    * If you own any prior packs which installed my Team #'s as part of the AFPSK, you may proceed, otherwise
      follow the installation instructions in Source Changes.pdf now.



   source/T3D/player.cpp:



    * Scroll down to Point3F Player::_move(), located around line 4575. Right above the definition line, add:
    
      //Phantom139: Added FF Header

      #include "PGD/AFPSK/forceField.h"
      
//Phantom139: End

    * Then, inside the same method, scroll down to the following block of code:

      // Take into account any physical zones...
      
      for (U32 j = 0; j < physZoneCollisionList.getCount(); j++) 
      {
         
         AssertFatal(dynamic_cast<PhysicalZone*>(physZoneCollisionList[j].object), "Bad phys zone!");
         
         const PhysicalZone* pZone = (PhysicalZone*)physZoneCollisionList[j].object;
         
         if (pZone->isActive())
            
            mVelocity *= pZone->getVelocityMod();
      
      }


    * After that block of code, add the following block of code:

      //Phantom139: Strip out the ForceField if this player may pass through.
		
      if(collisionList.getCount() != 0) {
			
         for(S32 i = 0; i < collisionList.getCount(); i++) {
				
            if(dStrcmp(collisionList[i].object->getClassName(), "ForceField") == 0) {
					
               if(static_cast<ForceField *>(collisionList[i].object) != NULL) {
						
                  ForceField *f = static_cast<ForceField *>(collisionList[i].object);
						
                  if(!f->shouldCollide(this)) {
							
                     //Byebye...
							
                     collisionList.remove(i);
						
                  }
					
               }
				
            }
			
         }
		
      }
		
      //Phantom139: End



   source/T3D/vehicles/vehicle.cpp:



    * Scroll down to bool Vehicle::updateCollision(), located around line 1335. Right above the definition line, add:

     //Phantom139: Added FF Header

     #include "PGD/AFPSK/forceField.h"
     
//Phantom139: End

    * Then, inside the same method, scroll down to the following block of code:

     mCollisionList.clear();
     CollisionState *state = mConvex.findClosestState(cmat, getScale(), mDataBlock->collisionTol);
     if (state && state->dist <= mDataBlock->collisionTol) 
     {
        //resolveDisplacement(ns,state,dt);
        mConvex.getCollisionInfo(cmat, getScale(), &mCollisionList, mDataBlock->collisionTol);
     }

    * Right after this block of code, add the following:

     //Phantom139: Added
     if(mCollisionList.getCount() != 0) {
		
        for(S32 i = 0; i < mCollisionList.getCount(); i++) {
			
           if(dStrcmp(mCollisionList[i].object->getClassName(), "ForceField") == 0) {
				
              if(static_cast<ForceField *>(mCollisionList[i].object) != NULL) {
					
                 ForceField *f = static_cast<ForceField *>(mCollisionList[i].object);
					
                 if(!f->shouldCollide(this)) {
						
                    //Byebye...
						
                    mCollisionList.remove(i);
					
                 }
				
              }
			
           }
		
        }
	
     }
     //Phantom139: End



   source/T3D/item.cpp:



    * Scroll down to void Item::updatePos(), located around line 725. Right above the definition line, add:

     //Phantom139: Added FF Header
     
#include "PGD/AFPSK/forceField.h"
     
//Phantom139: End

    * Then, inside the same method, scroll down to the following block of code:

      CollisionWorkingList& rList = mConvex.getWorkingList();
      CollisionWorkingList* pList = rList.wLink.mNext;
      while (pList != &rList) {
         if ((pList->mConvex->getObject()->getTypeMask() & mask) != 0)
         {
            Box3F convexBox = pList->mConvex->getBoundingBox();
            if (testBox.isOverlapped(convexBox))
            {
               pList->mConvex->getPolyList(&sExtrudedPolyList);
            }
         }
         pList = pList->wLink.mNext;
      }

    * Right after this block of code, add the following:

      //Phantom139: Added	
      if(collisionList.getCount() != 0) {
			
         for(S32 i = 0; i < collisionList.getCount(); i++) {
				
            if(dStrcmp(collisionList[i].object->getClassName(), "ForceField") == 0) {
					
               if(static_cast<ForceField *>(collisionList[i].object) != NULL) {
						
                  ForceField *f = static_cast<ForceField *>(collisionList[i].object);
						
                  if(!f->shouldCollide(this)) {
							
                     //Byebye...
							
                     collisionList.remove(i);
						
                  }
					
               }
				
            }
			
         }
		
      }
		
      //Phantom139: End

3) Save all of the files, and compile the engine with the changes.

4) Add a ForceField instance to your game by means of the examples provided in samples.cs.

Uninstallation: 

To uninstall the pack, just reverse the operations you did in this text file, removing the changes you 
 made to the files and removing the pack files.