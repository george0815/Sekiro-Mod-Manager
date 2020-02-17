#include "addprofile.h"
#include "sekiro.h"
#include "ui_addprofile.h"
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <qdebug.h>


extern struct profile Profile;

string modProfilePath;

extern bool isProfileDone;

addProfile::addProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addProfile)
{
    ui->setupUi(this);

    //makes window frameless
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
}

addProfile::~addProfile()
{
    delete ui;
}


//adds profile to manager

void addProfile::on_profileConfirm_clicked()
{

//adds the data for the profile being added to the profile struct

Profile.name = ui->profileNameLineEdit->text().toLocal8Bit().constData();

Profile.modNum = ui->profileModNumberSpinBox->value();

Profile.profileConfigPath = ".\\configsP\\" + Profile.name + ".ini";

Profile.path = ".\\profiles\\" + Profile.name;

Profile.profileFolder = sekDir + "/" + Profile.name;

qDebug() << QString::fromStdString(Profile.profileFolder);







//creates modengine.ini for profile

Profile.modengineConfig = "; Mod Engine configuration file\n"
        "; Mod Engine (C) 2019 Katalash. All Rights Reserved.\n"
        "; Mod Engine is a configurable injection DLL used to modify some aspects of the"
        "; game to make it more friendly for modding. See the following properties that can\n"
        "; be configured. \n"

        "[misc]\n"
        "; Skips displaying the logos on boot\n"
        "skipLogos=1\n"
        "; Chain loads another dll that hooks dinput8.dll\n"
        "; For example, if you have another dll mod that\'s named dinput8.dll, you can rename it to\n"
        "; othermod.dll or something, place it in the Sekiro directory, and set this path to\n"
        "; chainDInput8DLLPath=\"\\othermod.dll\" or whatever you named the dll\n"
        "chainDInput8DLLPath=\"\"\n"

        "[files]\n"
        "; Loads extracted files from UXM instead of data from the archives. Requires a complete UXM extraction\n"
        "; and should generally only be used by mod creators.\n"
        "loadUXMFiles=0\n"
        "; If enabled, a mod will be loaded from a specified override directory.\n"
        "useModOverrideDirectory=1\n"
        "; The directory from which to load a mod.\n"
        "modOverrideDirectory=\"\\"+ Profile.name +"\"\n"
        "; Caches results of looking up override files. Can speed up loading and reduce hitching, but may require game\n"
        "; restart when a file is added or removed from a mod. Mod developers probably want this disabled, while mod\n"
        "; users (and released mods) should have this enabled.\n"
        "cacheFilePaths=1\n"

        "[debug]\n"
        "; Shows the debug console when running the game. Can be useful for modders to troubleshoot\n"
        "showDebugLog=0\n";



ofstream modengoneconfig(".\\profiles\\"+ Profile.name + ".ini");

modengoneconfig << Profile.modengineConfig;

modengoneconfig.close();




//restructures mod archives

for(int i = 0; i < Profile.modNum; i++){

    //open file dialog

    //sets available filters for file extensions when choosing file
    QString filter = "All archives (*.rar *.zip *.7z) ;; RAR (*.rar)  ;;  Zip (*.zip) ;; 7z (*.7z) ";

    //Allows user to choose sekiro directory
    QString modNAME = QFileDialog::getOpenFileName(this, "Open Mod","C://",filter);





    //if modname is legit, then adds mod to profile
    if(!modNAME.isEmpty()&& !modNAME.isNull()){

    QFileInfo ext = modNAME;
    QString modExt = ext.suffix();




   //restructures archive, the reason for doing this is while almost every mod uses the unpacked sekiro folders like "parts" "chr" etc
   //some mod compressed archives might have their files in an extra folder, like one mod archive might have a directory like bossrushV.012/bossrush/mod folders
   //and another might have sekiro/mod folder so by restructuring, it makes sure that the mod folders ("parts" "chr" "mtd" etc) are in the root folder of the mod archive
   //for later exxtracting

   QFile file(modNAME);

   string modDir = ".\\profiles\\" + to_string(i) + "." + modExt.toLocal8Bit().constData();

   file.rename(modDir.c_str());

    QDir().mkdir(".\\tmp");

    if(modExt == "7z" || modExt == "zip"){

        unpackRepackProfiles("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\profiles\\"  + to_string(i) + "." + modExt.toLocal8Bit().constData() + "\"");


    }
    else if(modExt == "rar"){

        unpackRepackProfiles("cd %cd%    &    unrar x  -y \"%cd%\\profiles\\"  + to_string(i) + ".rar\" * .\\tmp\\");

    }

    traverseProfiles("*.*", ".\\tmp\\", 0);

    unpackRepackProfiles("cd %cd%   &   7za a -y \".\\tmp\\"  + to_string(i) + ".zip\" \"" + modProfilePath + "/*\"");

    file.remove();



    //moves restructured mod files to profiles folder

    string modFinalTemp = ".\\tmp\\" + to_string(i) + ".zip";

    QFile modFinal(modFinalTemp.c_str());

    string modDirTemp = ".\\profiles\\" + to_string(i) + ".zip";

    modFinal.rename(modDirTemp.c_str());




    //deletes tmp

    QDir dir(".\\tmp\\");
    dir.removeRecursively();

    QDir().mkdir(".\\tmp\\");

    }

}



//dump mod files in tmp

for(int i = 0; i < Profile.modNum; i++){

    unpackRepackProfiles("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\profiles\\"  + to_string(i) + ".zip""\"");

}






//get all files in tmp and put them in Profiles.files

traverseProfiles("*.*", ".\\tmp\\", 1);




//creates profile archive

unpackRepackProfiles("cd %cd%   &   7za a -y \".\\tmp\\"  + Profile.name + ".zip\" .\\tmp\\*");







//deletes profile mods in profiles folder

for(int i = 0; i < Profile.modNum; i++){

QFile file(QString::fromStdString(".\\profiles\\" + to_string(i) + ".zip"));


file.remove();

}





//moves profile archive to profiles folder

string modFinalTemp = ".\\tmp\\" + Profile.name + ".zip";

QFile modFinal(modFinalTemp.c_str());

string modDirTemp = ".\\profiles\\" + Profile.name + ".zip";

modFinal.rename(modDirTemp.c_str());





//deletes tmp folder

QDir dir(".\\tmp\\");
dir.removeRecursively();

close();




//creates config for profile and adds profile to profiles vector

Profile.profileConfigPath = ".\\configsP\\" + Profile.name + ".ini";
ofstream profileConfig(Profile.profileConfigPath);

profileConfig << Profile.name + "\n" + Profile.path + "\n" + Profile.profileConfigPath + "\n" + Profile.profileFolder + "\n" + to_string(Profile.modNum);

profileConfig.close();

profiles.push_back(Profile);

isProfileDone = true;

}






//unpacks and repacks compressed archives

void addProfile::unpackRepackProfiles(string line){


    //creates batch file, then puts string line into batch file, then launches it  using system, even though it
    //is called unpackRepack, it can be used whenever you need to execute a command prompt command

    ofstream unpack;
    unpack.open( "commands.cmd", ios::trunc );

    unpack <<
        line << endl;

    unpack.close();



    system( "cmd.exe /c commands.cmd" );


    remove( "commands.cmd" );

}



//looks in directory for profile files

void addProfile::traverseProfiles(const QString &pattern, const QString &dirname, int mode)
{
    QDir dir(dirname);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);

    int i = 0;

    //recursivly looks in the directory that was given
    foreach (QFileInfo fileInfo, dir.entryInfoList()) {



        //if the fileinfo is a readable directory then it looks for a folder that matches the folders used by sekiro with modengine

        if (fileInfo.isDir() && fileInfo.isReadable()){


            //if it finds a folder used by sekiro with modengine then it puts it into the string trueModPath, which holds the
            //folder in the mod archive that has the folders used by mod enigne

            if((fileInfo.baseName() == "parts" || fileInfo.baseName() == "event" ||fileInfo.baseName() == "map" || fileInfo.baseName() == "msg" ||
                    fileInfo.baseName() == "param" || fileInfo.baseName() == "script" || fileInfo.baseName() == "chr"  || fileInfo.baseName() == "cutscene"
                || fileInfo.baseName() == "event" || fileInfo.baseName() == "facegen" || fileInfo.baseName() == "font" || fileInfo.baseName() == "action"
                    || fileInfo.baseName() == "menu" || fileInfo.baseName() == "mtd" || fileInfo.baseName() == "obj" || fileInfo.baseName() == "other"
                    || fileInfo.baseName() == "sfx" || fileInfo.baseName() == "shader" ) && mode == 0){


                modProfilePath = fileInfo.path().toLocal8Bit().constData();
                qDebug() << modProfilePath.c_str();


            }


            //calls function so it searches all the sub directories
            traverseProfiles(pattern, fileInfo.filePath(), mode);



        }



        //if mode = 1 and the fileinfo is a file then puts all of the files in the profiles.files vector, so that when the user wants to uninstall the mod, the program has all of
        //the names of the files to delete

        if(mode == 1){




        QString file = fileInfo.filePath().remove(0, 5);



        if(fileInfo.isFile()){

        Profile.files.push_back(file.toLocal8Bit().constData());

        qDebug() << file;

        i++;

        }

      }



    }
}




//cancels adding a profile

void addProfile::on_cancel_clicked()
{

    isProfileDone = false;
    close();

}
