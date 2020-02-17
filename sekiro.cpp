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

string sekDir;

string modName;

struct mod Mod;


struct profile Profile;

vector<profile> profiles;

vector<mod> mods;

string trueModPath;

short isProfileDone = 0;

Sekiro::Sekiro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sekiro)
{
    ui->setupUi(this);

    setFixedSize(423, 401);



    checkDir();

    getSettings();

    getSettingsProfile();


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




    modEngineCheck();


}


Sekiro::~Sekiro()
{
    delete ui;
}












void Sekiro::on_addMod_clicked()
{

    //open file dialog

    //sets available filters for file extensions when choosing file
    QString filter = "All archives (*.rar *.zip *.7z) ;; RAR (*.rar)  ;;  Zip (*.zip) ;; 7z (*.7z) ";

    //Allows user to choose sekiro directory
    QString modNAME = QFileDialog::getOpenFileName(this, "Open Mod","C://",filter);


    if(!modNAME.isEmpty()&& !modNAME.isNull()){



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

    if(modExt == "7z" || modExt == "zip"){


        unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\mods\\"  + modName + "." + modExt.toLocal8Bit().constData() + "\"");


    }
    else if(modExt == "rar"){


        unpackRepack("cd %cd%    &    unrar x  -y \"%cd%\\mods\\"  + modName + ".rar\" * .\\tmp\\");

    }










    //restructures archive


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

    //debugFileList();





    //saves mod entry to file






    QDir toCount(".\\configs\\");

    toCount.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    int total_files = toCount.count();

    qDebug() << total_files;

    Mod.modConfigPath = ".\\configs\\" + Mod.name + ".ini";
    ofstream modConfig(Mod.modConfigPath);

    modConfig << Mod.name + "\n" + Mod.path + "\n" + Mod.modConfigPath;



    mods.push_back(Mod);





    //deletes tmp folder

    dir.removeRecursively();






    //creates new mod entry in combo box

    ui->modsInstalled->addItem(QString::fromStdString(modName));

}

}

void Sekiro::checkDir(){


    //makes folder "mods" in root program root directory
    QDir().mkdir("mods");

    //makes folder "configs" in root program root directory
    QDir().mkdir("configs");


    //makes folder "profiles" in root program root directory
    QDir().mkdir("configsP");

    //makes folder "profiles" in root program root directory
    QDir().mkdir("profiles");



    QFileInfo sekDirIni(".\\dir.ini");

    //checks if ".\dir.ini" exists, if it doesnt, then makes dir.ini
    if(!(sekDirIni.exists())){


        QFileDialog dialog;

        dialog.setFileMode(QFileDialog::Directory);
        dialog.setOption(QFileDialog::ShowDirsOnly);

        QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");

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



void Sekiro::traverse(const QString &pattern, const QString &dirname, int mode)
{
    QDir dir(dirname);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);

int i = 0;


    foreach (QFileInfo fileInfo, dir.entryInfoList()) {



        if (fileInfo.isDir() && fileInfo.isReadable()){


            if((fileInfo.baseName() == "parts" || fileInfo.baseName() == "event" ||fileInfo.baseName() == "map" || fileInfo.baseName() == "msg" ||
                    fileInfo.baseName() == "param" || fileInfo.baseName() == "script" || fileInfo.baseName() == "chr"  || fileInfo.baseName() == "cutscene"
                || fileInfo.baseName() == "event" || fileInfo.baseName() == "facegen" || fileInfo.baseName() == "font" || fileInfo.baseName() == "action"
                    || fileInfo.baseName() == "menu" || fileInfo.baseName() == "mtd" || fileInfo.baseName() == "obj" || fileInfo.baseName() == "other"
                    || fileInfo.baseName() == "sfx" || fileInfo.baseName() == "shader" ) && mode == 0){


                trueModPath = fileInfo.path().toLocal8Bit().constData();
                qDebug() << trueModPath.c_str();


            }

            traverse(pattern, fileInfo.filePath(), mode);



        }

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



void Sekiro::unpackRepack(string line){

    ofstream unpack;
    unpack.open( "commands.cmd", ios::trunc );

    unpack <<
        line << endl;

    unpack.close();



    system( "cmd.exe /c commands.cmd" );


    remove( "commands.cmd" );

}


void Sekiro::debugFileList(int mode){


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


void Sekiro::on_Install_clicked()
{


    if(ui->modsInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }


    else{

    unpackRepack("cd %cd%   &   7za e -spf -y -o\"" + sekDir + "\\mods\\\" \"%cd%\\mods\\"+ mods[ui->modsInstalled->currentIndex()].name + ".zip\" ");

}

}



void Sekiro::on_Uninstall_clicked()
{

    if(ui->modsInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }

    else{

    for(int i = 0; i < mods[ui->modsInstalled->currentIndex()].files.size(); i++){


        string del = sekDir + "/mods" + mods[ui->modsInstalled->currentIndex()].files[i];


        qDebug() << QString::fromStdString(del);

        QFile toBeDeleted(QString::fromStdString(del));



            toBeDeleted.remove();





    }



}



}



void Sekiro::getSettings(){




    QDir directory(".\\configs\\");
    QStringList configs = directory.entryList(QStringList() << "*.ini" ,QDir::Files);

    int i = 0;

    foreach(QString filename, configs) {






        ifstream config(".\\configs\\" + filename.toStdString());

        getline(config, Mod.name);
        getline(config, Mod.path);
        getline(config, Mod.modConfigPath);



        qDebug() << QString::fromStdString(Mod.name);
        qDebug() << QString::fromStdString(Mod.path);
        qDebug() << QString::fromStdString(Mod.modConfigPath);




        QDir().mkdir(".\\tmp");

        unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\mods\\"+ Mod.name + ".zip\"");

        traverse("*.*", ".\\tmp\\", 1);

        debugFileList(1);





        mods.push_back(Mod);





        //deletes tmp folder


        QDir dir(".\\tmp\\");
        dir.removeRecursively();






        //creates new mod entry in combo box

        ui->modsInstalled->addItem(QString::fromStdString(mods[i].name));

        i++;

    }

}




void Sekiro::on_removeMod_clicked()
{

    if(ui->modsInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }

    else{

    QFile configDel(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));

    configDel.remove();



    QFile modDel(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].path + ".zip"));

    modDel.remove();



    mods.erase(mods.begin() + ui->modsInstalled->currentIndex());




    ui->modsInstalled->removeItem(ui->modsInstalled->currentIndex());

    }
}





void Sekiro::on_changeSekDir_clicked()
{



    QFileDialog dialog;

    dialog.setFileMode(QFileDialog::Directory);
    dialog.setOption(QFileDialog::ShowDirsOnly);

    QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");

    if(!path.isEmpty()&& !path.isNull()){

    sekDir = path.toLocal8Bit().constData();

    ui->currentSekDir->setText(path);

    }



}





void Sekiro::on_addProfile_clicked()
{


    addProfile profileWindow;
    profileWindow.setModal(true);
    profileWindow.exec();

    profileWindow.setAttribute(Qt::WA_DeleteOnClose);


    while(isProfileDone == 0){

        qDebug() << "jfeowi";

    }

    ui->profilesInstalled->addItem(QString::fromStdString(profiles[ui->profilesInstalled->count()].name));


}






void Sekiro::on_installProfile_clicked()
{


    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Profile Selected");
    }


    else{


    string profileInstallFolder = sekDir + "";

    QDir().mkdir(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileFolder));

    unpackRepack("cd %cd%   &   7za e -spf -y -o\"" + sekDir + "\\"+profiles[ui->profilesInstalled->currentIndex()].name +"\\\" \"%cd%\\profiles\\"+ profiles[ui->profilesInstalled->currentIndex()].name + ".zip\" ");




    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    //moves profiles ini to sekiro directory




    string profileModengine  = ".\\profiles\\" + profiles[ui->profilesInstalled->currentIndex()].name + ".ini";

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));

}


}






void Sekiro::on_uninstallProfile_clicked()
{


    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Profile Selected");
    }

    else{

    for(int i = 0; i < profiles[ui->profilesInstalled->currentIndex()].files.size(); i++){


        string del = sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name + "/" + profiles[ui->profilesInstalled->currentIndex()].files[i];


        qDebug() << QString::fromStdString(del);

        QFile toBeDeleted(QString::fromStdString(del));



            toBeDeleted.remove();



            QDir dir(QString::fromStdString(sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name));
            dir.removeRecursively();




            //removes modengine ini

            string modengineRemove = sekDir + ".\\modengine.ini";

            QFile file(QString::fromStdString(modengineRemove));

            file.remove();



            string finalProfileModengine = sekDir + "\\modengine.ini";

            QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));



    }



}



}





void Sekiro::getSettingsProfile(){





    QDir directory(".\\configsP\\");
    QStringList configs = directory.entryList(QStringList() << "*.ini" ,QDir::Files);

    int i = 0;

    foreach(QString filename, configs) {






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








        QDir().mkdir(".\\tmp");

        unpackRepack("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\profiles\\"+ Profile.name + ".zip\"");

        traverseProfiles2("*.*", ".\\tmp\\", 1);

        debugFileList(2);





        profiles.push_back(Profile);





        //deletes tmp folder


        QDir dir(".\\tmp\\");
        dir.removeRecursively();






        //creates new mod entry in combo box

        ui->profilesInstalled->addItem(QString::fromStdString(profiles[i].name));

        i++;



    }




}




void Sekiro::traverseProfiles2(const QString &pattern, const QString &dirname, int mode){
    QDir dir(dirname);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);

int i = 0;


    foreach (QFileInfo fileInfo, dir.entryInfoList()) {



        if (fileInfo.isDir() && fileInfo.isReadable()){


            if((fileInfo.baseName() == "parts" || fileInfo.baseName() == "event" ||fileInfo.baseName() == "map" || fileInfo.baseName() == "msg" ||
                    fileInfo.baseName() == "param" || fileInfo.baseName() == "script" || fileInfo.baseName() == "chr"  || fileInfo.baseName() == "cutscene"
                || fileInfo.baseName() == "event" || fileInfo.baseName() == "facegen" || fileInfo.baseName() == "font" || fileInfo.baseName() == "action"
                    || fileInfo.baseName() == "menu" || fileInfo.baseName() == "mtd" || fileInfo.baseName() == "obj" || fileInfo.baseName() == "other"
                    || fileInfo.baseName() == "sfx" || fileInfo.baseName() == "shader" ) && mode == 0){


                modProfilePath = fileInfo.path().toLocal8Bit().constData();
                qDebug() << modProfilePath.c_str();


            }

            traverseProfiles2(pattern, fileInfo.filePath(), mode);



        }

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








void Sekiro::on_removeProfile_clicked()
{



    if(ui->profilesInstalled->count() <= 0){

        QMessageBox err;


       err.critical(this, "Error", "No Mod Selected");
    }

    else{

    QFile configDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));

    configDel.remove();



    QFile modDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].path + ".zip"));

    modDel.remove();



    QFile modEngineDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].path + ".ini"));

    modEngineDel.remove();


    profiles.erase(profiles.begin() + ui->profilesInstalled->currentIndex());




    ui->profilesInstalled->removeItem(ui->profilesInstalled->currentIndex());

    }




}





void Sekiro::on_setActiveProfile_clicked()
{



    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    string profileModengine  = ".\\profiles\\" + profiles[ui->profilesInstalled->currentIndex()].name + ".ini";

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));




}




void Sekiro::on_defaultProfile_clicked()
{



    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));


}





void Sekiro::on_launchSekiro_clicked()
{



    unpackRepack("cd /d " + sekDir + " & START sekiro.exe");


}







void Sekiro::modEngineCheck(){


    //checks if modengine is installed

    QFileInfo modengineInstallCheck(QString::fromStdString(sekDir + "\\modengine.ini"));

    //checks if ".\dir.ini" exists, if it doesnt, then makes dir.ini
    if(!(modengineInstallCheck.exists())){


        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Modengine not installed", "Modengine is not currently installed?  \nDo you want to install it?", QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {


            QString url = "https://cf-files.nexusmods.com/cdn/2763/6/ModEngine-6-0-1-11-1555994013.zip?md5=pDU1_H4yZ5AVMB-Z0AEk6w&expires=1581884707&user_id=8105063&rip=74.88.98.97";
            QDesktopServices::openUrl (url);

             string downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toLocal8Bit().constData();


            qDebug() << QString::fromStdString(downloadsFolder);

            Sleep(2000);


            QDir().mkdir(".\\tmp");

            unpackRepack("cd %cd%   &   7za e -y \""+ downloadsFolder + "//ModEngine-6-0-1-11-1555994013.zip\" -o\""+ sekDir +"\" *.txt *.ini *.dll -r & PAUSE");

        }





    }

}



