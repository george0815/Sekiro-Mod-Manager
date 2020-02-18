#include "sekiro.h"
#include "ui_sekiro.h"
#include <iostream>
#include <addprofile.h>
#include <string>
#include <QDebug>
#include <QFileDialog>
#include <fstream>
#include <ostream>
#include <Windows.h>
#include <QDirIterator>
#include <vector>
#include <cstdlib>
#include "direct.h"
#include <QDir>
#include <modname.h>
#include <QFileInfoList>
#include <QMessageBox>
#include <QUrl>
#include <QDesktopServices>

using namespace std;


bool  warning = false;

string sekDir;

string modName;

struct mod Mod;


struct profile Profile;

vector<profile> profiles;

vector<mod> mods;

string trueModPath;

bool isProfileDone = false;



Sekiro::Sekiro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sekiro)
{
    ui->setupUi(this);




    //sets the size

    setFixedSize(423, 401);




    //see function definition
    checkDir();



    //see function definition
    getSettings();


    //see function definition
    getSettingsProfile();




    //creates a default modengine.ini, this is used for when the user clicks "default" and wants to set the folder modengine uses to "\mods"

    string modengineConfig = "; Mod Engine configuration file\n"
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
            "modOverrideDirectory=\"\\mods\"\n"
            "; Caches results of looking up override files. Can speed up loading and reduce hitching, but may require game\n"
            "; restart when a file is added or removed from a mod. Mod developers probably want this disabled, while mod\n"
            "; users (and released mods) should have this enabled.\n"
            "cacheFilePaths=1\n"

            "[debug]\n"
            "; Shows the debug console when running the game. Can be useful for modders to troubleshoot\n"
            "showDebugLog=0\n";



    ofstream modengoneconfig(".\\modengine.ini");

    modengoneconfig << modengineConfig;

    modengoneconfig.close();



    //see function definition
    modEngineCheck();


}


Sekiro::~Sekiro()
{
    delete ui;
}










//adds mod to the mod manager

void Sekiro::on_addMod_clicked()
{

    //open file dialog

    //sets available filters for file extensions when choosing file
    QString filter = "All archives (*.rar *.zip *.7z) ;; RAR (*.rar)  ;;  Zip (*.zip) ;; 7z (*.7z) ";

    //Allows user to choose sekiro directory
    QString modNAME = QFileDialog::getOpenFileName(this, "Open Mod","C://",filter);




    //if mod name is legit, then goes through the process to add a mod
    if(!modNAME.isEmpty()&& !modNAME.isNull()){




    //gets mod filename extension, this is used when deciding whether to use unrar or 7zip extra
    QFileInfo ext = modNAME;
    QString modExt = ext.suffix();







    //pop up window asking for name of mod

    modname Modname;
    Modname.setModal(true);
    Modname.exec();

    Modname.setAttribute(Qt::WA_DeleteOnClose);







    //moves mod file to mods folder in program directory

    QFile file(modNAME);

    string modDir = ".\\mods\\" + modName + "." + modExt.toLocal8Bit().constData();

    file.rename(modDir.c_str());






    //sets mods[n].name, mods[n].path, and mods[n].modNum

    Mod.name = modName;

    Mod.path = ".\\mods\\" + modName;






    //creates tmp folder and extracts archive to temp

    QDir().mkdir(".\\tmp");

    //if mod is contained in a zip file
    if(modExt == "7z" || modExt == "zip"){


        unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\mods\\"  + modName + "." + modExt.toLocal8Bit().constData() + "\"");


    }


    //if mod is contained in a rar file
    else if(modExt == "rar"){


        unpackRepack("cd %cd%    &    unrar x  -y \"%cd%\\mods\\"  + modName + ".rar\" * .\\tmp\\");

    }










    //restructures archive, the reason for doing this is while almost every mod uses the unpacked sekiro folders like "parts" "chr" etc
    //some mod compressed archives might have their files in an extra folder, like one mod archive might have a directory like bossrushV.012/bossrush/mod folders
    //and another might have sekiro/mod folder so by restructuring, it makes sure that the mod folders ("parts" "chr" "mtd" etc) are in the root folder of the mod archive
    //for later exxtracting


    traverse("*.*", ".\\tmp\\", 0);

    unpackRepack("cd %cd%   &   7za a -y \".\\tmp\\"  + modName + ".zip\" \"" + trueModPath + "/*\"");


    file.remove();


    string modFinalTemp = ".\\tmp\\" + modName + ".zip";

    QFile modFinal(modFinalTemp.c_str());

    string modDirTemp = ".\\mods\\" + modName + ".zip";

    modFinal.rename(modDirTemp.c_str());


    QDir dir(".\\tmp\\");
    dir.removeRecursively();








    //gets names of all files and puts all file names in mods[n].files[i]

    QDir().mkdir(".\\tmp");

    unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\mods\\"+ modName + ".zip\"");

    traverse("*.*", ".\\tmp\\", 1);



    //see function definition
    debugFileList(1);





    //saves mod entry to file

    QDir toCount(".\\configs\\");

    toCount.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    int total_files = toCount.count();

    qDebug() << total_files;

    Mod.modConfigPath = ".\\configs\\" + Mod.name + ".ini";
    ofstream modConfig(Mod.modConfigPath);

    modConfig << Mod.name + "\n" + Mod.path + "\n" + Mod.modConfigPath;





    //psuhes mod to vector of mods

    mods.push_back(Mod);





    //deletes tmp folder

    dir.removeRecursively();






    //creates new mod entry in combo box

    ui->modsInstalled->addItem(QString::fromStdString(modName));

}

}







//on program startup, checks the dir.ini file for the sekiro directory, if there isnt an entry, then its prompts the user for it

void Sekiro::checkDir(){


    //makes folder "mods" in root program root directory
    QDir().mkdir("mods");

    //makes folder "configs" in root program root directory
    QDir().mkdir("configs");


    //makes folder "profiles" in root program root directory
    QDir().mkdir("configsP");

    //makes folder "profiles" in root program root directory
    QDir().mkdir("profiles");




    //creates fileinfo for dir.ini, the file that stores the sekiro directory, this is used to check if dir.ini exists
    QFileInfo sekDirIni(".\\dir.ini");



    //checks if ".\dir.ini" exists, if it doesnt, then makes dir.ini
    if(!(sekDirIni.exists())){


        QFileDialog dialog;

        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);

        QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");



        //checks if directory is legit, if it is then sets it equal to sekDir, then makes dir.ini
        if(!path.isEmpty()&& !path.isNull()){

        sekDir = path.toLocal8Bit().constData();

        ui->currentSekDir->setText(path);

        ofstream dir(".\\dir.ini");
        dir << sekDir;
        dir.close();

        }



    }



    else{

        ifstream in(".\\dir.ini");


        getline(in, sekDir);



        ui->currentSekDir->setText(QString::fromStdString(sekDir));

        //if directory is empty, then prompts the user again for sekiro directory
        if(sekDir == ""){

            QFileDialog dialog;

            dialog.setFileMode(QFileDialog::Directory);
            dialog.setOption(QFileDialog::ShowDirsOnly);

            QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");

            if(!path.isEmpty()&& !path.isNull()){

            ui->currentSekDir->setText(path);

            sekDir = path.toLocal8Bit().constData();

            ofstream dir(".\\dir.ini");
            dir << sekDir;
            dir.close();

            }


        }



    }



}






//looks in directory for mod files

void Sekiro::traverse(const QString &pattern, const QString &dirname, int mode)
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


                trueModPath = fileInfo.path().toLocal8Bit().constData();
                qDebug() << trueModPath.c_str();


            }


            //calls function so it searches all the sub directories
            traverse(pattern, fileInfo.filePath(), mode);



        }


        //if mode = 1 and the fileinfo is a file then puts all of the files in the mods.files vector, so that when the user wants to uninstall the mod, the program has all of
        //the names of the files to delete
        if(mode == 1){


        QString file = fileInfo.filePath().remove(0, 5);

        //qDebug() << file;


        if(fileInfo.isFile()){

        Mod.files.push_back(file.toLocal8Bit().constData());

        i++;

        }

      }



    }
}





//unpacks and repacks compressed archives

void Sekiro::unpackRepack(string line){




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







//prints files in files vector for profiles and mods

void Sekiro::debugFileList(int mode){

    //if mode = 1, then it prints out the files for the mod currently being installed
    //if mode = 2, then it prints out the files for the profile currently being installed


    if(mode == 1){

    for(int i = 0; i < Mod.files.size(); i++){


        cout << Mod.files[i] << endl;



    }


    }

    else if(mode == 2){

        for(int i = 0; i < Profile.files.size(); i++){


            cout << Profile.files[i] << endl;



        }


    }



}











//installs mod to sekiro directory

void Sekiro::on_Install_clicked()
{



    //checks if there are no mods installed, if there isnt, then throws an error
    if(ui->modsInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }


    else{




        if(warning == true){

            unpackRepack("cd %cd%   &   7za e -spf -y -o\"" + sekDir + "\\mods\\\" \"%cd%\\mods\\"+ mods[ui->modsInstalled->currentIndex()].name + ".zip\" ");


        }
        else if (warning == false){

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Warning", "Installing a mod might overwrite a previously installed mod  \nDo you still want to install this mod?", QMessageBox::Yes|QMessageBox::No);



        //if the user clicks yes then installs mod and if there is a mod at the current index, then unpacks the mod files into the sekiro directory
        if (reply == QMessageBox::Yes) {

            unpackRepack("cd %cd%   &   7za e -spf -y -o\"" + sekDir + "\\mods\\\" \"%cd%\\mods\\"+ mods[ui->modsInstalled->currentIndex()].name + ".zip\" ");


        }


        }

   }


}










//uninstalls mod from sekiro directory

void Sekiro::on_Uninstall_clicked()
{
    //checks if there are no mods installed, if there isnt, then throws an error
    if(ui->modsInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }

    else{

    for(int i = 0; i < mods[ui->modsInstalled->currentIndex()].files.size(); i++){


        //goes thorugh the files vector and gets the name for the file to be deleted, then deletes it

        string del = sekDir + "/mods" + mods[ui->modsInstalled->currentIndex()].files[i];

        qDebug() << QString::fromStdString(del);

        QFile toBeDeleted(QString::fromStdString(del));

        toBeDeleted.remove();





    }



}



}











//loads the installed mods

void Sekiro::getSettings(){



    //gets all of the ini files for mods

    QDir directory(".\\configs\\");
    QStringList configs = directory.entryList(QStringList() << "*.ini" ,QDir::Files);

    int i = 0;



    //for each ini files, installs the mod for that ini file

    foreach(QString filename, configs) {




        //gets mod name, path, and the path for the config

        ifstream config(".\\configs\\" + filename.toStdString());

        getline(config, Mod.name);
        getline(config, Mod.path);
        getline(config, Mod.modConfigPath);

        qDebug() << QString::fromStdString(Mod.name);
        qDebug() << QString::fromStdString(Mod.path);
        qDebug() << QString::fromStdString(Mod.modConfigPath);





        //gets files for mod

        QDir().mkdir(".\\tmp");

        unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\mods\\"+ Mod.name + ".zip\"");

        traverse("*.*", ".\\tmp\\", 1);

        debugFileList(1);




        //creates mod entry in vector of mods

        mods.push_back(Mod);





        //deletes tmp folder

        QDir dir(".\\tmp\\");
        dir.removeRecursively();






        //creates new mod entry in combo box

        ui->modsInstalled->addItem(QString::fromStdString(mods[i].name));

        i++;

    }

}









//removes mod from manager

void Sekiro::on_removeMod_clicked()
{


    //checks if no mods are installed, if there are no mods installed, then it throws an error
    if(ui->modsInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }


    //if there is a mod at the current index then it uninstalls it
    else{




    //uninstalls mod config file

    QFile configDel(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));

    configDel.remove();




    //uninstalls mod archive

    QFile modDel(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].path + ".zip"));

    modDel.remove();




    //deletes mod entry in vector

    mods.erase(mods.begin() + ui->modsInstalled->currentIndex());



    //deletes mod entry in combp box

    ui->modsInstalled->removeItem(ui->modsInstalled->currentIndex());

    }
}










//changes sekiro directory

void Sekiro::on_changeSekDir_clicked()
{



    //askes the user if they want to install mod
    QMessageBox::StandardButton reply;

    if(warning == false){

    reply = QMessageBox::question(this, "Warning", "Changing the Sekiro directory will require you to reinstall your mods  \nDo you still want to change directory?", QMessageBox::Yes|QMessageBox::No);



    //if the user clicks yes then downloads and installs modengine
    if (reply == QMessageBox::Yes) {







    //prompts the user for sekiro directory

    QFileDialog dialog;

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");



    //checks if the directory is legit, if it is then puts it into sekDir
    if(!path.isEmpty()&& !path.isNull()){

    sekDir = path.toLocal8Bit().constData();

    ui->currentSekDir->setText(path);

    }


}
    }


    else if(warning == true){

        //prompts the user for sekiro directory

        QFileDialog dialog;

        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);

        QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");



        //checks if the directory is legit, if it is then puts it into sekDir
        if(!path.isEmpty()&& !path.isNull()){

        sekDir = path.toLocal8Bit().constData();

        ui->currentSekDir->setText(path);

        }

    }
}










//adds profile(basically a merged mod that you can enable/disable without completely uninstalling)

void Sekiro::on_addProfile_clicked()
{



    //launches the add profile prompt

    addProfile profileWindow;
    profileWindow.setModal(true);
    profileWindow.exec();

    profileWindow.setAttribute(Qt::WA_DeleteOnClose);


    //when the profile is added, it then updates the combo box


    if(isProfileDone == true){
   ui->profilesInstalled->addItem(QString::fromStdString(profiles[ui->profilesInstalled->count()].name));
}

}









//installs profile to the sekiro directory

void Sekiro::on_installProfile_clicked()
{

    //checks if no profiles are installed, if there are no profiles installed, then it throws an error

    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Profile Selected");
    }

    //if there is a profile installed at the current index then unpacks the profile files into the sekiro directory
    else{






    //unpacks the profile files into the sekiro directory

    string profileInstallFolder = sekDir + "";

    QDir().mkdir(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileFolder));

    unpackRepack("cd %cd%   &   7za e -spf -y -o\"" + sekDir + "\\"+profiles[ui->profilesInstalled->currentIndex()].name +"\\\" \"%cd%\\profiles\\"+ profiles[ui->profilesInstalled->currentIndex()].name + ".zip\" ");




    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    //moves profiles modengine.ini to sekiro directory

    string profileModengine  = ".\\profiles\\" + profiles[ui->profilesInstalled->currentIndex()].name + ".ini";

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));

}


}









//uninstalls profile from the sekiro directory

void Sekiro::on_uninstallProfile_clicked()
{

    //checks if no profiles are installed, if there are no profiles installed, then it throws an error
    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Profile Selected");
    }


    //if there is a profile at the current index, then uninstalls it
    else{



    for(int i = 0; i < profiles[ui->profilesInstalled->currentIndex()].files.size(); i++){


        //goes through the files vector and gets the name for the file to be deleted, then deletes it

        string del = sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name + "/" + profiles[ui->profilesInstalled->currentIndex()].files[i];

        qDebug() << QString::fromStdString(del);

        QFile toBeDeleted(QString::fromStdString(del));

        toBeDeleted.remove();





        //deletes the profile folder in the sekiro directory

        QDir dir(QString::fromStdString(sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name));
        dir.removeRecursively();




        //removes modengine ini

        string modengineRemove = sekDir + ".\\modengine.ini";

        QFile file(QString::fromStdString(modengineRemove));

        file.remove();



        //copys the default modengine.ini to the sekiro directory

        string finalProfileModengine = sekDir + "\\modengine.ini";

        QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));



    }



}



}








//loads the installed profiles

void Sekiro::getSettingsProfile(){



    //gets all the ini files for profiles

    QDir directory(".\\configsP\\");
    QStringList configs = directory.entryList(QStringList() << "*.ini" ,QDir::Files);

    int i = 0;





    //for each ini files, installs the profile for that ini file

    foreach(QString filename, configs) {




        //gets profile name, path, and the path for the config

        ifstream config(".\\configsP\\" + filename.toStdString());

        getline(config, Profile.name);
        getline(config, Profile.path);
        getline(config, Profile.profileConfigPath);
        getline(config, Profile.profileFolder);

        string tmp;

        getline(config, tmp);


        Profile.modNum = stoi(tmp);

        qDebug() << QString::fromStdString(Profile.name);
        qDebug() << QString::fromStdString(Profile.path);
        qDebug() << QString::fromStdString(Profile.profileConfigPath);
        qDebug() << QString::fromStdString(Profile.profileFolder);
        qDebug() << Profile.modNum;






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







        //gets files for profiles

        QDir().mkdir(".\\tmp");

        unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\profiles\\"+ Profile.name + ".zip\"");

        traverseProfiles2("*.*", ".\\tmp\\", 1);

        debugFileList(2);





        //creates profile entry in vector of profiles

        profiles.push_back(Profile);





        //deletes tmp folder

        QDir dir(".\\tmp\\");
        dir.removeRecursively();






        //creates new mod entry in combo box

        ui->profilesInstalled->addItem(QString::fromStdString(profiles[i].name));

        i++;



    }




}












//looks in directory for profile files

void Sekiro::traverseProfiles2(const QString &pattern, const QString &dirname, int mode){



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
            traverseProfiles2(pattern, fileInfo.filePath(), mode);



        }



        //if mode = 1 and the fileinfo is a file then puts all of the files in the profiles.files vector, so that when the user wants to uninstall the mod, the program has all of
        //the names of the files to delete

        if(mode == 1){




        QString file = fileInfo.filePath().remove(0, 5);



        if(fileInfo.isFile()){

        Profile.files.push_back(file.toLocal8Bit().constData());

        //qDebug() << file;

        i++;

        }

      }



    }
}










//removes profile from the mod manager

void Sekiro::on_removeProfile_clicked()
{


    //checks if no profiles are installed, if there are no profiles installed, then it throws an error
    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Profile Selected");
    }



    //if there is a profile in profilesInstalled, it uninstalls it
    else{



    //removes profile config file

    QFile configDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));

    configDel.remove();



    //removes profile archive file

    QFile modDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].path + ".zip"));

    modDel.remove();




    //removes profile modengine.ini

    QFile modEngineDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].path + ".ini"));

    modEngineDel.remove();




    //erases entry in profiles vector

    profiles.erase(profiles.begin() + ui->profilesInstalled->currentIndex());



    //erases entry in profilesInstalled index

    ui->profilesInstalled->removeItem(ui->profilesInstalled->currentIndex());

    }




}









//sets the folder modenegine uses to the folder for the profile that is at the current index of the combo box

void Sekiro::on_setActiveProfile_clicked()
{


    //checks if no profiles are installed, if there are no profiles installed, then it throws an error
    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Profile Selected");
    }

    else{






    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    //copys the modengine.ini for the profile at the current index to the sekiro directory

    string profileModengine  = ".\\profiles\\" + profiles[ui->profilesInstalled->currentIndex()].name + ".ini";

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));


}

}









//sets the folder modenegine uses to the default, which is "mods" in the sekiro directory

void Sekiro::on_defaultProfile_clicked()
{



    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    //copys the default modengine.ini that was made on program startup to the sekiro directory

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));


}









//launches sekiro

void Sekiro::on_launchSekiro_clicked()
{


    //literally just launches sekiro lol

    unpackRepack("cd /d " + sekDir + " & START sekiro.exe");


}










//on startup, checks if modengine is installed, if not it downloads and installs it for the user

void Sekiro::modEngineCheck(){


    //checks if modengine is installed

    QFileInfo modengineInstallCheck(QString::fromStdString(sekDir + "\\modengine.ini"));

    //checks if "modengine" exists in the sekiro directory, if it doesnt, then makes asks the user if they want to install modengine
    if(!(modengineInstallCheck.exists())){



        //askes the user if they want to install modengine
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Modengine not installed", "Modengine is not currently installed  \nDo you want to install it?", QMessageBox::Yes|QMessageBox::No);



        //if the user clicks yes then downloads and installs modengine
        if (reply == QMessageBox::Yes) {


            QString url = "https://cf-files.nexusmods.com/cdn/2763/6/ModEngine-6-0-1-11-1555994013.zip?md5=pDU1_H4yZ5AVMB-Z0AEk6w&expires=1581884707&user_id=8105063&rip=74.88.98.97";
            QDesktopServices::openUrl (url);

             string downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toLocal8Bit().constData();


            qDebug() << QString::fromStdString(downloadsFolder);

            Sleep(2000);


            QDir().mkdir(".\\tmp");

            unpackRepack("cd %cd%   &   7za e -y \""+ downloadsFolder + "//ModEngine*.zip\" -o\""+ sekDir +"\" *.txt *.ini *.dll -r");

        }





    }

}








//enables or disables warning messages

void Sekiro::on_warnings_stateChanged()
{


    if(!ui->warnings->isChecked()){

        warning = false;

    }


    else if(ui->warnings->isChecked()){

        warning = true;

    }

}
