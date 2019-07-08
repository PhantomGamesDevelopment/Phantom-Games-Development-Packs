<?php
     /**
        Install.php
        By: Robert C. Fritzen (Phantom139)
        (C) 2014, Phantom Games Development
        
        This PHP Script will guide you through the installation process for the
        cryptography setup for the Multiplayer Assembly Package
        
        I recommend removing this file from the server after successful installation.
        Only use this file if you are creating additional projects or need to do a
        fresh installation.
     **/
     
     ini_set('max_execution_time', 0);
     require("admin/sources/php/Crypt/RSA.php");
     
     $currentStep = $_POST["currentInstallStep"];
     if(strcmp($currentStep, "") == 0) {
        $currentStep = 0;
     }
     
     switch($currentStep) {
         case 0:
             //Get Project Info Step.
             getProjectInfo();
             return;
             
         case 1:
             //Test Connection To SQL, Create Folders.
             $project = $_POST["projName"];
             $db = $_POST["sqlDatabase"];
             $us = $_POST["sqlUser"];
             $pw = $_POST["sqlPass"];
             testProjectInfo($project, $db, $us, $pw);
             return;
             
         case 2:
             //Create SQL Tables, Select RSA CA Key Strength
             $proj = $_POST["proj"];
             $db = $_POST["db"];
             $us = $_POST["us"];
             $pw = $_POST["pw"];
             handleSQLOperations($proj, $db, $us, $pw);
             return;
             
         case 3:
             //Generate RSA Keypair.
             $proj = $_POST["proj"];
             $rsaStr = $_POST["rsaStrength"];
             doRSAKeyGen($proj, $rsaStr);
             return;
             
         case 4:
             pushComplete();
             return;
     }


     function getProjectInfo() {
         echo "<b>Cryptography Installation: Project Information<br/>STEP 1 / 5</b><p>";
         echo "This PHP script will guide you through the SQL installation portion of the PHP side of the cryptography module. ".
         "Please create a MySQL database and a MySQL user and attach it to the database. Please also provide a project name to identify ".
         "your game project for the authentication system.";
         echo "<form method=\"post\" name=\"prog\" action=\"#\">".
         "Project Name (No Spaces):<br />".
         "<input type=\"text\" name=\"projName\" size=\"25\" value=\"\"><br /><br />".
         "Database:<br />".
         "<input type=\"text\" name=\"sqlDatabase\" size=\"25\" value=\"\"><br /><br />".
         "SQL User:<br />".
         "<input type=\"text\" name=\"sqlUser\" size=\"25\" value=\"\"><br /><br />".
         "SQL Password:<br />".
         "<input type=\"text\" name=\"sqlPass\" size=\"25\" value=\"\"><br /><br />".
         "<input type=\"hidden\" name=\"currentInstallStep\" value=\"1\"/>".
         "<input type=\"submit\" name=\"test\" value=\"Test Settings\"></form>";
     }
     
     function testProjectInfo($projectName, $dbName, $userName, $password) {
         echo "<b>Cryptography Installation: Testing SQL Connection, Creating Project Files<br/>STEP 2 / 5</b><p>";
         if(strlen($projectName) < 3) {
            die("Please input a project name that is at or above 3 characters.");
         }
         $projectName = str_replace(" ", "", $projectName);
         echo "Testing MySQL Connection: ".$dbName.", ".$userName."<br/>";
         /* check connection */
         if (mysqli_connect_errno()) {
             printf("Connect failed: %s\n", mysqli_connect_error());
             echo "<form method=\"post\" name=\"prog\" action=\"#\">".
             "<input type=\"hidden\" name=\"currentInstallStep\" value=\"0\"/>".
             "<input type=\"submit\" name=\"test\" value=\"Re-Enter Data\"></form>";
             return -1;
         }
         else {
             echo "Connection to MySQL Successful, Creating Directory for Project.<br/>";
             mkdir($projectName);
             //Copy the template files.
             if (version_compare(phpversion(), '5.3.3', '<')) {
                if(!copy("templateProject/index_php52.php", $projectName."/index.php")) {
                   die("Failed to copy index.php, please re-attempt.");
                }
             }
             else {
                if(!copy("templateProject/index_php53.php", $projectName."/index.php")) {
                   die("Failed to copy index.php, please re-attempt.");
                }
             }
             //Copy the functions lib
             if(!copy("templateProject/pgdFunctions.php", $projectName."/pgdFunctions.php")) {
                die("Failed to copy pgdFunctions.php, please re-attempt.");
             }
             echo "File moving successful. Click to proceed to next step.<p>";
             echo "<form method=\"post\" name=\"prog\" action=\"#\">".
             "<input type=\"hidden\" name=\"currentInstallStep\" value=\"2\"/>".
             "<input type=\"hidden\" name=\"proj\" value=\"".$projectName."\"/>".
             "<input type=\"hidden\" name=\"db\" value=\"".$dbName."\"/>".
             "<input type=\"hidden\" name=\"us\" value=\"".$userName."\"/>".
             "<input type=\"hidden\" name=\"pw\" value=\"".$password."\"/>".
             "<input type=\"submit\" name=\"test\" value=\"Proceed to Next Step\"></form>";
         }
     }
     
     function handleSQLOperations($proj, $db, $us, $pw) {
         echo "<b>Cryptography Installation: SQL Work, Project Settings Work, CA Key<br/>STEP 3 / 5</b><p>";
         echo "Attempting To Create Accounts Table...<p>";
         $mysqli = new mysqli("localhost", "$us", "$pw", "$db");

         $sql = "CREATE TABLE Accounts".$proj."(guid INT(9),E TEXT,N TEXT,D TEXT,D_DEC_HASH TEXT,signature TEXT,username TEXT,email TEXT,hashpass TEXT)";
         if (mysqli_query($mysqli, $sql)) {
             echo "Table Creation Success, Attempting To Create AccountKeys Table...<p>";
             $sql2 = "CREATE TABLE AccountKeys".$proj."(Email TEXT,AccKey TEXT,PurchaseCode TEXT)";
             if (mysqli_query($mysqli, $sql2)) {
                 echo "Table Creation Success, Generating Project Settings File...";
                 $XXZVariables["database"][$proj] = $db;
                 $XXZVariables["dbAccess"][$proj] = "localhost";
                 $XXZVariables["db_username"][$proj] = $us;
                 $XXZVariables["db_password"][$proj] = $pw;
                 $XXZVariables["account_table"][$proj] = "Accounts".$proj;
                 $XXZVariables["key_table"][$proj] = "AccountKeys".$proj;
                 $XXZVariables["AK_BindEmail"][$proj] = 1;
                 $my_file = $proj . "/projectSettings.php";
                 $data = '$XXZVariables = ' . var_export($XXZVariables, true) . ';';
                 $contents = "<?PHP\n"."$"."AuthOnline = 1;\n".$data."\nglobal "."$"."XXZVariables;\nglobal "."$"."AuthOnline\n?>";
                 file_put_contents($my_file, $contents);
                 echo "Project Settings File Created Successfully...<p>";
                 echo "Please Select a RSA CA Keypair Strength.<br/>The Higher the value, the more secure your accounts will be, however, it will take more processing to both ".
                 "generate the accounts, and to use the various encryptions on the accounts. It is recommended to choose a value in between for best performance, although anything ".
                 "over 1024 is generally adequate enough for modern day security purposes.<br/>";
                 echo "<form method=\"POST\" action=\"\"><input type=\"hidden\" name=\"currentInstallStep\" value=\"3\"/><input type=\"hidden\" name=\"proj\" value=\"".$proj."\"/><SELECT name=\"rsaStrength\">";
                 echo "<OPTION value=512> RSA 512<OPTION value=768> RSA 768<OPTION value=1024> RSA 1024<OPTION value=2048> RSA 2048<OPTION value=4092> RSA 4092";
                 echo "</select><input type=\"submit\" name=\"Submit\" />";
             }
             else {
                 die("Failed to create table AccountKeys".$proj.", Please check the permissions of the MySQL user on the Database, and re-try this installation.");
             }
         }
         else {
             die("Failed to create table Accounts".$proj.", Please check the permissions of the MySQL user on the Database, an re-try this installation.");
         }
     }
     
     function doRSAKeyGen($proj, $rsaStr) {
         echo "<b>Cryptography Installation: CA Key Creation, Final Steps...<br/>STEP 4 / 5</b><p>";
         if(!isSet($rsaStr)) {
             echo "Please Select a RSA CA Keypair Strength.<br/>The Higher the value, the more secure your accounts will be, however, it will take more processing to both ".
             "generate the accounts, and to use the various encryptions on the accounts. It is recommended to choose a value in between for best performance, although anything ".
             "over 1024 is generally adequate enough for modern day security purposes.<br/>";
             echo "<form method=\"POST\" action=\"\"><input type=\"hidden\" name=\"currentInstallStep\" value=\"3\"/><input type=\"hidden\" name=\"proj\" value=\"".$proj."\"/><SELECT name=\"rsaStrength\">";
             echo "<OPTION value=512> RSA 512<OPTION value=768> RSA 768<OPTION value=1024> RSA 1024<OPTION value=2048> RSA 2048<OPTION value=4092> RSA 4092";
             echo "</select><input type=\"submit\" name=\"Submit\" />";
             return;
         }

         echo "Please Wait... Generating RSA Keypair (This Process may take up to 3 minutes)<p>";
         define('MATH_BIGINTEGER_MODE', MATH_BIGINTEGER_MODE_INTERNAL);
         $rsa = new Crypt_RSA();
         extract($rsa->createKey($rsaStr));
         
         $textText = "PGDMultiplayerPack";
         $rsa->loadKey($privatekey);
         $rsa->setSignatureMode(CRYPT_RSA_SIGNATURE_PKCS1);
         $signature = $rsa->sign($textText);

         $rsa->loadKey($publickey);
         $fails = 0;
         echo "Verifying RSA Key<br/>";
         while($rsa->verify($textText, $signature) != 1) {
             echo "<font color=\"red\">Signature not verified, re-attempting generation...</font><p>";
             extract($rsa->createKey($rsaStr));
             $rsa->loadKey($privatekey);
             $rsa->setSignatureMode(CRYPT_RSA_SIGNATURE_PKCS1);
             $signature = $rsa->sign($textText);
             
             $fails++;
             if($fails >= 5) {
                 die("RSA Generator Failed Too Many Times. Please Proceed to Manual Installation using: http://www.phantomdev.net/certAuthorityCreate.php");
             }
         }
         $pubKey = $publickey;
         $priKey = $privatekey;
         
         $keyHex = $rsa->modulus->toHex();

         $pubKeyOut = str_replace("\\r\\n", "\n", $pubKey);
         $priKeyOut = str_replace("\\r\\n", "\n", $priKey);
         echo "<font color=\"green\">Signature Verified, Inserting Keypair into PGDFunctions.php and generating C++ code.</font>";
         
         $file = @file_get_contents($proj."/pgdFunctions.php");
         if($file) {
             $file = str_replace("YOUR PRIVATE KEY HERE", $priKeyOut, $file);
             $file = str_replace("YOUR PUBLIC KEY HERE", $pubKeyOut, $file);
             //Save file.
             file_put_contents($proj."/pgdFunctions.php", $file);
             //Proceed.
             echo "To complete installation on the C++ you will need to adjust cryptoPackage.cpp to contain the folowing definition for your public key:<br/>".
             "Please replace the definition of xxz568::caPublic() with the following: <br/><textarea name=\"textarea\" cols=\"150\" rows=\"3\" wrap=\"VIRTUAL\">".
             "const char * xxz568::caPublic() {\n".
             "   return \"".$keyHex."\";\n".
             "}\n".
             "</textarea><p>";
             echo "Step Success, Please Proceed to Complete Installation.<p>";
             echo "<form method=\"post\" name=\"prog\" action=\"#\">".
             "<input type=\"hidden\" name=\"currentInstallStep\" value=\"4\"/>".
             "<input type=\"submit\" name=\"test\" value=\"Complete Installation\"></form>";
         }
         else {
             echo "Cannot locate ".$proj."/pgdFunctions.php, please verify that this file exists, and then click the button below.<br/>";
             echo "<form method=\"POST\" action=\"\"><input type=\"hidden\" name=\"currentInstallStep\" value=\"3\"/><input type=\"hidden\" name=\"proj\" value=\"".$proj."\"/><SELECT name=\"rsaStrength\">";
             echo "<OPTION value=512> RSA 512<OPTION value=768> RSA 768<OPTION value=1024> RSA 1024<OPTION value=2048> RSA 2048<OPTION value=4092> RSA 4092";
             echo "</select><input type=\"submit\" name=\"Submit\" />";
             return;
         }
     }
     
     function pushComplete() {
         echo "<b>Cryptography Installation: Installation Complete.<br/>STEP 5 / 5</b><p>";
         echo "Thank you for using the Cryptography Module installer script written by Robert C. Fritzen (Phantom139). If you have encountered ".
         "any problems using this script to aid the installation process, please contact me at rfritzen@phantomdev.net. I hope you enjoy the ".
         "Multiplayer Assembly Package for Torque 3D MIT and thank you for your purchase!";
         
         echo "<p><br/><b>Please Delete This File Now. You may re-upload it to your server in the case you need to install another auth system.</b>";
     }
?>
