//Chapter 8 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

$TestObj[0] = isObject($TestObj[0]) ? $TestObj[0] : new ScriptObject();
$TestObj[1] = isObject($TestObj[1]) ? $TestObj[1] : new ScriptObject();
$TestObj[2] = isObject($TestObj[2]) ? $TestObj[2] : new ScriptObject();
$TestObj[3] = isObject($TestObj[3]) ? $TestObj[3] : new ScriptObject();

function basicObjectExample() {
   %newObj = new ScriptObject() {
      class = "ScriptObject";
      number = 1;
   };
   echo("The ID of this object is "@%newObj@"\nThe number parameter is "@%newObj.number);
}

function namedObjectExample() {
   if(isObject(mySampleObject)) {
      echo("Detected existing object, deleting.");
      mySampleObject.delete();
   }
   new ScriptObject(mySampleObject) {
      number = 1;
   };
   %id = nameToID("mySampleObject");
   echo("The ID of this object is "@%id);
}

function applySettingExample(%newNumber) {
   %newObj = new ScriptObject() {
      class = "ScriptObject";
      number = 1;
   };
   %newObj.number = %newNumber;
   echo("The ID of this object is "@%newObj@"\nThe number parameter is now "@%newObj.number);
}

function newParameterExample(%newName) {
   %newObj = new ScriptObject() {
      class = "ScriptObject";
   };
   %newObj.myName = %newName;
   echo("The ID of this object is "@%newObj@"\My name is "@%newObj.myName);
}

function objectMath(%objectsToSpawn) {
   for(%i = 0; %i < %objectsToSpawn; %i++) {
      %newObj[%i] = new ScriptObject() {
         class = "ScriptObject";
      };
      %newObj[%i].number = getRandom(0, 1000);
      echo("Math Object "@%i+1@": ID:"@%newObj[%i]@", Number:"@%newObj[%i].number);
   }
   echo("Spawned "@%i+1@" objects., Performing addition.");
   %result = 0;
   for(%i = 0; %i < %objectsToSpawn; %i++) {
      %result += %newObj[%i].number;
   }
   echo("The Answer Is "@%result);
}

function objectMathExampleTwo(%objectsToSpawn) {
   for(%i = 0; %i < %objectsToSpawn; %i++) {
      %newObj[%i] = new ScriptObject() {
         class = "ScriptObject";
      };
   }
   echo("Spawned "@%i+1@" objects., Performing addition.");
   %total = 0;
   for(%i = 0; %i < %objectsToSpawn; %i++) {
      performObjectAddition(%newObj[%i]);
      %total += %newObj[%i].number;
   }
   echo("Addition complete, result is: "@%total);
}

function performObjectAddition(%objRef) {
   %objRef.number = getRandom(0, 1000);
}

function basicClassExample() {
   %newObj = new ScriptObject() {
      class = "MyClass";
   };
   echo("The ID of this object is "@%newObj);
}

function basicMultipleInheritanceExample_One() {
   %newObj = new ScriptObject() {
      class = "MyClass";
      superclass = "ScriptObject"; //<- Demonstration purposes only.
      numberVal = 10;
      name = "Bob";
      objType = "Basic";
   };
}

function basicMultipleInheritanceExample_Two() {
   %newObj = new ScriptObject() {
      class = "MyClass";
      superclass = "ScriptObject"; //<- Demonstration purposes only.
      numberVal = 10;
      name = "Bob";
      objType = "Basic";
   };
   %newObjTwo = new ScriptObject() {
      class = "MySecondClass";
      superclass = "MyClass";
      numberVal = 20;
      coolField = true;
   };
}

function directInheritanceExample() {
   %newObj = new ScriptObject(ObjectOne) {
      class = "MyClass";
      numberVal = 10;
      name = "Bob";
   };
   //Create the second instance, inheriting the properties of the first.
   %newObjTwo = new ScriptObject(ObjectTwo : ObjectOne) {
      class = "MyClass";
      numberVal2 = 20;
   }
   echo("Inherited Properties: "@%newObjTwo.numberVal@", Name: "@%newObjTwo.name);
   echo("Non-Inherited Properties: "@%newObjTwo.numberVal2);
}

function basicObjectMethodExample() {
   %newObj = new ScriptObject() {
      class = "MyClass";
      number = 1;
   };
   echo("Generating a new number for object "@%newObj);
   %newObj.generateNewNumber(0, 1000);
   echo("The number is now "@%newObj.number);
}

function MyClass::generateNewNumber(%this, %miniumumValue, %maximumValue) {
   %this.number = getRandom(%miniumumValue, %maximumValue);
}

function basicObjectReturnMethodExample() {
   %newObj = new ScriptObject() {
      class = "MyClass";
      number = 1;
   };
   echo("Generating a new number for object "@%newObj);
   %newNumber = %newObj.generateNewNumberTwo(0, 1000);
   %newObj.number = %newNumber;
   echo("The number is now "@%newObj.number);
}

function MyClass::generateNewNumberTwo(%this, %miniumumValue, %maximumValue) {
   %newNumber = getRandom(%miniumumValue, %maximumValue);
   return %newNumber; //Alternatively, this could just be 'return getRandom(%minimumValue, %maximumValue);'
}

function timedObjectMethod_Example1() {
   %newObj = new ScriptObject() {
      class = "MyClass";
      number = 1;
   };
   echo("We currently have: "@%newObj.number);
   %newObj.schedule(1000, 'generateNewNumberTimer', 0, 1000);
}

function MyClass::generateNewNumberTimer(%this, %miniumumValue, %maximumValue) {
   %this.number = getRandom(%miniumumValue, %maximumValue);
   echo("Now we have "@%this.number);
}

function timedObjectMethod_Example2() {
   %newObj = new ScriptObject() {
      class = "MyClass";
      number = 1;
   };
   echo("We currently have: "@%newObj.number);
   %newObj.checkSchedule = %newObj.schedule(1000, 'generateNewNumberTimer2', 0, 1000);
}

function MyClass::generateNewNumberTimer2(%this, %miniumumValue, %maximumValue) {
   %this.minVal = %minimumValue;
   %this.maxVal = %maximumValue;
   %this.number = getRandom(%miniumumValue, %maximumValue);
   echo("Now we have "@%this.number);
   %this.checkSchedule = %this.schedule(1000, 'generateNewNumberTimer2', %this.minVal, %this.maxVal);
   %this.schedule(250, 'checkValue');
}

function MyClass::checkValue(%this) {
   if(%this.number > (%this.maxVal / 2)) {
      cancel(%this.checkSchedule);
   }
}

function simGroupExample_One() {
   %group = new SimGroup() { };
   return %group;
}

function simSetExample_One() {
   %group = new SimSet() { };
   return %group;
}

function simSetExample_Two() {
   %group = new SimSet() { };
   for(%i = 0; $TestObj[%i] !$= ""; %i++) {
      %group.add($TestObj[%i]);
   }
}

function simGroupExample_Two() {
   %group = new SimGroup() { };
   for(%i = 0; $TestObj[%i] !$= ""; %i++) {
      if(%group.acceptsAsChild($TestObj[%i])) {
         %group.add($TestObj[%i]);
      }
      else {
         error("Cannot add object "@$TestObj[%i]@" on index "@%i@", it belongs to another group.");
      }
   }
}

function simSetExample_Three() {
   %group = new SimSet() { };
   for(%i = 0; $TestObj[%i] !$= ""; %i++) {
      %group.add($TestObj[%i]);
   }
   //Remove the third object.
   if(%group.isMember($TestObj[3])) {
      %group.remove($TestObj[3]);
   }
}

function simSetExample_Four() {
   %group = new SimSet() { };
   for(%i = 0; $TestObj[%i] !$= ""; %i++) {
      %group.add($TestObj[%i]);
   }
   echo("Cleaning the set.");
   %group.clear();
}

function simSetExample_Five() {
   %group = new SimSet() { };
   for(%i = 0; $TestObj[%i] !$= ""; %i++) {
      %group.add($TestObj[%i]);
   }
   //Get the objects...
   for(%i = 0; %i < %group.getCount(); %i++) {
      %obj = %group.getObject(%i);
      echo("Object at Index "@%i@": "@%obj);
      //Do work here...
   }
}

function simSetExample_Six() {
   %group = new SimSet() { };
   for(%i = 0; $TestObj[%i] !$= ""; %i++) {
      %group.add($TestObj[%i]);
   }
   //fetch an object.
   %obj = %group.getRandom();
   if(%obj != -1) {
      %index = %group.getObjectIndex(%obj);
      echo("Random has pulled the object at index "@%index@" from the group: "@%obj);
   }
   else {
      error("No objects in the group.");
   }
}

function helloWorld(%moreText) {
   echo("Hello World! "@%moreText);
}

function packageDemo1(%moreText) {
   helloWorld(%moreText);
   if(!isActivePackage(hwOverload)) {
      activatePackage(hwOverload);
   }
   helloWorld(%moreText);
   deactivatePackage(hwOverload);
}

package hwOverload {
   function helloWorld(%moreText) {
      echo("Hello World, from inside the package, also: "@%moreText);
   }
};

function initMathDemoObject() {
   %mObj = new ScriptObject() {
      class = "MathObject";
   };
   %mObj.num1 = getRandom(1, 1000);
   %mObj.num2 = getRandom(1, 1000);
   return %mObj;
}

function MathObject::performMathematics(%this) {
   return %this.num3 $= "" ? (%this.num1 + %this.num2) : (%this.num1 + %this.num2 + %this.num3);
}

function packageDemo2() {
   %mObj = initMathDemoObject();
   echo("Math Demo 1: "@%mObj.performMathematics());
   if(!isActivePackage(mathOverload)) {
      activatePackage(mathOverload);
   }
   echo("Math Demo 2: "@%mObj.performMathematics());
   %mObj.delete();
   %mObj = -1; //Set to no object, just to be safe.
   //Now, generate a second object from inside the package.
   %mObj = initMathDemoObject();
   echo("Math Demo 2 [2]: "@%mObj.performMathematics());
   deactivatePackage(mathOverload);
   echo("Math Demo 1 [2]: "@%mObj.performMathematics());
   %mObj.delete();
}

package mathOverload {
   function initMathDemoObject() {
      %mObj = parent::initMathDemoObject();
      %mObj.num3 = getRandom(1, 1000);
      return %mObj;
   }

   function MathObject::performMathematics(%this) {
      %addResult = parent::performMathematics(%this);
      %multResult = %this.num3 $= "" ? (%this.num1 * %this.num2) : (%this.num1 * %this.num2 * %this.num3);
      return %multResult;
   }
};


function baseFunction() {
   echo("base.");
}

function packageDemo3() {
   baseFunction();
   activatePackage(baseOverload1);
   baseFunction();
   activatePackage(baseOverload2);
   baseFunction();
   deactivatePackage(baseOverload1); //<- What happens here?
   baseFunction();
}

package baseOverload1 {
   function baseFunction() {
      echo("package 1.");
   }
};

package baseOverload2 {
   function baseFunction() {
      echo("package 2.");
   }
};

function MathObject::onAdd(%this) {
   parent::onAdd(%this);
   echo("Adding a MathObject. Num1: "@%this.num1@", Num2: "@%this.num2);
}
