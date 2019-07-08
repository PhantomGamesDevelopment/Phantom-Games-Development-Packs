<?php
//Phantom139: Multiplayer Assembly Package
beginModule( 'MAP' );	
   addEngineSrcDir('PGD');
   addEngineSrcDir('PGD/Control');
   addEngineSrcDir('PGD/Crypto');
   addEngineSrcDir('PGD/Solutions');

   addIncludePath( getAppLibSrcDir() . 'curl' );
   addProjectLibDir( getAppLibSrcDir() . 'curl' );

   addIncludePath( getAppLibSrcDir() . 'cryptoPP/include' );
   addProjectLibDir( getAppLibSrcDir() . 'cryptoPP' );
   
   addProjectIgnoreDefaultLib( 'LIBCMT' );
   
   //Define:
   addProjectDefines( 'CURL_STATICLIB' );
   //Phantom139: End
endModule();
?>
