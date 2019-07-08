//Chapter 12 Code Samples
//The Ultimate Guide To Torque 3D
// By Robert C. Fritzen

function tagExample1() {
   %basicString = "This is a string";
   %basicTaggedString = 'This is a string';
   echo("Str: "@%basicString@", Tagged: "@%basicTaggedString);
}

function tagExample2() {
   %basicString = "This is a string";
   %basicTaggedString = addTaggedString(%basicString);
   echo("Str: "@%basicString@", Tagged: "@%basicTaggedString);
   %removedTag = getTaggedString(%basicTaggedString);
   echo("Decompressed: "@%removedTag);
}

//Server Functions (Put these on the server side):
function networkExample1(%targetClient) {
   if(%targetClient.stopSendingEvents) {
      %targetClient.stopSendingEvents = 0;
      return;
   }
   commandToClient(%targetClient, 'TestEvent1', "Hello World");
   schedule(2500, 0, networkExample1, %targetClient);
}

function serverCmdRecieveEvent1(%client, %message) {
   echo("Client "@%client@" says: "@%message);
}

function serverCmdStopBuggingMe(%client) {
   %client.stopSendingEvents = 1;
}

//Client Functions (Put these on the client side):
function stopMessages() {
   commandToServer('StopBuggingMe');
}

function clientCmdTestEvent1(%message) {
   echo("Got "@%message@" from the server, responding.");
   commandToServer('RecieveEvent1', "I got it!");
}

