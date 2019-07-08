//news.cs
//Phantom139

//Download and populate the newsDlg. Also controls the commands
//for this specific GUI control.

//STYLE:
//$PGD$NEWS Title\tContent

function addNewsData(%data) {
   %this = newsControl;
   // Create the article
   %article = new ScriptObject() {
      headline = getField(%data, 0);
      content = strReplace(getField(%data, 1), "[n]", "\n");
   };
   // Add the article
   %this.article[%this.last] = %article;
   %this.last++;
}

function newsDlg::onWake(%this) {
   if(!isObject(newsControl)) {
      MessagePopup("News", "Please Wait, Connecting to PGD News Server");
      schedule(500, 0, downloadNews);
      return;
   }
   //check the news control
   %this.populate();
   //select the last article
   Headlines.setSelectedRow(0);
   newsContent.setText(newsControl.article[0].content);
}

function newsDlg::populate(%this) {
   if(!isObject(newsControl)) {
      return;
   }
   for(%i = 0; newsControl.article[%i] !$= ""; %i++) {
      %headline = newsControl.article[%i].headline;
      //%content = newsControl.article[%i].content;
      //Add the headline to the text.
      Headlines.addRow(%i, %headline);
   }
}

function Headlines::onSelect(%id, %text) {
   //echo(%id @" "@%text);
   newsContent.setText(newsControl.article[%text].content);
}

function downloadNews() {
   if(!isObject(newsControl)) {
      new ScriptObject(newsControl) {
         last = 0;
      };
   }
	else {
	   newsControl.delete();
      new ScriptObject(newsControl) {
         last = 0;
      };
	}
	Headlines.clear();
   if(isObject(NewsDownloader)) {
      NewsDownloader.delete();
   }
   %theNews = new TCPObject(NewsDownloader);
   
   %separator = getRandomSeperator(16);
   %header = assembleHTTP1_1Header("www.phantomdev.net", "/survival/news/index.php", "POST", "PGD Survival Client", "Content-Type: multipart/form-data; boundary="@%separator@"\r\n");
   %dispo = makeDisposition(%separator, newsReq, 1, 1);
   %header = %header @ "Content-Length: " @ strLen(%dispo) @ "\r\n\r\n";
   
   %theNews.request = %header @ %dispo;
   %theNews.connect("www.phantomdev.net:80");
}

function NewsDownloader::onConnected(%this) {
   %this.timeout = schedule(7500, 0, TimeoutNewsDownload);

   %this.send(%this.request);
}

function NewsDownloader::onLine(%this, %line) {
   closeMessagePopup();
   if(strstr(%line, "$PGD") != -1) {
      %line = strReplace(%line, "$PGD", "");
      %response = stripchars(%line, "$\n");
      switch$(firstWord(%response)) {
         case "INTERNAL":
            MessageBoxOK("News", "Could Not Download The News: \n"@%response);
            return;
         case "NEWS":
            //add the news to the news control
            %response = strReplace(%response, "NEWS ", "");
            addNewsData(%response);
      }
   }
}

function NewsDownloader::onDisconnect(%this) {
	cancel(%this.timeout);
   %this.delete();

   for(%i = 0; newsControl.article[%i] !$= ""; %i++) {
      %headline = newsControl.article[%i].headline;
      //%content = newsControl.article[%i].content;
      //Add the headline to the text.
      Headlines.addRow(%i, %headline);
   }
	//select the last article
   Headlines.setSelectedRow(0);
   newsContent.setText(newsControl.article[0].content);
}

function NewsDowloader::onConnectFailed(%this) {
   TimeoutNewsDownload();
}
function NewsDowloader::onDNSFailed(%this) {
   TimeoutNewsDownload();
}

function TimeoutNewsDownload() {
	if(isObject(NewsDownloader)) {
	   NewsDownloader.disconnect();
	}

	closeMessagePopup();
	MessageBoxOk("Connection Timed Out", "Unable to connect to the news server.");
}