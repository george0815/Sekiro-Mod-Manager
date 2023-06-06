# Sekiro-Mod-Manager
![540](https://github.com/george0815/Sekiro-Mod-Manager/assets/20736715/e7f2e6d2-47c6-4ce0-86dc-c6df35f4d2c7)


Mod Manger for the 2019 Game of The Year; Sekiro - Shadows Die Twice

Currently you can add, remove, install, and uninstall mods, modpacks and profiles, as well as edit most of modengine's settings. 


# Usage


In addition to modengine, you need the Microsoft Visual Studio C++ Redistributables found here https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads . I'm pretty sure you only need the 2015 version, but if it doesn't work then install the 2017 and 2019 versions.
NOTE: THIS MANAGER ONLY WORKS ON MODS USED WITH MODENGINE, ZIPPED UP IN EITHER A .ZIP OR .RAR FILE<br>

Install - Installs mod to Sekiro directory <br>
Uninstall - Uninstalls mod from Sekiro directory<br>
/+ - Adds mod to manager<br>
/- - Removes mod from manager (will not automatically uninstall mod)<br>

Finally, if you add or remove a mod without using the manager, you will have to also do it with the manager if you want to prevent a mismatch between what mods are actually installed and what mods the manager thinks are installed.  

## Mods

To install a mod, first add it to the manager by pressing the "plus" button, then press the install button. This will unpack the mod files into the mods folder in the Sekiro directory(provided modengine is installed). 


## Profiles

Basically, this feature makes use of modengine’s “modOverrideDirectory” option to allow the quick enabling and disabling of multiple mods at once; without having to constantly uninstall and reinstall multiple mods. 

Let’s say you have multiple mods that make the game easy, and multiple mods that make the game more difficult. You can create a profile that contains all of the “easy” mods, and another profile that contains all of the “hard” mods. Once the profiles are created, you can choose which set of mods you want to play with with a click of a button. 

In short; modengine by default uses whatever files are in the “mods” folder, the manager’s profile feature just allows you to organize your mods into separate folders, then quickly switch whatever folder modengine will use to load mods. 

To install a profile, just click the "plus" button, specify the name of the profile and how many mods will be included, then choose what mods to add. 
The currently active profile will be displayed on the bottom right of the window, you can switch the currently active profile by clicking “Set Active”


## Modpacks

Sometimes mod authors will bundle multiple mods into one zip file, like this Costume Pack mod found here. In this case, it would be best to use the modpack feature. The modpack feature basically just installs whatever individual mods you want without making you extract the zip file and rezip each mod individually. 

To install a modpack, click the “Modpack” checkbox when you are asked the name of the mod. Now, instead of choosing the name of an individual mod, you are choosing the name of the modpack you are installing. Upon confirming, you will be asked to choose the folder that contains the files for the individual mod you want to install, then the manager will ask you for the name of that individual mod. This cycle will continue until the user presses “Cancel” when they are in the file dialog. 
When it’s all done, the names of the mods installed from the mod pack will have the name of the modpack appended to it


## Miscellaneous

Most of the other settings (seen at the bottom right of the window) are merely just modengine settings that are changeable via the manager. So if you have any questions regarding those I would consult modengine's documentation.


# Installation

Just extract the zip file and launch the program, you will be prompted to choose your Sekiro directory. 
NOTE: YOU MUST HAVE SEKIRO INSTALLED, if you don't, the file dialog will just keep asking you where it is, and the only way you'll be able to break out of it is by tricking it by naming a file "sekiro.exe" or ending the task via task manager. Yes, I know this is stupid, this was a huge oversight on my part and I apologize. 



## **IMPORTANT**

### EDGE CASE MODS

When handling mod archive files with no folders used by modenigne(parts, mtd, param...), the program sorts the files based on their name, however some files have the same name (font.gfx for example). When installing a mod that contains these files the program will display this prompt


![TBV9Dzb](https://github.com/george0815/Sekiro-Mod-Manager/assets/20736715/2951d36d-6f4a-4c43-afee-3cb06bd2d765)


if you see this, just go to the mod page of the mod and find out which folder the file goes in. Then choose the folder out of the drop down menu.


### OVERWRITING
if you install two or more mods that edit the same files, they will overwrite each other, same goes if you add multiple mods that edit the same file into a profile


### UI SIZE
If the checkboxes and other UI elements are too large, please go into your windows display settings and change the size of text, apps, and other items to 100%
