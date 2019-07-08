<?php
   //install.php
   //created for MAP 2.0, Master Server System.
   $step = $_POST["step"];
   if(strcmp($step, "") == 0) {
      $step = 0;
   }
   switch($step) {
      //get SQL Info
      case 0:
         getSQLInfo();
         return;
      
      //test SQL info
      case 1:
         $database = $_POST["sqlDatabase"];
         $sqlUser = $_POST["sqlUser"];
         $sqlPass = $_POST["sqlPass"];
         testSQLInfo($database, $sqlUser, $sqlPass);
         return;
      
      //build SQL Tables
      case 2:
         $database = $_POST["db"];
         $sqlUser = $_POST["us"];
         $sqlPass = $_POST["pw"];
         doSQLWork($database, $sqlUser, $sqlPass);
         return;
      
      //apply settings to files
      case 3:
         $database = $_POST["db"];
         $sqlUser = $_POST["us"];
         $sqlPass = $_POST["pw"];
         createSettingsFile($database, $sqlUser, $sqlPass);
         return;
      
      //done!
      case 4:
         $database = $_POST["db"];
         $sqlUser = $_POST["us"];
         $sqlPass = $_POST["pw"];
         $project = $_POST["projName"];
         finishUp($database, $sqlUser, $sqlPass, $project);
         return;
   }

   function getSQLInfo() {
      echo "<b>MAP Master Server System: SQL Install<br/>STEP 1 / 5</b><p>";
      echo "This PHP script will guide you through the SQL installation portion of the PHP master server. ".
      "First, you'll need to create a new MySQL database and account for your master server. You may also use your ".
      "existing database and account for your XXZ system (I recommend using this if you have one). IF you're installing ".
      "just the master server portion, please create the database and user with all permissions now, and enter it's info below.";
      echo "<form method=\"post\" name=\"prog\" action=\"#\">".
      "Database:<br />".
      "<input type=\"text\" name=\"sqlDatabase\" size=\"25\" value=\"\"><br /><br />".
      "SQL User:<br />".
      "<input type=\"text\" name=\"sqlUser\" size=\"25\" value=\"\"><br /><br />".
      "SQL Password:<br />".
      "<input type=\"text\" name=\"sqlPass\" size=\"25\" value=\"\"><br /><br />".
      "<input type=\"hidden\" name=\"step\" value=\"1\"/>".
      "<input type=\"submit\" name=\"test\" value=\"Test Settings\">";
   }
   
   function testSQLInfo($db, $us, $pw) {
      echo "<b>MAP Master Server System: SQL Install<br/>STEP 2 / 5</b><p>";
      echo "Testing MySQL Info...<p>Database: ".$db."<br/>Username: ".$us."<br/>Password: ".$pw."<p>";
      $mysqli = new mysqli("localhost", "$us", "$pw", "$db");

      /* check connection */
      if (mysqli_connect_errno()) {
         printf("Connect failed: %s\n", mysqli_connect_error());
         echo "<form method=\"post\" name=\"prog\" action=\"#\">".
         "<input type=\"hidden\" name=\"step\" value=\"0\"/>".
         "<input type=\"submit\" name=\"test\" value=\"Re-Enter Data\">";
         return -1;
      }
      else {
         echo "Connection Successful... Click To Continue.";
         echo "<form method=\"post\" name=\"prog\" action=\"#\">".
         "<input type=\"hidden\" name=\"step\" value=\"2\"/>".
         "<input type=\"hidden\" name=\"db\" value=\"".$db."\"/>".
         "<input type=\"hidden\" name=\"us\" value=\"".$us."\"/>".
         "<input type=\"hidden\" name=\"pw\" value=\"".$pw."\"/>".
         "<input type=\"submit\" name=\"test\" value=\"Continue\">";
      }
   }
   
   function doSQLWork($db, $us, $pw) {
      echo "<b>MAP Master Server System: SQL Install<br/>STEP 3 / 5</b><p>";
      echo "Creating MySQL Tables and Populating Information...<p>";
      $mysqli = new mysqli("localhost", "$us", "$pw", "$db");
      
      $sql = "CREATE TABLE Servers(IP TEXT,UTCExpry INT(11))";
      if (mysqli_query($mysqli, $sql)) {
         echo "Successfully created Servers table.";
         echo "<form method=\"post\" name=\"prog\" action=\"#\">".
         "<input type=\"hidden\" name=\"step\" value=\"3\"/>".
         "<input type=\"hidden\" name=\"db\" value=\"".$db."\"/>".
         "<input type=\"hidden\" name=\"us\" value=\"".$us."\"/>".
         "<input type=\"hidden\" name=\"pw\" value=\"".$pw."\"/>".
         "<input type=\"submit\" name=\"test\" value=\"Continue\">";
      }
      else {
         echo "Error creating table: " . mysqli_error($mysqli);
         echo "<form method=\"post\" name=\"prog\" action=\"#\">".
         "<input type=\"hidden\" name=\"step\" value=\"0\"/>".
         "<input type=\"submit\" name=\"test\" value=\"Re-Enter Data\">";
      }
   }
   
   function createSettingsFile($db, $us, $pw) {
      echo "<b>MAP Master Server System: SQL Install<br/>STEP 4 / 5</b><p>";
      echo "Creating Settings File...<p>";
      echo "<form method=\"post\" name=\"prog\" action=\"#\">".
      "Enter The Project Folder Name (What you changed myGame to):<br />".
      "<input type=\"text\" name=\"projName\" size=\"25\" value=\"\"><br /><br />".
      "<input type=\"hidden\" name=\"step\" value=\"4\"/>".
      "<input type=\"hidden\" name=\"db\" value=\"".$db."\"/>".
      "<input type=\"hidden\" name=\"us\" value=\"".$us."\"/>".
      "<input type=\"hidden\" name=\"pw\" value=\"".$pw."\"/>".
      "<input type=\"submit\" name=\"test\" value=\"Create File\">";
   }
   
   function finishUp($db, $us, $pw, $proj) {
      echo "<b>MAP Master Server System: SQL Install<br/>STEP 5 / 5</b><p>";
      echo "Finishing Up...<p>";

      $MasterVariables["db_username"] = $us;
      $MasterVariables["db_password"] = $pw;
      $MasterVariables["database"] = $db;
      $MasterVariables["table"] = "Servers";

      $my_file = $proj . "/settings.php";
      $data = '$MasterVariables = ' . var_export($MasterVariables, true) . ';';
      file_put_contents($my_file, '<?PHP ' . $data . ' ?>');
      
      echo "<p>All Done!<p>".
      "Thank you for using the install.php script. You may now continue the installation ".
      "through the documentation... <b>Please Delete This File When Done!!!</b>";
   }
?>
