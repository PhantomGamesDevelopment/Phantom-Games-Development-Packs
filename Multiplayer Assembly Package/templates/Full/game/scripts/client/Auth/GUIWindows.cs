//Auth/GUIWindows.cs
//Robert Fritzen (Phantom139)
//(c)Phantom Games Development 2011
//Central Script File for GUI actions

//====================================================
//Login Window Functions
//====================================================
function LoginWindow::onWake(%this) {
   //trace(1);
   TestServerList();
   
   %ok = 0;
   //activate net port (TCP objects need this???)
   for(%i = 27950; %i < 28100; %i++) {
      if(setNetPort(%i)) {
         %ok = 1;
         break;
      }
   }
   
   if(!%ok) {
      error("* Potential Error: No open network ports.");
   }
   
   AccountSelector_DropBox.populate();
   %this.nextAccount = 1;
}

function LoginWindow::recoverAccountAndLogin(%this, %password) {
   //MessagePopup("Please Wait", "Attempting account recovery, standby...");
   echo("Doing Login!");
   if(TestServerList() !$= "yes") {
      %this.schedule(5000, "recoverAccountAndLogin", %password);
   }
   else {
      %user = recUsernameEnter.getText();
      TCP_DoLoginRecovery(%user, %password);
   }
}

function LoginWindow::doLogin(%this) {
   %accID = AccountSelector_DropBox.getSelected();
   %password = passwordEnter.getText();
   if(%accID == 0) {
      //recover selected account
      %this.recoverAccountAndLogin(%password);
   }
   else {
      %user = $Auth::AccountName[%accID - 1];
      %public = getPublicCertificate(%user);
      %private = getPrivateCertificate(%user);
      
      echo(%user);
      if(%public $= "NOT_FOUND" || %private $= "NOT_FOUND") {
         MessageBoxOk("Certificate Corruption Error", "Your certificate file appears to be missing or incomplete, please recover the account to address this.");
         return;
      }
      //split the fields we need
      %e = getField(%public, 3);
      %n = getField(%public, 4);
      %d = getField(%private, 1);
      %sig = getField(%public, 5);
      //
      //attempt the login
      %decHash = getSubStr(%d, 0, strstr(%d, ":"));
      %encD = getSubStr(%d, strLen(%decHash) + 1, strLen(%d));
      //
      //Invoke the account decryption here
      %attempt = DecryptAccount(%encD, %decHash, %password);
      if(%attempt !$= "INVALID_PASSWORD") {
         StoreSuccessfulLogin(%e, %n, %sig);

         $ConnStore::username = %user;
         $ConnStore::email = getField(%public, 2);
         $ConnStore::guid = getField(%public, 1);
         $ConnStore::sig = %sig;
         //push the main menu
         $PublicCert = %public; //keep it stored here for usage in servers
         Canvas.setContent(MainMenuGui);
         
			$pref::Player::Name = $ConnStore::username;
      }
      else {
         MessageBoxOK("Invalid Password", "You have entered the incorrect password for this account");
      }
   }
}

function AccountSelector_DropBox::populate(%this) {
   getAccountList();

   %this.clear();

   %this.add("Recover Account", 0);
   %this.setSelected(0);
   //%this.onSelect(0, "Recover Account");
   
   %account = 0;
   while($Auth::AccountName[%account] !$= "") {
      %this.add($Auth::AccountName[%account], %account + 1);
      %this.setSelected(%account + 1);
      %account++;
   }
}

function AccountSelector_DropBox::onSelect( %this, %id, %text ) {
   if(%id == 0) {
      recUsernameEnter.setHidden(false);
      ML_recoveruser.setHidden(false);
   }
   else {
      recUsernameEnter.setHidden(true);
      ML_recoveruser.setHidden(true);
   }
   
   %this.setText(%text);
}

//====================================================
//Account Registration Window Functions
//====================================================
function AccountRegistrationWindow::onWake(%this) {
   $Authentication::InReg = 1;
   //check the server
   %_OK = "art/gui/tickMark_OK.png";
   %_BAD = "art/gui/tickMark_BAD.png";
   if(TestServerList() $= "yes") {
      //set the gui as properly needed
      AS_StatIcon.setBitmap(%_OK);
      AS_StatIcon.tooltip = "";
      AS_Stat.setText("<Font:Arial:16>Server Status: Ready To Register Accounts");
   }
   else {
      AS_StatIcon.setBitmap(%_BAD);
      AS_StatIcon.tooltip = "No Servers";
      AS_Stat.setText("<Font:Arial:16>Server Status: No Validated Servers Found, Please Try Again Later");
   }
   //set up bad tickmarks
   nameCheckIcon.setBitmap(%_BAD);
   nameCheckIcon.tooltip = "You need to enter a username";
   PasswordCheck.setBitmap(%_BAD);
   PasswordCheck.tooltip = "You need to enter two matching passwords";
   EmailCheck.setBitmap(%_BAD);
   EmailCheck.tooltip = "You need to enter two matching emails";
   KeyCheck.setBitmap(%_BAD);
   KeyCheck.tooltip = "You need to enter your account key";
   TOUCheckIcon.setBitmap(%_BAD);
   TOUCheckIcon.tooltip = "You must agree to the terms of registration";
   //fill the EULA
   %file = new FileObject();
   %file.openForRead("PRR_LA.txt");
   while(!%file.isEof()) {
      %buf = %buf @ %file.readLine() @ " ";
   }
   TermsOfUse.setText(%buf);
   
   %this.updateLoopSched = %this.schedule(1, updateLoop);
}

function AccountRegistrationWindow::updateLoop(%this) {
   %this.onUpdateFields();
   %this.updateLoopSched = %this.schedule(2500, updateLoop);
}

function AccountRegistrationWindow::OnUpdateFields(%this) {
   %_BAD = "art/gui/tickMark_BAD.png";
   %_WARN = "art/gui/tickMark_WARN.png";
   %_OK = "art/gui/tickMark_OK.png";

   if(TestServerList() $= "yes") {
      //set the gui as properly needed
      AS_StatIcon.setBitmap(%_OK);
      AS_StatIcon.tooltip = "";
      AS_Stat.setText("<Font:Arial:16>Server Status: Ready To Register Accounts");
   }
   else {
      AS_StatIcon.setBitmap(%_BAD);
      AS_StatIcon.tooltip = "No Servers";
      AS_Stat.setText("<Font:Arial:16>Server Status: No Validated Servers Found, Please Try Again Later");
   }

   %nameChange = %this.lastName $= NameEnter.getText() ? 0 : 1;
   %pw1Change = %this.lastPW1 $= PWEnter.getText() ? 0 : 1;
   %pw2Change = %this.lastPW2 $= PWRepeat.getText() ? 0 : 1;
   %email1Change = %this.lastEMail1 $= EmailEnter.getText() ? 0 : 1;
   %email2Change = %this.lastEMail2 $= EmailRepeat.getText() ? 0 : 1;
   %keyChange = %this.lastKey $= AccountKey.getText() ? 0 : 1;
   
   %this.lastName = NameEnter.getText();
   %this.lastPW1 = PWEnter.getText();
   %this.lastPW2 = PWRepeat.getText();
   %this.lastEMail1 = EmailEnter.getText();
   %this.lastEMail2 = EmailRepeat.getText();
   %this.lastKey = AccountKey.getText();
   
   //client-side checks
   //===username stuff
   if(%nameChange) {
      if(strlen(%this.lastName) < 7) {
         nameCheckIcon.setBitmap(%_BAD);
         nameCheckIcon.tooltip = "Your selected username is too short, it must be longer than 7 characters";
      }
      //else if(!checkCurse(%this.lastName)) {
      //   nameCheckIcon.setBitmap(%_BAD);
      //   nameCheckIcon.tooltip = "You have selected an invalid username, or have placed invalid words in your username";
      //}
      else {
         nameCheckIcon.setBitmap(%_WARN);
         nameCheckIcon.tooltip = "Perform Registration to Finish Checks";
         //server-side checks
      }
   }
   //===password stuff
   if(%pw1Change || %pw2Change) {
      if(strlen(%this.lastPW1) < 6) {
         PasswordCheck.setBitmap(%_BAD);
         PasswordCheck.tooltip = "Passwords must be more than 6 characters long";
      }
      else if(%this.lastPW1 !$= %this.lastPW2) {
         PasswordCheck.setBitmap(%_BAD);
         PasswordCheck.tooltip = "Your two passwords do not match";
      }
      else if(strlen(%this.lastPW1) < 9) {
         PasswordCheck.setBitmap(%_WARN);
         PasswordCheck.tooltip = "While acceptable, this password is not considered strong, consider one longer than 9 characters";
      }
      //
      else {
         PasswordCheck.setBitmap(%_OK);
         PasswordCheck.tooltip = "";
      }
   }
   //===email stuff
   if(%email1Change || %email2Change) {
      if(strstr(%this.lastEmail1, "@") == -1 || strstr(%this.lastEmail1, ".") == -1) {
         EmailCheck.setBitmap(%_BAD);
         EmailCheck.tooltip = "You have entered an invalid email address";
      }
      else if(%this.lastEmail1 !$= %this.lastEmail2) {
         EmailCheck.setBitmap(%_BAD);
         EmailCheck.tooltip = "Your two email addresses do not match";
      }
      else {
         EmailCheck.setBitmap(%_OK);
         EmailCheck.tooltip = "";
      }
   }
   //===key stuff
   if(%keyChange) {
      if(strlen(%this.lastKey) != 25) {
         KeyCheck.setBitmap(%_BAD);
         KeyCheck.tooltip = "Invalid account key";
      }
      else {
         KeyCheck.setBitmap(%_OK);
         KeyCheck.tooltip = "";
      }
   }
   //===
   if(AccountAgreement.isStateOn()) {
      TOUCheckIcon.setBitmap(%_OK);
      TOUCheckIcon.tooltip = "";
   }
   else {
      TOUCheckIcon.setBitmap(%_BAD);
      TOUCheckIcon.tooltip = "You must agree to the terms of registration";
   }
}

function AccountAgreement::onClick(%this) {
   AccountRegistrationWindow.OnUpdateFields();
}

function AccountRegistrationWindow::OnClickCheckRegister(%this) {
   if((nameCheckIcon.tooltip $= "" || nameCheckIcon.tooltip $= "Perform Registration to Finish Checks")
      && (PasswordCheck.tooltip $= "" || PasswordCheck.tooltip $= "While acceptable, this password is not considered strong, consider one longer than 9 characters")
      && EmailCheck.tooltip $= ""
      && KeyCheck.tooltip $= ""
      && TOUCheckIcon.tooltip $= ""
      && AS_StatIcon.tooltip $= "") {
      %this.performRegistration();
   }
   else {
      MessageBoxOK("Unfinished Form", "You have not addressed all of the invalidations above, please do so and try again.");
      
      %this.onUpdateFields();
   }
}

function AccountRegistrationWindow::performRegistration(%this) {
   %Name = NameEnter.getText();
   %PW = PWEnter.getText();
   %EMail = EmailEnter.getText();
   %Key = AccountKey.getText();
   %Agree = AccountAgreement.isStateOn();
   
   if(%Name $= "" || strlen(%Name) < 7) {
      MessageBoxOK("Registration Denied", "Your username is invalid, please re-enter the username");
      return;
   }
   if(%PW $= "" || strlen(%PW) < 6) {
      MessageBoxOK("Registration Denied", "Your password is invalid, please re-enter the password");
      return;
   }
   if(%EMail $= "" || strstr(%EMail, "@") == -1 || strstr(%EMail, ".") == -1) {
      MessageBoxOK("Registration Denied", "Your email is invalid, please re-enter the email");
      return;
   }
   if(%Key $= "" || strlen(%Key) != 25) {
      MessageBoxOK("Registration Denied", "Your account key is invalid, please re-enter the key");
      return;
   }
   if(%Agree != 1) {
      MessageBoxOK("Registration Denied", "You must agree to the registration terms to register an account");
      return;
   }

   //MessagePopup("Registering", "Sending Registration Data, Please Wait...");
   TCP_PerformRegister(%Name, %PW, %EMail, %Key);
}

function AccountRegistrationWindow::OnClickClearFields(%this) {
   NameEnter.setText("");
   PWEnter.setText("");
   PWRepeat.setText("");
   EmailEnter.setText("");
   EmailRepeat.setText("");
   AccountKey.setText("");
   
   %this.OnUpdateFields();
}

function AccountRegistrationWindow::OnSleep(%this) {
   $Authentication::InReg = 0;

   %this.OnClickClearFields();
   cancel(%this.updateLoopSched);
}
