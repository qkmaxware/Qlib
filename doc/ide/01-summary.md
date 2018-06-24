#Qlib - IDE:
##Development environment for use with the Qlib - Runtime and the QAS langauge

##Installation
The /src/ide directory contains a Netbeans project folder. Either download the precompiled .jar from the Releases or download Netbeans, open the project and compile your own executable.

##How to Launch
Launch the .jar by double clicking the jar file or by entering the following command into your terminal/cmd/powershell.
```
> java -jar QLib_IDE.jar
```

##Parts of the Ui
1) There is a header along the top with all of the functionality of the IDE
2) The center panel contains all open scripts separated by tabs
3) Along the bottom is a log where script output will be printed.

##Basic Use
####Point to the Qlib - Runtime
The first thing you will want to do is configure the IDE to point to your compiled version of the Qlib - Runtime. You can do this by clicking the "Wrench" icon on the top right side of the window. This opens the program configuration settings. Simply double click the value next to the "binary path" key and change it to the path to your executable. By default this is set to "./qlib".
####Create new file
The top left icon which looks like blank page will open a new empty file into the editor. This file will need to be saved in order to name the file.
####Load existing file
The button next to the new file button will open a file browser that can load an exisiting text file into the editor.
####Save file
Any file that has been edited will appear with a "*" behind it in the tab's header. The button next to the load file button will save the file
####Close file
The button next to save file will close the current file in the editor.
####Execute Script
In the middle of the header is a green right-arrow icon. Clicking this icon will invoke the Qlib - Runtime with the currently open file passed as the input parameter.
####Clear log
The button next to the execute script button will clear the log.
####Open help
The first button on the far right will open the internet browser to the online documentation for Qlib.
####Settings button
The final button on the far right of the header will open a new window where the settings for the IDE can be modified.

##Shortcuts
| Action | Key |
| ------ | --- |
| New File | Ctrl + N |
| Open File | Ctrl + O |
| Save File | Ctrl + S |
| Close File | Ctrl + W |
| Run Script | Ctrl + Enter |