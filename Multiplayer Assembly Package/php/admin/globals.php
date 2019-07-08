<?php

   //globals.php
   //Robert Fritzen
   //xxz 568 v1.4
   
   $AuthOnline = 1; //Is our server online for use?
   
   /**
   The first half of this file is essential to you, please adjust all of the fields
   below to ensure that this package fully functions as it was meant to, forgetting
   a piece of this will cause it to malfunction during account based operations that involve
   the server
   
   The second half of this however, can be left unadjusted, it is mainly for the RSA Key
   generation for the Certificate Authority (CA) part of this resource. If you would like
   a stronger (or weaker) key, you can adjust that below.
   
   The below is an explanation of each, the actual setup is at the bottom of the page
   **/
   
   //================================================================================
   /**
   Set this to your website's MAIN address, do not include any subsets.
   unless if you are a part of a subdomian, then do so. This is used for the
   re-direct links for invalidations, as well as a few other things
   **/
   $siteAddr = "www.mywebsite.com";
   
   /**
   Set this to the database that the tables used by XXZ will be stored on, for
   example, mine reads: phantom7_TUprising, the usual format of these are
   webadminuser_databasename, the second one varies by the server, most of the
   time you can leave it be, but if you need to access off site, then change it
   as necessary
   **/
   //$XXZVariables["database"]["newProject"] = "your_database";
   //$XXZVariables["dbAccess"]["newProject"] = "localhost";
   
   /**
   Set these to the MySQL account with access privlages to the tables/database as
   we will be needing to read, write, and delete to/from the tables
   **/
   //$XXZVariables["db_username"]["newProject"] = "your_database_username";
   //$XXZVariables["db_password"]["newProject"] = "your_db_user_password";

   /**
   This is the table that stores the account information to the server
   **/
   //$XXZVariables["account_table"]["newProject"] = "Accounts";
   /**
   This is the table that stores the account keys to the server
   **/
   //$XXZVariables["key_table"]["newProject"] = "AccountKeys";
   
   /**
   Set this to 1 to bind keys by the email address. This will lock out anyone else from creating
   an account with that account key if they use a different email. I prefer this, but if you think
   your data is secured the way it is, then you do not need to go this route.
   **/
   //$XXZVariables["AK_BindEmail"]["newProject"] = 1;
   
   //================================================================================
   /**
   This is the strength of the RSA CA key that is generated
   **/
   $CAP["RSA_Strength"] = 1024;

   /**
   This is the length of the account key, 25 is the default value, usually anything over
   15 is fine.
   **/
   $CAP["AK_Length"] = 25;
   
   /**
   This is the hash algorithm used when generating account keys on the server.
   Ensure this matches the TS Variable $Auth::HashMode in scripts/client/auth/globals.cs

   1 - SHA-1
   2 - Whirlpool
   **/
   $CAP["AK_HashingAlgo"] = 3;

   //================================================================================
   //=                      PROJECT SETTINGS                                        =
   //================================================================================
   //*** TEMPLATE
   $XXZVariables["database"]["newProject"] = "your_database";
   $XXZVariables["dbAccess"]["newProject"] = "localhost";
   $XXZVariables["db_username"]["newProject"] = "your_database_username";
   $XXZVariables["db_password"]["newProject"] = "your_db_user_password";
   $XXZVariables["account_table"]["newProject"] = "Accounts";
   $XXZVariables["key_table"]["newProject"] = "AccountKeys";
   $XXZVariables["AK_BindEmail"]["newProject"] = 1;
   //*** END TEMPLATE
   
   //*** Your Project
   
   //*** End Your Project

   
   //================================================================================
   //================================================================================
   
   global $XXZVariables;
   global $siteAddr;
   global $CAP;
   global $AuthOnline;
?>
