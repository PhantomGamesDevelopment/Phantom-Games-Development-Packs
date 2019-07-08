Operation of the guiObjectView remains exactly the same as before with some changes:

All of the functions that are related to the mounting of objects now involve an 'index' parameter. The 
change set adds a Vector<> to all of the existing mounting operations, so this now is treated as an array
of mounts. They are not sorted in any ways shape or form, so you cannot access them by means of using 
common ideas. 

The following TS function has been removed from the control:
%gui.setMount()

Here are the new functions:

%gui.getMountedModel(%index): returns the name of the mounted model in index slot %index

%gui.setMountedModel(%index, %pathToShape): sets a model in %index to %pathToShape, You need to initalize
the field in %index first using the new addMountObject() function.

%gui.addMountObject(%pathToShape, %node, %skin): initialize a new array element using some standard fields,
the %node and %skin fields can be safely ignored, however proper model initialization will likely require
you to at least set a node after creation.

%gui.removeMount(%index): remove the object in %index.

%gui.getMountNode(%index): returns the string containing which node the object is mounted to in %index

%gui.setMountNode(%index, %nodeNumber, %nodeName): set the mount location for %index, you can set 
%nodeNumber to -1 to use %nodeName instead, or simply use %nodeNumber, allowing %nodeName to be ignored.

%gui.indexFromNode(%nodeNumber, %nodeName): returns the array index that is being used by the mounted
object in %nodeNumber, set %nodeNumber to -1 to use %nodeName.

%gui.getMountSkin(%index): returns the skin being used by the object in %index.

%gui.setMountSkin(%index, %skin): set the skin for the object in %index.

Happy Mounting!
~Phantom139