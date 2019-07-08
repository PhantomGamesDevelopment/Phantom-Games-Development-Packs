//Chapter 7 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

function arrayExample(%numberOfElements) {
   //Start by populating the array
   for(%i = 0; %i < %numberOfElements; %i++) {
      %myArray[%i] = getRandom(1, 100);
   }
   //Print them... but backwards! Quick review: Does %i above go out of scope at this location? Answer: No, if you printed %i right
   // here it would match %numberOfElements
   for(%i = %numberOfElements; %i >= 0; %i--) {
      echo("Array Index:"@%i@": Value: "@%myArray[%i]);
   }
}

$EnemyTypes = 5;
function fetchHighestValue() {
   for(%i = 0; %i < $EnemyTypes; %i++) {
      %enemyScore[%i] = getRandom(100, 5000);
   }
   //Fetch the highest.
   %highest = 0;
   %highIndex = -1;
   for(%i = 0; %i < $EnemyTypes; %i++) {
      if(%enemyScore[%i] > %highest) {
         %highest = %enemyScore[%i];
         %highIndex = %i;
      }
   }
   echo("Highest Enemy Score is: "@%highest@" located at index "@%highIndex);
}

function basic2DArrayLoop() {
   %my2DArray[0][0] = 0;
   %my2DArray[0][1] = 1;
   %my2DArray[1][0] = 2;
   %my2DArray[1][1] = 3;
   for(%i = 0; %i <= 1; %i++) {
      for(%j = 0; %j <= 1; %j++) {
         echo("Printing %my2DArray at ["@%i@"]["@%j@"]: "@%my2DArray[%i][%j]);
      }
   }
}

function another2DArrayLoop() {
   %highestJ = 2;
   %my2DArray[0][0] = 0;
   %my2DArray[0][1] = 1;
   %my2DArray[0][2] = 2;
   %my2DArray[1][0] = 2;
   %my2DArray[1][1] = 3;
   for(%i = 0; %i <= 1; %i++) {
      for(%j = 0; %j <= %highestJ; %j++) {
         if(%my2DArray[%i]%j] $= "") {
            break; //<- This cancels out the %j loop, but leaves the %i loop running.
         }
         echo("Printing %my2DArray at ["@%i@"]["@%j@"]: "@%my2DArray[%i][%j]);
      }
   }
}

$myConstantNumber = 27;
$numberOfCalls = 0;
function globalVariableBasics() {
   $numberOfCalls++;
   echo("The global variable $myConstantNumber is: "@$myConstantNumber@", you have called this function "@$numberOfCalls@" times.");
}

$Statuses::Fire = 0;
$Statuses::Ice = 1;
$Statuses::Electric = 2;
function setStatusEffect() {
   %effectToApply = getRandom(0, 2);
   switch(%effectToApply) {
      case 0:
         %e = $Statuses::Fire;
      case 1:
         %e = $Statuses::Ice
      case 2:
         %e = $Statuses::Electric;
   }
   return %e;
}

function sayHello(%moreText) {
   echo("Hello Computer...");
   schedule(2500, 0, 'helloWorld', %moreText);
}

function helloWorld(%moreText) {
   echo("hello world!" @ " " @ %moreText);
}

function timerLoop(%counter) {
   if(%counter < -100 && isEventPending($schedRef)) {
      echo("Number is too far away, killing schedule.");
      cancel($schedRef);
      return;  //<- Don't forget this return, otherwise the function will continue!
   }
   if(%counter > 10) {
      echo("I'm done!");
      return;
   }
   echo("Counter At "@%counter);
   $schedRef = schedule(1000, 0, 'timerLoop', (%counter + 1));
}

function getWordExample() {
   %string = "This is a nice long string instance";
   %word4 = getWord(%string, 4); //<- "long";
   echo("The fourth \"word\" in "@%string@" is "@%word4); //<- The \" termination sequence allows you to insert a " character in a string without needing to close it.
}

function getWordsExample() {
   %string = "This is a nice long string instance";
   %firstThree = getWords(%string, 0, 2);
   echo("The first three \"words\" in "@%string@" are "@%firstThree);
}

function setWordExample() {
   %string = "This is a nice long string instance";
   %newStr = setWord(%string, 3, "very"); //<- Replace the word nice with the word very.
   echo("The new string is: "@%newStr);
}

function removeWordExample() {
   %string = "This is a nice long string instance";
   %newStr = removeWord(%string, 3); //<- Remove the word nice from the string.
   echo("After removing word 3, we have: "@%newStr);
}

function getWordCountExample() {
   %string = "This is a nice long string instance";
   for(%i = 0; %i < getWordCount(%string); %i++) {
      echo("Word "@%i@": "@getWord(%string, %i);
   }
}

function fieldsExample() {
   %string = "String One\tString Two\tString Three";
   %field1 = getField(%string, 1); //<- String Two
   %first2 = getFields(%string, 0, 1);
   %newField1 = setField(%string, 1, "String #2");
   %minus2 = removeField(%string, 2);
   %fCount = getFieldCount(%string);
   for(%i = 0; %i < %fCount; %i++) {
      echo("Field "@%i@": "@getField(%string, %i));
      for(%j = 0; %j < getWordCount(getField(%string, %i)); %j++) {
         echo("Word "@%j@": "@getWord(getField(%string, %i), %j));
      }
   }
}
