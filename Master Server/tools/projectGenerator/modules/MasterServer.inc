<?php
//Phantom139: Multiplayer Assembly Package
beginModule( 'MasterServer' );	
   addEngineSrcDir('PGD');
   addEngineSrcDir('PGD/Control');
   addEngineSrcDir('PGD/Solutions');

   addIncludePath( getAppLibSrcDir() . 'curl' );
   addProjectLibDir( getAppLibSrcDir() . 'curl' );

   //Define:
   addProjectDefines( 'CURL_STATICLIB' );
   //Phantom139: End
endModule();
?>
