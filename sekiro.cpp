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
#include <algorithm>
#include <set>
#include <QFileInfoList>
#include <cctype>
#include <QMessageBox>
#include <cstring>
#include <QFontDatabase>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <QUrl>
#include <QDialogButtonBox>
#include <QDesktopServices>
#include "windows.h"
#include "repeatfile.h"

using namespace std;


bool  warning = false, isModValid = false, isProfileDone = false, modpackBool = false;

string sekDir, repeatFileName, modName, trueModPath = "", modpackName;

short repeatType;

struct mod Mod;

struct profile Profile;

set<string> modengineFolders = {"parts",
                                   "event",
                                   "map",
                                   "msg",
                                   "param",
                                   "script",
                                   "chr",
                                   "cutscene",
                                   "facegen",
                                   "font",
                                   "action",
                                   "menu",
                                   "mtd",
                                   "obj",
                                   "other",
                                   "sfx",
                                   "shader",
                                   "sound",
                                   "movie"};

vector<profile> profiles;

vector<mod> mods;

ofstream logFile(".\\log.txt");




Sekiro::Sekiro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sekiro)
{
    ui->setupUi(this);


    //sets font
    qDebug() << QFontDatabase::addApplicationFont(":/fonts/font/Assassin.ttf");

     QFont sekFont("Assassin$");
     QApplication::setFont(sekFont);








    //sets background of the main window
    setStyleSheet("Sekiro {background-image: url(:/uielements/uielements/Sekiro Mod Manager UI 720p.png) 0 0 0 0 stretch stretch;}");



    //sets the size

    setFixedSize(1280, 720);




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
    settings(0);


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();

    //see function definition
    modEngineCheck();



    //see function definion
    getActiveProfile();


    this->show();
    this->setWindowState(Qt::WindowState::WindowActive);


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


    //if mod is a modpacl
    if(modpackBool){

        //sets name of modpack
        modpackName = modName;




        //moves mod file to mods folder in program directory

        QFile file(modNAME);

        string modDir = ".\\mods\\" + modName + "." + modExt.toLocal8Bit().constData();


        QFile::copy(modNAME, QString::fromStdString(modDir));












        //creates tmp folder and extracts archive to temp

        QDir().mkdir(".\\modpack");

        //if mod is contained in a zip file
        if(modExt == "7z" || modExt == "zip"){


            unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\modpack\" \"%cd%\\mods\\"  + modName + "." + modExt.toLocal8Bit().constData() + "\"");


        }


        //if mod is contained in a rar file
        else if(modExt == "rar"){


            unpackRepack("cd \"%cd%\"    &    unrar x  -y \"%cd%\\mods\\"  + modName + ".rar\" * .\\modpack\\");

        }




        //bool that hold whether the user pressed cancel in filedialog
        bool modpackCancelCheck = false;









        //WHILE LOOP THAT CHECKS IF THE USER CANCELS A DIALOGE

        //while !cancel
        while(!modpackCancelCheck){






            //opens file dialogue for choose individual mod

            QString modpackMod = QFileDialog::getExistingDirectory(this, "Please select mod folder from modpack, or press cancel if you are finished installing mods", ".\\modpack\\");


            //checks if user pressed "cancel"
            if(!modpackMod.isEmpty() && !modpackMod.isNull()){




            //gets new mod name

            modname Modname;
            Modname.setModal(true);
            Modname.exec();

            Modname.setAttribute(Qt::WA_DeleteOnClose);




            //sets mods[n].name, mods[n].path, and mods[n].modNum

            modName = modName + "(" + modpackName + ")";

            Mod.name = modName;

            Mod.path = ".\\mods\\" + modName;




            //checks of mod is made up of loose files
            traverse("*.*", modpackMod.toLocal8Bit().constData(), 0);



            //if mod doesn't have loose files
            if(isModPathEmpty(trueModPath) == false){




            unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\"  + modName + ".zip\" \"" + trueModPath + "/*\"");


            QFile fileDel(modDir.c_str());

            fileDel.remove();





            QFile::copy(".\\tmp\\" + QString::fromStdString(modName) + ".zip", ".\\mods\\" + QString::fromStdString(modName) + ".zip");


            QDir dir(".\\tmp\\");
            dir.removeRecursively();








            //gets names of all files and puts all file names in mods[n].files[i]

            QDir().mkdir(".\\tmp");

            unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\mods\\"+ modName + ".zip\"");

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


            Mod.isInstalled = 'n';

            modConfig << Mod.isInstalled + "\n" + Mod.name + "\n" + Mod.path + "\n" + Mod.modConfigPath + "\n";
            modConfig << Mod.files.size() << "\n";


            for(int i = 0; i < Mod.files.size(); i++){


                modConfig << Mod.files[i] << endl;



            }

            modConfig.close();


            //pushes mod to vector of mods

            mods.push_back(Mod);


            //resets mod structure
            Mod.isInstalled = "";
            Mod.name = "";
            Mod.path = "";
            Mod.modConfigPath = "";
            Mod.files.clear();




            //deletes tmp folder

            dir.removeRecursively();






            //creates new mod entry in combo box

            ui->modsInstalled->addItem(QString::fromStdString(modName));

            log("Added: " + modName);

            trueModPath = "";

            }

            //if mod has loose files
            else if(isModPathEmpty(trueModPath) == true){

                isModValid = false;

                traverse("*.*", modpackMod.toLocal8Bit().constData(), 2);

                if(isModValid){

                    unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\tmp1\\"  + modName + ".zip\" \".\\tmp\\tmp1\\*\"");

                    QFile fileDel(modDir.c_str());

                    fileDel.remove();





                    QFile::copy(".\\tmp\\tmp1\\" + QString::fromStdString(modName) + ".zip", ".\\mods\\" + QString::fromStdString(modName) + ".zip");


                    QDir dir(".\\tmp\\");
                    dir.removeRecursively();








                    //gets names of all files and puts all file names in mods[n].files[i]

                    QDir().mkdir(".\\tmp");

                    unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\mods\\"+ modName + ".zip\"");

                    traverse("*.*", ".\\tmp\\", 1);



                    //see function definition
                    debugFileList(1);






                    //creates mod config file
                    Mod.modConfigPath = ".\\configs\\" + Mod.name + ".ini";
                    ofstream modConfig(Mod.modConfigPath);

                    Mod.isInstalled = 'n';

                    modConfig << Mod.isInstalled + "\n" + Mod.name + "\n" + Mod.path + "\n" + Mod.modConfigPath + "\n";
                    modConfig << Mod.files.size() << "\n";


                    for(int i = 0; i < Mod.files.size(); i++){


                        modConfig << Mod.files[i] << endl;



                    }

                    modConfig.close();


                    //pushes mod to vector of mods

                    mods.push_back(Mod);


                    //resets mod structure
                    Mod.isInstalled = "";
                    Mod.name = "";
                    Mod.path = "";
                    Mod.modConfigPath = "";
                    Mod.files.clear();




                    //deletes tmp folder

                    dir.removeRecursively();






                    //creates new mod entry in combo box

                    ui->modsInstalled->addItem(QString::fromStdString(modName));

                    log("Added: " + modName);

                    trueModPath = "";







                }


                else{
                QFont sekFont("Assassin$");
                QFont errFont("Segoe UI", 8);

                QMessageBox err;

                QApplication::setFont(errFont);

                err.critical(this, "Error", "No mod files found");

                QFile fileDel(modDir.c_str());

                fileDel.remove();


                QDir dir(".\\tmp\\");
                dir.removeRecursively();



               QApplication::setFont(sekFont);

               log("Error: No mod files found");

                }

            }








            }
            else{

                qDebug() << "CANCEL";
                modpackCancelCheck = true;

            }



        }




        //delete modpack file and modpack folder
        QFile::remove(".\\mods\\"+ QString::fromStdString(modpackName));

        QDir modPackDir(".\\modpack");
        modPackDir.removeRecursively();


        //set modpackbool to false so the checkbox shows up next time the user add a mod
        modpackBool = false;

        //resets modpackName
        modpackName = "";
    }
    //if it isnt a modpack
    else{




    //moves mod file to mods folder in program directory

    QFile file(modNAME);

    string modDir = ".\\mods\\" + modName + "." + modExt.toLocal8Bit().constData();




    QFile::copy(modNAME, QString::fromStdString(modDir));





    //sets mods[n].name, mods[n].path, and mods[n].modNum

    Mod.name = modName;

    Mod.path = ".\\mods\\" + modName;






    //creates tmp folder and extracts archive to temp

    QDir().mkdir(".\\tmp");

    //if mod is contained in a zip file
    if(modExt == "7z" || modExt == "zip"){


        unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\mods\\"  + modName + "." + modExt.toLocal8Bit().constData() + "\"");


    }


    //if mod is contained in a rar file
    else if(modExt == "rar"){


        unpackRepack("cd \"%cd%\"    &    unrar x  -y \"%cd%\\mods\\"  + modName + ".rar\" * .\\tmp\\");

    }










    //restructures archive, the reason for doing this is while almost every mod uses the unpacked sekiro folders like "parts" "chr" etc
    //some mod compressed archives might have their files in an extra folder, like one mod archive might have a directory like bossrushV.012/bossrush/mod folders
    //and another might have sekiro/mod folder so by restructuring, it makes sure that the mod folders ("parts" "chr" "mtd" etc) are in the root folder of the mod archive
    //for later extracting



    traverse("*.*", ".\\tmp\\", 0);


    //if mod doesn't have loose files
    if(isModPathEmpty(trueModPath) == false){




    unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\"  + modName + ".zip\" \"" + trueModPath + "/*\"");


    QFile fileDel(modDir.c_str());

    fileDel.remove();





    QFile::copy(".\\tmp\\" + QString::fromStdString(modName) + ".zip", ".\\mods\\" + QString::fromStdString(modName) + ".zip");


    QDir dir(".\\tmp\\");
    dir.removeRecursively();








    //gets names of all files and puts all file names in mods[n].files[i]

    QDir().mkdir(".\\tmp");

    unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\mods\\"+ modName + ".zip\"");

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

    Mod.isInstalled = 'n';

    modConfig << Mod.isInstalled + "\n" + Mod.name + "\n" + Mod.path + "\n" + Mod.modConfigPath + "\n";
    modConfig << Mod.files.size() << "\n";


    for(int i = 0; i < Mod.files.size(); i++){


        modConfig << Mod.files[i] << endl;



    }

    modConfig.close();


    //pushes mod to vector of mods

    mods.push_back(Mod);


    //resets mod structure
    Mod.isInstalled = "";
    Mod.name = "";
    Mod.path = "";
    Mod.modConfigPath = "";
    Mod.files.clear();




    //deletes tmp folder

    dir.removeRecursively();






    //creates new mod entry in combo box

    ui->modsInstalled->addItem(QString::fromStdString(modName));

    log("Added: " + modName);

    trueModPath = "";

    }

    //if mod has loose files
    else if(isModPathEmpty(trueModPath) == true){

        isModValid = false;

        traverse("*.*", ".\\tmp\\", 2);

        if(isModValid){

            unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\tmp1\\"  + modName + ".zip\" \".\\tmp\\tmp1\\*\"");

            QFile fileDel(modDir.c_str());

            fileDel.remove();





            QFile::copy(".\\tmp\\tmp1\\" + QString::fromStdString(modName) + ".zip", ".\\mods\\" + QString::fromStdString(modName) + ".zip");


            QDir dir(".\\tmp\\");
            dir.removeRecursively();








            //gets names of all files and puts all file names in mods[n].files[i]

            QDir().mkdir(".\\tmp");

            unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\mods\\"+ modName + ".zip\"");

            traverse("*.*", ".\\tmp\\", 1);



            //see function definition
            debugFileList(1);






            //creates mod config file
            Mod.modConfigPath = ".\\configs\\" + Mod.name + ".ini";
            ofstream modConfig(Mod.modConfigPath);

            Mod.isInstalled = 'n';

            modConfig << Mod.isInstalled + "\n" + Mod.name + "\n" + Mod.path + "\n" + Mod.modConfigPath + "\n";
            modConfig << Mod.files.size() << "\n";


            for(int i = 0; i < Mod.files.size(); i++){


                modConfig << Mod.files[i] << endl;



            }

            modConfig.close();


            //pushes mod to vector of mods

            mods.push_back(Mod);


            //resets mod structure
            Mod.isInstalled = "";
            Mod.name = "";
            Mod.path = "";
            Mod.modConfigPath = "";
            Mod.files.clear();




            //deletes tmp folder

            dir.removeRecursively();






            //creates new mod entry in combo box

            ui->modsInstalled->addItem(QString::fromStdString(modName));

            log("Added: " + modName);

            trueModPath = "";







        }


        else{
        QFont sekFont("Assassin$");
        QFont errFont("Segoe UI", 8);

        QMessageBox err;

        QApplication::setFont(errFont);

        err.critical(this, "Error", "No mod files found");

        QFile fileDel(modDir.c_str());

        fileDel.remove();


        QDir dir(".\\tmp\\");
        dir.removeRecursively();



       QApplication::setFont(sekFont);

       log("Error: No mod files found");

        }

    }


    }


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


        bool isFolderValid = false;

        while (isFolderValid == false){


            //prompts the user for sekiro directory

            QFileDialog dialog;

            dialog.setFileMode(QFileDialog::Directory);
            dialog.setOption(QFileDialog::ShowDirsOnly);

            QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");




            sekDir = path.toLocal8Bit().constData();

            //checks if the directory is legit, if it is then puts it into sekDir
            if(sekiroCheck() == 1){

                qDebug() << sekiroCheck();


            ui->currentSekDir->setText(path);


            QFile dir("dir.ini");
            dir.remove();


            ofstream dirNew(".\\dir.ini");
            dirNew << sekDir;
            dirNew.close();

            isFolderValid = true;

            }




}




    }



    else{

        ifstream in(".\\dir.ini");


        getline(in, sekDir);



        bool isFolderValid = false;

        while (isFolderValid == false){




            //checks if the directory is legit, if it is then puts it into sekDir
            if(sekiroCheck() == 1){

                qDebug() << sekiroCheck();


            ui->currentSekDir->setText(QString::fromStdString(sekDir));


            QFile dir("dir.ini");
            dir.remove();


            ofstream dirNew(".\\dir.ini");
            dirNew << sekDir;
            dirNew.close();

            isFolderValid = true;

            }
            else{

                //prompts the user for sekiro directory

                QFileDialog dialog;

                dialog.setFileMode(QFileDialog::Directory);
                dialog.setOption(QFileDialog::ShowDirsOnly);

                QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");

                sekDir = path.toLocal8Bit().constData();

            }




}







    }




    //hides line edit
    ui->logTextEdit->hide();




}






//looks in directory for mod files

void Sekiro::traverse(const QString &pattern, const QString &dirname, int mode, bool mod)
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


            //converts folder name to lowercase
            string folderName = fileInfo.baseName().toLocal8Bit().constData();
            transform(folderName.begin(),folderName.end(),folderName.begin(), tolower);



            if(modengineFolders.find(folderName) != modengineFolders.end() && mode == 0){


                if(!mod){

                    modProfilePath = fileInfo.path().toLocal8Bit().constData();
                }
                else{

                trueModPath = fileInfo.path().toLocal8Bit().constData();

                }



            }


            else{

               //calls function so it searches all the sub directories

                if(!mod){

                   traverse(pattern, fileInfo.filePath(), mode, false);
                }
                else{

                traverse(pattern, fileInfo.filePath(), mode);

                }



            }



        }


        //if mode = 1 and the fileinfo is a file then puts all of the files in the mods.files/profiles.files vector, so that when the user wants to uninstall the mod, the program has all of
        //the names of the files to delete
        if(mode == 1){


        QString file = fileInfo.filePath().remove(0, 5);



        if(fileInfo.isFile()){


            if(!mod){

               Profile.files.push_back(file.toLocal8Bit().constData());
            }
            else{

            Mod.files.push_back(file.toLocal8Bit().constData());

            }



        i++;

        }




      }

        //sorts loose files
        if(mode == 2){



            QDir tmp1(".\\tmp\\tmp1\\");
            if(!tmp1.exists()){

                mkdir(".\\tmp\\");
                mkdir(".\\tmp\\tmp1\\");

            }




            string looseFileName = fileInfo.fileName().toUtf8().constData();

            //"action" folder
            if(looseFileName.find(".txt") != string::npos){
                isModValid = true;


                QDir parts(".\\tmp\\tmp1\\action\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\action\\");




                }

                 QDir newLooseDir = fileInfo.dir();
                 QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\action\\" + QString::fromStdString(looseFileName));


             }
            else if(looseFileName.find(".hks") != string::npos){
                isModValid = true;

                QDir parts(".\\tmp\\tmp1\\action\\script\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\action\\");
                    mkdir(".\\tmp\\tmp1\\action\\script\\");



                }


                QDir newLooseDir = fileInfo.dir();
                qDebug() << QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\action\\script\\" + QString::fromStdString(looseFileName)) << "BGOI$BGBORGGEG";
                qDebug() << newLooseDir.path() << "GNEIOBGIBOEBGOBGIBEOBNGI";
                qDebug() << QString::fromStdString(looseFileName) << "GNEIOBGIBOEBGOBGIBEOBNGI";

            }


            //"chr" folder
            else if(looseFileName.find(".ani") != string::npos || looseFileName.find(".tex") != string::npos || looseFileName.find(".beh") != string::npos ||looseFileName.find(".chr") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\chr\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\chr\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\chr\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //cutscene folder
            else if(looseFileName.find(".cutscene") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\cutscene\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\cutscene\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\cutscene\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);
            }


            //event folder
            else if(looseFileName.find(".emevd") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\event\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\event\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\event\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);
            }


            //facegen folder
            else if(looseFileName.find("facegen.fgbnd.dcx") != string::npos){
                isModValid = true;

                QDir parts(".\\tmp\\tmp1\\facegen\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\facegen\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\facegen\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //parts folder
            else if(looseFileName.find(".partsbnd") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\parts\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\parts\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\parts\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);


            }



            //script folder
            else if(looseFileName.find("talke") != string::npos){
                isModValid = true;

                QDir parts(".\\tmp\\tmp1\\script\\talk\\");

                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\script\\");
                    mkdir(".\\tmp\\tmp1\\script\\talk\\");

                }


                QDir newLooseDir = fileInfo.dir();
                qDebug() << QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\script\\talk\\" + QString::fromStdString(looseFileName)) << "BGOI$BGBORGGEG";
                qDebug() << newLooseDir.path() << "GNEIOBGIBOEBGOBGIBEOBNGI";
                qDebug() << QString::fromStdString(looseFileName) << "GNEIOBGIBOEBGOBGIBEOBNGI";

            }
            else if(looseFileName.find(".lua") != string::npos){
                    isModValid = true;
                    QDir parts(".\\tmp\\tmp1\\script\\");
                    if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\script\\");


                }



                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\script\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }




            //sfx
            else if(looseFileName.find(".ffx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\sfx\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\sfx\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\sfx\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }



            //shader
            else if(looseFileName.find(".shader") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\shader\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\shader\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\shader\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //sound
            else if(looseFileName.find(".itl") != string::npos ||looseFileName.find(".fev") != string::npos || looseFileName.find(".fsb") != string::npos ||
                    looseFileName.find("multich.mch") != string::npos || looseFileName.find("multimix.mix") != string::npos || looseFileName.find("multirpc.rpc") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\sound\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\sound\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\sound\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //movie
            else if(looseFileName.find(".bk2") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\movie\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\movie\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\movie\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //mtd
            else if(looseFileName.find("allmaterialbnd.mtdbnd.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\mtd\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\mtd\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\mtd\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //obj
            else if(looseFileName.find(".obj") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\obj\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\obj\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\obj\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //other
            else if(looseFileName.find("decaltex.tpf.dcx") != string::npos || looseFileName.find("default.rumblebnd.dcx") != string::npos ||looseFileName.find("ingamestay.loadlist") != string::npos ||
                    looseFileName.find("maptex.tpf.dcx") != string::npos || looseFileName.find("modelviewer_default") != string::npos || looseFileName.find("movtae.movtae.dcx") != string::npos || looseFileName.find("systex.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\other\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\other\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\other\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //param
            else if(looseFileName.find(".gparam") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\param\\drawparam\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\param\\");
                    mkdir(".\\tmp\\tmp1\\param\\drawparam\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\param\\drawparam\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if(looseFileName.find("gameparam.parambnd.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\param\\gameparam\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\param\\");
                    mkdir(".\\tmp\\tmp1\\param\\gameparam\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\param\\gameparam\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if(looseFileName.find("graphicsconfig.parambnd.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\param\\graphicsconfig\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\param\\");
                    mkdir(".\\tmp\\tmp1\\param\\graphicsconfig\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\param\\graphicsconfig\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //parts
            else if(looseFileName.find(".partsbnd") != string::npos || looseFileName.find("common_body.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\parts\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\parts\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\parts\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //map
            else if((looseFileName.find("m10") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m10") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m10_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m10\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m10\\");

                }

                QDir newLooseDir = fileInfo.dir();
                qDebug() << QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m10\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("m11") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m11") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m11_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m11\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m11\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m11\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("m13") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m13") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m13_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m13\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m13\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m13\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("m15") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m15") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m15_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m15\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m15\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m15\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("m17") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m17") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m17_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m17\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m17\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m17\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("m20") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m20") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m20_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m20\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m20\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m20\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("m25") != string::npos && looseFileName.find(".tpfbdt") != string::npos) || looseFileName.find("m25") != string::npos && looseFileName.find(".tpfbhd") != string::npos || looseFileName.find("m25_cgrading.tpf.dcx") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m25\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m25\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m25\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }

            else if((looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("10_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m10_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m10_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m10_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("11_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m11_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m11_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m11_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("11_01_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m11_01_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m11_01_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m11_01_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("11_02_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m11_02_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m11_02_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m11_02_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("13_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m13_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m13_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m13_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("15_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m15_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m15_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m15_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("17_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m17_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m17_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m17_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("20_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m20_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m20_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m20_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("25_00_00_00") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m25_00_00_00\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m25_00_00_00\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m25_00_00_00\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if((looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".mapbnd.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".hkxbdt") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".hkxbhd") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".btl.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".partsgroup.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".nva.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".nvmhktbnd.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".tpf.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".param.dcx") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".edgebdt") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".edgebhd") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".wallbdt") != string::npos) ||
                    (looseFileName.find("89_00_00_99") != string::npos && looseFileName.find(".wallbhd") != string::npos)){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\m89_00_00_99\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\m89_00_00_99\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\m89_00_00_99\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }




            else if(looseFileName.find(".entryfilelist") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\entryfilelist\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\entryfilelist\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\entryfilelist\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if(looseFileName.find(".onav") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\onav\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\onav\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\onav\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if(looseFileName.find(".msb") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\mapstudio\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");
                    mkdir(".\\tmp\\tmp1\\map\\mapstudio\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\mapstudio\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }
            else if(looseFileName.find("worldmsblist.worldloadlistlist") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\map\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\map\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\map\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //menu
            else if(looseFileName.find(".gfx") != string::npos && looseFileName[0] == '0'){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\menu\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\menu\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\menu\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //font
            else if(looseFileName.find("dbgfont14h") != string::npos){
                isModValid = true;
                QDir parts(".\\tmp\\tmp1\\font\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\font\\");

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\font\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);

            }


            //REPEATED FILES

            //font
            else if(looseFileName.find("font.gfx") != string::npos){
                isModValid = true;
                repeatFileName = looseFileName;
                repeatType = 0;

                repeatFile repeat;
                repeat.setModal(true);
                repeat.exec();

                repeat.setAttribute(Qt::WA_DeleteOnClose);


                QDir parts(".\\tmp\\tmp1\\font\\" + QString::fromStdString(repeatFileName) + "\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\font\\");

                    string tempDir = ".\\tmp\\tmp1\\font\\" + repeatFileName + "\\";
                    mkdir(tempDir.c_str());

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\font\\" + QString::fromStdString(repeatFileName) + "\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);
                qDebug() << QString::fromStdString(repeatFileName);

            }

            //msg 2 letter folders
            else if(looseFileName.find("sellregion.msgbnd.dcx") != string::npos){
                isModValid = true;
                repeatFileName = looseFileName;
                repeatType = 1;

                repeatFile repeat;
                repeat.setModal(true);
                repeat.exec();

                repeat.setAttribute(Qt::WA_DeleteOnClose);


                QDir parts(".\\tmp\\tmp1\\msg\\" + QString::fromStdString(repeatFileName) + "\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\msg\\");

                    string tempDir = ".\\tmp\\tmp1\\msg\\" + repeatFileName + "\\";
                    mkdir(tempDir.c_str());

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\msg\\" + QString::fromStdString(repeatFileName) + "\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);
                qDebug() << QString::fromStdString(repeatFileName);

            }


            //msg other folders
            else if(looseFileName.find("menu.msgbnd.dcx") != string::npos || looseFileName.find("item.msgbnd.dcx") != string::npos){
                isModValid = true;
                repeatFileName = looseFileName;
                repeatType = 2;

                repeatFile repeat;
                repeat.setModal(true);
                repeat.exec();

                repeat.setAttribute(Qt::WA_DeleteOnClose);


                QDir parts(".\\tmp\\tmp1\\msg\\" + QString::fromStdString(repeatFileName) + "\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\msg\\");

                    string tempDir = ".\\tmp\\tmp1\\msg\\" + repeatFileName + "\\";
                    mkdir(tempDir.c_str());

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\msg\\" + QString::fromStdString(repeatFileName) + "\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);
                qDebug() << QString::fromStdString(repeatFileName);

            }


            //menu
            else if(looseFileName.find("menu_load") != string::npos || looseFileName.find("01_common") != string::npos || looseFileName.find("05_dummy.tpf.dcx") != string::npos){
                isModValid = true;
                repeatFileName = looseFileName;
                repeatType = 3;

                repeatFile repeat;
                repeat.setModal(true);
                repeat.exec();

                repeat.setAttribute(Qt::WA_DeleteOnClose);


                QDir parts(".\\tmp\\tmp1\\menu\\" + QString::fromStdString(repeatFileName) + "\\");
                if(!parts.exists()){

                    mkdir(".\\tmp\\tmp1\\menu\\");

                    string tempDir = ".\\tmp\\tmp1\\menu\\" + repeatFileName + "\\";
                    mkdir(tempDir.c_str());

                }

                QDir newLooseDir = fileInfo.dir();
                QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\menu\\" + QString::fromStdString(repeatFileName) + "\\" + QString::fromStdString(looseFileName));
                qDebug() << newLooseDir.path();
                qDebug() << QString::fromStdString(looseFileName);
                qDebug() << QString::fromStdString(repeatFileName);

            }


            //menu mapimage
            else if(looseFileName.find(".tpf.dcx") != string::npos && looseFileName[0] == '0' && looseFileName[2] == '_'){
                isModValid = true;
                repeatFileName = looseFileName;
                repeatType = 4;

                repeatFile repeat;
                repeat.setModal(true);
                repeat.exec();

                repeat.setAttribute(Qt::WA_DeleteOnClose);


                if(repeatFileName == "mapimage(hi)"){


                    QDir parts(".\\tmp\\tmp1\\menu\\hi\\mapimage\\");
                    if(!parts.exists()){

                        mkdir(".\\tmp\\tmp1\\menu\\");
                        mkdir(".\\tmp\\tmp1\\menu\\hi\\");
                        mkdir(".\\tmp\\tmp1\\menu\\hi\\mapimage\\");



                    }

                    QDir newLooseDir = fileInfo.dir();
                    QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\menu\\hi\\mapimage\\" + QString::fromStdString(looseFileName));
                    qDebug() << newLooseDir.path();
                    qDebug() << QString::fromStdString(looseFileName);
                    qDebug() << QString::fromStdString(repeatFileName);


                }
                else if (repeatFileName == "mapimage(low)"){

                    QDir parts(".\\tmp\\tmp1\\menu\\low\\mapimage\\");
                    if(!parts.exists()){

                        mkdir(".\\tmp\\tmp1\\menu\\");
                        mkdir(".\\tmp\\tmp1\\menu\\low\\");
                        mkdir(".\\tmp\\tmp1\\menu\\low\\mapimage\\");



                    }

                    QDir newLooseDir = fileInfo.dir();
                    QFile::copy(fileInfo.path() + "\\" + QString::fromStdString(looseFileName), ".\\tmp\\tmp1\\menu\\low\\mapimage\\" + QString::fromStdString(looseFileName));
                    qDebug() << newLooseDir.path();
                    qDebug() << QString::fromStdString(looseFileName);
                    qDebug() << QString::fromStdString(repeatFileName);

                }



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

        error(0);

        log("Error: No mod selected");

    }


    else{




        if(warning == true){

            unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"" + sekDir + "\\mods\\\" \"%cd%\\mods\\"+ mods[ui->modsInstalled->currentIndex()].name + ".zip\" ");

            log("Installed: " + mods[ui->modsInstalled->currentIndex()].name);



            //edits config
            ifstream filein(mods[ui->modsInstalled->currentIndex()].modConfigPath); //orig file
            ofstream fileout("TMP.ini"); //Temp file



            //take entire line as a string
            short counter = 0;
            for(string line; getline( filein, line ); )
            {

                if(counter == 0){

                    line = "y";


                }

                counter++;


               fileout << line << endl;
            }



            //closes file streams
            filein.close();
            fileout.close();


            //switches orig config with final/edited one
            QFile::remove(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));
            QFile::rename(".\\TMP.ini", QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));




            //add icon to combobox
            ui->modsInstalled->setItemIcon(ui->modsInstalled->currentIndex(),QIcon(":/uielements/uielements/sekiroCheck.PNG"));


            //updates struct
            mods[ui->modsInstalled->currentIndex()].isInstalled = "y";

        }
        else if (warning == false){




        QMessageBox r;

        r.setWindowTitle("Overwrite");
        r.setText(tr("Installing a mod might overwrite a previously installed mod  \nDo you still want to install this mod?"));
        QAbstractButton* pButtonYes = r.addButton(tr("Yes"), QMessageBox::YesRole);
        r.addButton(tr("No"), QMessageBox::NoRole);

        QFont errFont("Segoe UI", 8);


        r.setFont(errFont);

        r.exec();





        //if the user clicks yes then installs mod and if there is a mod at the current index, then unpacks the mod files into the sekiro directory
        if (r.clickedButton() == pButtonYes) {

            unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"" + sekDir + "\\mods\\\" \"%cd%\\mods\\"+ mods[ui->modsInstalled->currentIndex()].name + ".zip\" ");

            log("Installed: " + mods[ui->modsInstalled->currentIndex()].name);


            //edits config
            ifstream filein(mods[ui->modsInstalled->currentIndex()].modConfigPath); //orig file
            ofstream fileout("TMP.ini"); //Temp file



            //take entire line as a string
            short counter = 0;
            for(string line; getline( filein, line ); )
            {

                if(counter == 0){

                    line = "y";


                }

                counter++;


               fileout << line << endl;
            }



            //closes file streams
            filein.close();
            fileout.close();


            //switches orig config with final/edited one
            QFile::remove(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));
            QFile::rename(".\\TMP.ini", QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));


            //add icon to combobox
            ui->modsInstalled->setItemIcon(ui->modsInstalled->currentIndex(),QIcon(":/uielements/uielements/sekiroCheck.PNG"));


            //updates struct
            mods[ui->modsInstalled->currentIndex()].isInstalled = "y";

        }


        }

   }


}










//uninstalls mod from sekiro directory

void Sekiro::on_Uninstall_clicked()
{
    //checks if there are no mods installed, if there isnt, then throws an error
    if(ui->modsInstalled->count() <= 0){

        error(0);

        log("Error: No mod selected");

    }


    //checks if mod the user is trying to uninstall is not even installed in the first place
    if(mods[ui->modsInstalled->currentIndex()].isInstalled == "n"){

        QFont sekFont("Assassin$");
        QFont errFont("Segoe UI", 8);

        QMessageBox err;

        QApplication::setFont(errFont);

        err.critical(this, "Error", "Mod already isn't installed");







       QApplication::setFont(sekFont);

       log("Error: Mod already isn't installed");

    }

    else{

    for(int i = 0; i < mods[ui->modsInstalled->currentIndex()].files.size(); i++){


        //goes thorugh the files vector and gets the name for the file to be deleted, then deletes it

        string del = sekDir + "/mods" + mods[ui->modsInstalled->currentIndex()].files[i];

        qDebug() << QString::fromStdString(del);

        QFile toBeDeleted(QString::fromStdString(del));

        toBeDeleted.remove();





    }

    log("Uninstalled: " + mods[ui->modsInstalled->currentIndex()].name);


    //edits config
    ifstream filein(mods[ui->modsInstalled->currentIndex()].modConfigPath); //orig file
    ofstream fileout("TMP.ini"); //Temp file



    //take entire line as a string
    short counter = 0;
    for(string line; getline( filein, line ); )
    {

        if(counter == 0){

            line = "n";


        }

        counter++;


       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));
    QFile::rename(".\\TMP.ini", QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));


    //removes icon from combobox
    ui->modsInstalled->setItemIcon(ui->modsInstalled->currentIndex(), QIcon(":/uielements/uielements/sekiroFakeIcon.png"));



    //updates struct
    mods[ui->modsInstalled->currentIndex()].isInstalled = "n";
}



}











//loads the installed mods

void Sekiro::getSettings(){

    //gets settings for misc modengine
    parseSettings();



    //gets all of the ini files for mods

    QDir directory(".\\configs\\");
    QStringList configs = directory.entryList(QStringList() << "*.ini" ,QDir::Files);

    int i = 0;



    //for each ini file, installs the mod for that ini file

    foreach(QString filename, configs) {




        //gets mod name, path, and the path for the config

        string fileAmountS;

        ifstream config(".\\configs\\" + filename.toStdString());

        getline(config, Mod.isInstalled);
        getline(config, Mod.name);
        getline(config, Mod.path);
        getline(config, Mod.modConfigPath);
        getline(config, fileAmountS);

        int fileAmount = stoi(fileAmountS);




        for(int j = 0; j <= fileAmount; j++){

            string tmp;
            getline(config, tmp);

           Mod.files.push_back(tmp);



        }



        qDebug() << QString::fromStdString(Mod.name);
        qDebug() << QString::fromStdString(Mod.path);
        qDebug() << QString::fromStdString(Mod.modConfigPath);
         debugFileList(1);





        //creates mod entry in vector of mods

        mods.push_back(Mod);




        //resets mod structure
        Mod.isInstalled = "";
        Mod.name = "";
        Mod.path = "";
        Mod.modConfigPath = "";
        Mod.files.clear();


        //deletes tmp folder

        QDir dir(".\\tmp\\");
        dir.removeRecursively();






        //creates new mod entry in combo box

        ui->modsInstalled->addItem(QString::fromStdString(mods[i].name));


        //sets icon
        if(mods[i].isInstalled == "y"){

            //add icon to combobox
            ui->modsInstalled->setItemIcon(i,QIcon(":/uielements/uielements/sekiroCheck.PNG"));


        }
        else{
            //removes icon from combobox
            ui->modsInstalled->setItemIcon(i, QIcon(":/uielements/uielements/sekiroFakeIcon.png"));
        }

        i++;

    }

}









//removes mod from manager

void Sekiro::on_removeMod_clicked()
{


    //checks if no mods are installed, if there are no mods installed, then it throws an error
    if(ui->modsInstalled->count() <= 0){

        error(0);

        log("Error: No mod selected");
    }


    //if there is a mod at the current index then it uninstalls it
    else{




    //uninstalls mod config file

    QFile configDel(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].modConfigPath));

    configDel.remove();




    //uninstalls mod archive

    QFile modDel(QString::fromStdString(mods[ui->modsInstalled->currentIndex()].path + ".zip"));

    modDel.remove();


    log("Removed: " + mods[ui->modsInstalled->currentIndex()].name);

    //deletes mod entry in vector

    mods.erase(mods.begin() + ui->modsInstalled->currentIndex());






    //deletes mod entry in combp box

    ui->modsInstalled->removeItem(ui->modsInstalled->currentIndex());




    }
}










//changes sekiro directory

void Sekiro::on_changeSekDir_clicked()
{

        bool isFolderValid = false;

        string sekDirBefore = sekDir;

        while (isFolderValid == false){


            //prompts the user for sekiro directory

            QFileDialog dialog;

            dialog.setFileMode(QFileDialog::Directory);
            dialog.setOption(QFileDialog::ShowDirsOnly);



            QString path = dialog.getExistingDirectory(this, "Open Sekiro Folder");

            sekDir = path.toLocal8Bit().constData();

            //checks if the directory is legit, if it is then puts it into sekDir
            if(sekiroCheck() == 1){

                qDebug() << sekiroCheck();


            ui->currentSekDir->setText(path);


            QFile dir("dir.ini");
            dir.remove();


            ofstream dirNew(".\\dir.ini");
            dirNew << sekDir;
            dirNew.close();

            isFolderValid = true;

            }




}


        log("Directory Changed: " + sekDir);




        //reapplies modengine settings if user has that option checked
        if(ui->keepModengineSettings->isChecked()){

            /*copys the modengine.ini for the profile at the current index to the sekiro directory

            string profileModengine = ui->activeProfile->text().toLocal8Bit().constData();
            profileModengine  = ".\\profiles\\" + profileModengine + ".ini";

            string finalProfileModengine = sekDir + "\\modengine.ini";

            QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));


            applySettings(); */




            string finalProfileModengine = sekDir + "/modengine.ini";
            string profileModengine = sekDirBefore + "/modengine.ini";

            qDebug() << QString::fromStdString(profileModengine) << QString::fromStdString(finalProfileModengine);

            QFile::remove(QString::fromStdString(finalProfileModengine));

            QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));





        }
        else{


            //if modengine isn't found

            QFileInfo modengineInstallCheck(QString::fromStdString(sekDir + "\\modengine.ini"));

            //checks if "modengine" exists in the sekiro directory, if it doesnt, then makes asks the user if they want to install modengine
            if(!(modengineInstallCheck.exists())){

                //copys the default modengine.ini that was made on program startup to the sekiro directory

                string finalProfileModengine = sekDir + "\\modengine.ini";

                QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));

            }

            parseSettings();

        }



        //reinstalls mods and profiles if the user has the option checked
        if(ui->reinstallAfterDirChange->isChecked()){

            //reinstalls mods
            for(int i = 0; i < mods.size(); i++){

                if(mods[i].isInstalled == "y"){


                    unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"" + sekDir + "\\mods\\\" \"%cd%\\mods\\"+ mods[i].name + ".zip\" ");

                }


            }



            //reinstalls profiles
            for(int i = 0; i < profiles.size(); i++){

                if(profiles[i].isInstalledP == "y"){

                    qDebug() << "GE";
                    //unpacks the profile files into the sekiro directory

                    string profileInstallFolder = sekDir + "";

                    QDir().mkdir(QString::fromStdString(profiles[i].profileFolder));

                    unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"" + sekDir + "\\"+profiles[i].name +"\\\" \"%cd%\\profiles\\"+ profiles[i].name + ".zip\" ");

                }


            }

        }
        else{

            //updates mod entries to uninstalled
            for(int i = 0; i < mods.size(); i++){

                if(mods[i].isInstalled == "y"){

                    //edits config
                    ifstream filein(mods[i].modConfigPath); //orig file
                    ofstream fileout("TMP.ini"); //Temp file



                    //take entire line as a string
                    short counter = 0;
                    for(string line; getline( filein, line ); )
                    {

                        if(counter == 0){

                            line = "n";


                        }

                        counter++;


                       fileout << line << endl;
                    }



                    //closes file streams
                    filein.close();
                    fileout.close();


                    //switches orig config with final/edited one
                    QFile::remove(QString::fromStdString(mods[i].modConfigPath));
                    QFile::rename(".\\TMP.ini", QString::fromStdString(mods[i].modConfigPath));


                    //removes icon from combobox
                    ui->modsInstalled->setItemIcon(i, QIcon(":/uielements/uielements/sekiroFakeIcon.png"));



                    //updates struct
                    mods[i].isInstalled = "n";

                }


                }


            //upddates profile entries to uninstalled
            for(int i = 0; i < profiles.size(); i++){

                if(profiles[i].isInstalledP == "y"){

                    profiles[i].isInstalledP = "n";

                    //edits profile config
                    ifstream filein(profiles[i].profileConfigPath); //orig file
                    ofstream fileout("TMP.ini"); //Temp file



                    //take entire line as a string
                    short counter = 0;
                    for(string line; getline( filein, line ); )
                    {

                        if(counter == 0){

                            line = "n";


                        }

                        counter++;


                       fileout << line << endl;
                    }



                    //closes file streams
                    filein.close();
                    fileout.close();


                    //switches orig config with final/edited one
                    QFile::remove(QString::fromStdString(profiles[i].profileConfigPath));
                    QFile::rename(".\\TMP.ini", QString::fromStdString(profiles[i].profileConfigPath));




                    //removes icon from combobox
                    ui->profilesInstalled->setItemIcon(i, QIcon(":/uielements/uielements/sekiroFakeIcon.png"));



                }


                }

        }

        getActiveProfile();

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


    if(isProfileDone == true && passed == true){

        log("Added Profile: " + profiles[ui->profilesInstalled->count()].name);

   ui->profilesInstalled->addItem(QString::fromStdString(profiles[ui->profilesInstalled->count()].name));


}

}









//installs profile to the sekiro directory

void Sekiro::on_installProfile_clicked()
{

    //checks if no profiles are installed, if there are no profiles installed, then it throws an error

    if(ui->profilesInstalled->count() <= 0){

        error(1);

        log("Error: No profile selected");
    }

    //if there is a profile installed at the current index then unpacks the profile files into the sekiro directory
    else{



    profiles[ui->profilesInstalled->currentIndex()].isInstalledP = "y";


    //unpacks the profile files into the sekiro directory

    string profileInstallFolder = sekDir + "";

    QDir().mkdir(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileFolder));

    unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"" + sekDir + "\\"+profiles[ui->profilesInstalled->currentIndex()].name +"\\\" \"%cd%\\profiles\\"+ profiles[ui->profilesInstalled->currentIndex()].name + ".zip\" ");




    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();


    //sets active profile label
    ui->activeProfile->setText(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].name));



    //moves profiles modengine.ini to sekiro directory

    string profileModengine  = ".\\profiles\\" + profiles[ui->profilesInstalled->currentIndex()].name + ".ini";

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));

    log("Installed Profile: " + profiles[ui->profilesInstalled->currentIndex()].name);




    //edits config
    ifstream filein(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath); //orig file
    ofstream fileout("TMP.ini"); //Temp file



    //take entire line as a string
    short counter = 0;
    for(string line; getline( filein, line ); )
    {

        if(counter == 0){

            line = "y";


        }

        counter++;


       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));
    QFile::rename(".\\TMP.ini", QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));




    //add icon to combobox
    ui->profilesInstalled->setItemIcon(ui->profilesInstalled->currentIndex(),QIcon(":/uielements/uielements/sekiroCheck.PNG"));


    //reapplies misc settings
    applySettings();


}


}









//uninstalls profile from the sekiro directory

void Sekiro::on_uninstallProfile_clicked()
{

    //checks if no profiles are installed, if there are no profiles installed, then it throws an error
    if(ui->profilesInstalled->count() <= 0){

        error(1);

        log("Error: No profile selected");

    }


    //if there is a profile at the current index, then uninstalls it
    else{



    for(int i = 0; i < profiles[ui->profilesInstalled->currentIndex()].files.size(); i++){


        //goes through the files vector and gets the name for the file to be deleted, then deletes it

        string del = sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name + "/" + profiles[ui->profilesInstalled->currentIndex()].files[i];

        qDebug() << QString::fromStdString(del);

        QFile toBeDeleted(QString::fromStdString(del));

        toBeDeleted.remove();





    }






    //deletes the profile folder in the sekiro directory

    QDir dir(QString::fromStdString(sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name));
    dir.removeRecursively();



    //if profile was active then sets the modengine in sekiro folder to default
    if(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].name) == ui->activeProfile->text()){

    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    //copys the default modengine.ini to the sekiro directory

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));


    //updates label
    ui->activeProfile->setText("mods (default)");


    //reapplies misc settings
    applySettings();

}

    profiles[ui->profilesInstalled->currentIndex()].isInstalledP = "n";


    log("Uninstalled Profile: " + profiles[ui->profilesInstalled->currentIndex()].name);




    //edits profile config
    ifstream filein(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath); //orig file
    ofstream fileout("TMP.ini"); //Temp file



    //take entire line as a string
    short counter = 0;
    for(string line; getline( filein, line ); )
    {

        if(counter == 0){

            line = "n";


        }

        counter++;


       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));
    QFile::rename(".\\TMP.ini", QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));




    //removes icon from combobox
    ui->profilesInstalled->setItemIcon(ui->profilesInstalled->currentIndex(), QIcon(":/uielements/uielements/sekiroFakeIcon.png"));


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
        string fileAmountS;
        string tmpNum;

        ifstream config(".\\configsP\\" + filename.toStdString());

        getline(config, Profile.isInstalledP);
        getline(config, Profile.name);
        getline(config, Profile.path);
        getline(config, Profile.profileConfigPath);
        getline(config, Profile.profileFolder);
        getline(config, tmpNum);
        getline(config, fileAmountS);


        int fileAmount = atoi(fileAmountS.c_str());




        for(int j = 0; j <= fileAmount; j++){

            string tmp;
            getline(config, tmp);

           Profile.files.push_back(tmp);



        }

        Profile.modNum = stoi(tmpNum);


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






        //creates profile entry in vector of profiles

        profiles.push_back(Profile);



        //resets profile structure
        Profile.name = "";
        Profile.path = "";
        Profile.profileConfigPath = "";
        Profile.profileFolder = "";
        Profile.files.clear();



        //deletes tmp folder

        QDir dir(".\\tmp\\");
        dir.removeRecursively();






        //creates new mod entry in combo box

        ui->profilesInstalled->addItem(QString::fromStdString(profiles[i].name));


        //sets icon
        if(profiles[i].isInstalledP == "y"){

            //add icon to combobox
            ui->profilesInstalled->setItemIcon(i,QIcon(":/uielements/uielements/sekiroCheck.PNG"));


        }
        else{

            //removes icon from combobox
            ui->profilesInstalled->setItemIcon(i, QIcon(":/uielements/uielements/sekiroFakeIcon.png"));

        }


        i++;









    }




}



















//removes profile from the mod manager

void Sekiro::on_removeProfile_clicked()
{


    //checks if no profiles are installed, if there are no profiles installed, then it throws an error
    if(ui->profilesInstalled->count() <= 0){

        error(1);


        log("Error: No profile selected");
    }



    //if there is a profile in profilesInstalled, it uninstalls it
    else{


    //uninstalls profile from sekiro directory
        for(int i = 0; i < profiles[ui->profilesInstalled->currentIndex()].files.size(); i++){


            //goes through the files vector and gets the name for the file to be deleted, then deletes it

            string del = sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name + "/" + profiles[ui->profilesInstalled->currentIndex()].files[i];

            qDebug() << QString::fromStdString(del);

            QFile toBeDeleted(QString::fromStdString(del));

            toBeDeleted.remove();





        }






        //deletes the profile folder in the sekiro directory

        QDir dir(QString::fromStdString(sekDir + "/" + profiles[ui->profilesInstalled->currentIndex()].name));
        dir.removeRecursively();




    //removes profile config file

    QFile configDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].profileConfigPath));

    configDel.remove();



    //removes profile archive file

    QFile modDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].path + ".zip"));

    modDel.remove();




    //removes profile modengine.ini

    QFile modEngineDel(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].path + ".ini"));

    modEngineDel.remove();


    log("Profile Removed: " + profiles[ui->profilesInstalled->currentIndex()].name);







    //if profile was active then sets the modengine in sekiro folder to default
    if(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].name) == ui->activeProfile->text()){


        //removes modengine ini

        string modengineRemove = sekDir + ".\\modengine.ini";

        QFile file(QString::fromStdString(modengineRemove));

        file.remove();



        //copys the default modengine.ini to the sekiro directory

        string finalProfileModengine = sekDir + "\\modengine.ini";

        QFile::copy(".\\modengine.ini", QString::fromStdString(finalProfileModengine));


        //updates label
        ui->activeProfile->setText("mods (default)");


        //repapplies misc settings
        applySettings();

    }




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

        error(1);

        log("Error: No profile selected");
    }

    else{






    //removes modengine ini

    string modengineRemove = sekDir + ".\\modengine.ini";

    QFile file(QString::fromStdString(modengineRemove));

    file.remove();



    //sets active profile label
    ui->activeProfile->setText(QString::fromStdString(profiles[ui->profilesInstalled->currentIndex()].name));


    //copys the modengine.ini for the profile at the current index to the sekiro directory

    string profileModengine  = ".\\profiles\\" + profiles[ui->profilesInstalled->currentIndex()].name + ".ini";

    string finalProfileModengine = sekDir + "\\modengine.ini";

    QFile::copy(QString::fromStdString(profileModengine), QString::fromStdString(finalProfileModengine));

    log("Profile Active: " + profiles[ui->profilesInstalled->currentIndex()].name);





    //reapplies settings
    applySettings();

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

    log("Default Profile (mods)");



    //sets activeProfile
    ui->activeProfile->setText("mods (default)");



    //reapplies settings
    applySettings();

}









//launches sekiro

void Sekiro::on_launchSekiro_clicked()
{


    //literally just launches sekiro lol

    unpackRepack("cd /d " + sekDir + " & START sekiro.exe");

    log("Sekiro Launched");



    if(ui->closeOnLaunch->checkState()){

        close();


    }




}










//on startup, checks if modengine is installed, if not it downloads and installs it for the user

void Sekiro::modEngineCheck(){


    //checks if modengine is installed

    QFileInfo modengineInstallCheck(QString::fromStdString(sekDir + "\\modengine.ini"));

    //checks if "modengine" exists in the sekiro directory, if it doesnt, then makes asks the user if they want to install modengine
    if(!(modengineInstallCheck.exists())){




        QMessageBox r;

        r.setWindowTitle("Modengine not installed");
        r.setText(tr("Modengine is not currently installed  \nDo you want to install it?"));
        QAbstractButton* pButtonYes = r.addButton(tr("Yes"), QMessageBox::YesRole);
        r.addButton(tr("No"), QMessageBox::NoRole);


        QFont errFont("Segoe UI", 8);


        r.setFont(errFont);

        r.exec();




        //if the user clicks yes then downloads and installs modengine
        if (r.clickedButton() == pButtonYes) {


            QString url = "https://cf-files.nexusmods.com/cdn/2763/6/ModEngine-6-0-1-11-1555994013.zip?md5=pDU1_H4yZ5AVMB-Z0AEk6w&expires=1581884707&user_id=8105063&rip=74.88.98.97";
            QDesktopServices::openUrl (url);

             string downloadsFolder = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation).toLocal8Bit().constData();


            qDebug() << QString::fromStdString(downloadsFolder);

            Sleep(2000);


            QDir().mkdir(".\\tmp");

            unpackRepack("cd %cd%   &   7za e -y \""+ downloadsFolder + "//ModEngine*.zip\" -o\""+ sekDir +"\" *.txt *.ini *.dll -r");

            QDir().mkdir(QString::fromStdString(sekDir + "\\mods"));
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



    settings(2);

}





//checks if mod path is empty
 bool Sekiro::isModPathEmpty(string truemodpath){

    if(truemodpath == "" ){

        return true;
    }

    else{

        return false;
    }


}





//checks if folder has sekiro.exe
 int Sekiro::sekiroCheck(){



     QFileInfo sekiroCheck(QString::fromStdString(sekDir + "\\sekiro.exe"));

     //checks if "sekiro" exists in the sekiro directory, if it doesnt, then m
     if(!(sekiroCheck.exists())){



         QMessageBox err(QMessageBox::Critical, tr("Error"), tr("Sekiro executable not found, please choose sekiro folder."));
         err.setWindowFlags(err.windowFlags() | Qt::WindowStaysOnTopHint);


         QFont errFont("Segoe UI", 8);


         err.setFont(errFont);

         err.exec();



         ui->currentSekDir->setText("");

         QFile delDir(".\\dir.ini");
         delDir.remove();





         return 0;

     }

     else{

         return 1;

     }





 }




//displays error if no mpds/profiles selected
 void Sekiro::error(int mode){


     if(mode == 0){

     QFont sekFont("Assassin$");
     QFont errFont("Segoe UI", 8);

     QApplication::setFont(errFont);

     QMessageBox err;



    err.critical(this, "Error", "No mod selected");

    QApplication::setFont(sekFont);
     }

     else if(mode == 1){

         QFont sekFont("Assassin$");
         QFont errFont("Segoe UI", 8);

         QApplication::setFont(errFont);

         QMessageBox err;



        err.critical(this, "Error", "No profile selected");

        QApplication::setFont(sekFont);

     }


 }



//checks if entered name matches a name of a previously installed mod or profile
 int Sekiro::nameRepeatCheck(bool mode, string lineedit){


 if(mode == true){

         int counter = 0;

         for(int i = 0;i < mods.size();i++){

             if(lineedit == mods[i].name){

                 counter++;

             }



         }

         if (counter == 0){

             return 0;

         }
         else{

             return 1;

         }

     }

 else if( mode == false){


     int counter = 0;

     for(int i = 0;i < profiles.size();i++){

         if(lineedit == profiles[i].name){

             counter++;

         }



     }

     if (counter == 0){

         return 0;

     }
     else{

         return 1;

     }

 }


 }



 //logs what happens in the program
 void Sekiro::log(string log){


ui->logTextEdit->insertPlainText(QString::fromStdString(log) + "\n");

logFile << log + "\n";




}



 //sets log on or off
void Sekiro::on_logOn_stateChanged()
{

    if(ui->logOn->isChecked()){

        ui->logTextEdit->show();


    }
    else if(ui->logOn->checkState() == false){

        ui->logTextEdit->hide();
    }


    settings(1);

}





//handles settings
void Sekiro::settings(int mode){

    if(mode  == 0){


        QFileInfo settings(".\\conf.ini");


        if(!(settings.exists())){


            QFile set(".\\conf.ini");

            ofstream setNew(".\\conf.ini");
            setNew << ui->logOn->checkState() << endl << ui->warnings->checkState() << endl << ui->closeOnLaunch->checkState() << endl << ui->keepModengineSettings->checkState()<< endl << ui->reinstallAfterDirChange->checkState();
            setNew.close();


        }

        else{

            ifstream settings(".\\conf.ini");

            int i = 0;

            for(string line; getline(settings, line);){


                i++;


                if(i == 1){

                    if (line ==  "0"){

                        ui->logOn->setChecked(false );

                    }
                    else if(line == "1"){

                        ui->logOn->setChecked(true);

                    }


                }

                else if(i == 2){

                    if (line ==  "0"){

                        ui->warnings->setChecked(false );

                    }
                    else if(line == "1"){

                        ui->warnings->setChecked(true);

                    }

                }
                else if(i == 3){


                    if (line ==  "0"){

                        ui->closeOnLaunch->setChecked(false );

                    }
                    else if(line == "1"){

                        ui->closeOnLaunch->setChecked(true);

                    }


                }
                else if(i == 4){


                    if (line ==  "0"){

                        ui->keepModengineSettings->setChecked(false );

                    }
                    else if(line == "1"){

                        ui->keepModengineSettings->setChecked(true);

                    }


                }
                else if(i == 5){


                    if (line ==  "0"){

                        ui->reinstallAfterDirChange->setChecked(false );

                    }
                    else if(line == "1"){

                        ui->reinstallAfterDirChange->setChecked(true);

                    }


                }


            }


        }


    }

else if(mode == 1){


    string tempPath = ".\\ini.conf";
    string path = ".\\conf.ini";

    ofstream settingsTemp(tempPath);
    ifstream settings(path);




    int i = 0;

    for(string line; getline(settings, line);){


        i++;

        if(i == 1){


            if(ui->logOn->isChecked()){

                line = "1";
            }
            else{

                line = "0";
            }


        }


        settingsTemp << line << endl;

    }

    settings.close();
    settingsTemp.close();

    QFile::remove(QString::fromStdString(path));
    QFile::rename(QString::fromStdString(tempPath), QString::fromStdString(path));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();


    }


else if(mode == 2){


string tempPath = ".\\ini.conf";
string path = ".\\conf.ini";

ofstream settingsTemp(tempPath);
ifstream settings(path);




int i = 0;

for(string line; getline(settings, line);){


    i++;

    if(i == 2){

        if(ui->warnings->isChecked()){

            line = "1";
        }
        else{

            line = "0";
        }

    }


    settingsTemp << line << endl;

}

settings.close();
settingsTemp.close();

QFile::remove(QString::fromStdString(path));
QFile::rename(QString::fromStdString(tempPath), QString::fromStdString(path));

//deletes temp files if they arent deleted
QFile tmpDel(".\\TMP.ini");
tmpDel.remove();


QFile iniDel(".\\ini.conf");
iniDel.remove();


}


else if(mode == 3){


string tempPath = ".\\ini.conf";
string path = ".\\conf.ini";

ofstream settingsTemp(tempPath);
ifstream settings(path);




int i = 0;

for(string line; getline(settings, line);){


    i++;

    if(i == 3){

        if(ui->closeOnLaunch->isChecked()){

            line = "1";
        }
        else{

            line = "0";
        }

    }


    settingsTemp << line << endl;

}

settings.close();
settingsTemp.close();

QFile::remove(QString::fromStdString(path));
QFile::rename(QString::fromStdString(tempPath), QString::fromStdString(path));

//deletes temp files if they arent deleted
QFile tmpDel(".\\TMP.ini");
tmpDel.remove();


QFile iniDel(".\\ini.conf");
iniDel.remove();


}


else if(mode == 4){


    string tempPath = ".\\ini.conf";
    string path = ".\\conf.ini";

    ofstream settingsTemp(tempPath);
    ifstream settings(path);




    int i = 0;

    for(string line; getline(settings, line);){


        i++;

        if(i == 4){


            if(ui->keepModengineSettings->isChecked()){

                line = "1";
            }
            else{

                line = "0";
            }


        }


        settingsTemp << line << endl;

    }

    settings.close();
    settingsTemp.close();

    QFile::remove(QString::fromStdString(path));
    QFile::rename(QString::fromStdString(tempPath), QString::fromStdString(path));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();


    }


else if(mode == 5){


    string tempPath = ".\\ini.conf";
    string path = ".\\conf.ini";

    ofstream settingsTemp(tempPath);
    ifstream settings(path);




    int i = 0;

    for(string line; getline(settings, line);){


        i++;

        if(i == 5){


            if(ui->reinstallAfterDirChange->isChecked()){

                line = "1";
            }
            else{

                line = "0";
            }


        }


        settingsTemp << line << endl;

    }

    settings.close();
    settingsTemp.close();

    QFile::remove(QString::fromStdString(path));
    QFile::rename(QString::fromStdString(tempPath), QString::fromStdString(path));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();


    }




}




//close program on launch
void Sekiro::on_closeOnLaunch_stateChanged()
{

    settings(3);


}






//gets active profile
void Sekiro::getActiveProfile(){



    ifstream modEngineConfig(sekDir + "\\modengine.ini");

    string tmp;

    for(string line; getline(modEngineConfig, line);){



        if(line.find("modOverrideDirectory=\"") != string::npos){

            tmp = line.erase(0, 22);
            qDebug() << QString::fromStdString(tmp);


tmp.erase(remove(tmp.begin(), tmp.end(), '\\'), tmp.end());
tmp.erase(remove(tmp.begin(), tmp.end(), '\"'), tmp.end());



            if(tmp == "mods"){

                ui->activeProfile->setText(QString::fromStdString("mods (default)"));
            }
            else{
            ui->activeProfile->setText(QString::fromStdString(tmp));

            }
        }


    }


}




//edits modengine.ini
void Sekiro::modengineEdit(string dll, bool unchain){


    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file
    ofstream fileout("TMP.ini"); //Temp file


    //take entire line as a string

    for(string line; getline( filein, line ); )
    {

        if(line.find("chainDInput8DLLPath=") != string::npos && !(line.find(";") != string::npos)){


            if(!unchain){
            line = "chainDInput8DLLPath=\"\\" + dll + "\"";
            }
            else{
            line = "chainDInput8DLLPath=\"" + dll + "\"";
            }

        }




       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(sekDir + "\\" + "modengine.ini"));
    QFile::rename(".\\TMP.ini", QString::fromStdString(sekDir + "\\" + "modengine.ini"));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();




}




//chains and unchains dll
void Sekiro::on_chainUnchain_stateChanged()
{



    if(ui->chainUnchain->isChecked()){


        if(ui->dllNameLabel->text() == "" || ui->dllNameLabel->text() == "None"){

        //opens file dailogue asking user for dll, if dll is named dinput.dll then throw error
       QFileInfo dll = QFileDialog::getOpenFileName(this, "Choose dll file to chain");

       qDebug() << dll.absoluteFilePath();
       qDebug() << dll.fileName();

       if(dll.fileName() == "dinput8.dll"){

           QFont sekFont("Assassin$");
           QFont errFont("Segoe UI", 8);

           QMessageBox err;

           QApplication::setFont(errFont);

           err.critical(this, "Error", "The dll file to be chained cannot be named dinput.dll, please rename the file and try again");


           ui->chainUnchain->setChecked(false);


          QApplication::setFont(sekFont);

          log("Error: dll named dinput8.dll");

       }
        else{
       QFile::copy(dll.absoluteFilePath() ,QString::fromStdString(sekDir) + "\\" + dll.fileName());


       //sets dll name label
      ui->dllNameLabel->setText(dll.fileName());


        //edits modengine.ini
       modengineEdit(dll.fileName().toLocal8Bit().constData());
}

}

    }
    else{


        modengineEdit("", true);
        ui->dllNameLabel->setText("None");


    }




}



//toggles show debug files in modengine on and off
void Sekiro::on_debug_stateChanged()
{


    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file
    ofstream fileout("TMP.ini"); //Temp file


    //take entire line as a string

    for(string line; getline( filein, line ); )
    {

        if(line.find("showDebugLog=") != string::npos && !(line.find(";") != string::npos)){


            if(ui->debug->isChecked()){

            line = "showDebugLog=1";

            }
            else{

            line = "showDebugLog=0";

            }

        }




       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(sekDir + "\\" + "modengine.ini"));
    QFile::rename(".\\TMP.ini", QString::fromStdString(sekDir + "\\" + "modengine.ini"));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();


}



//if checked, skips logos for sekiro
void Sekiro::on_logo_stateChanged()
{



    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file
    ofstream fileout("TMP.ini"); //Temp file


    //take entire line as a string

    for(string line; getline( filein, line ); )
    {

        if(line.find("skipLogos=") != string::npos && !(line.find(";") != string::npos)){


            if(ui->logo->isChecked()){

            line = "skipLogos=1";

            }
            else{

            line = "skipLogos=0";

            }

        }




       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(sekDir + "\\" + "modengine.ini"));
    QFile::rename(".\\TMP.ini", QString::fromStdString(sekDir + "\\" + "modengine.ini"));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();




}



//toggles cache file paths in modengine on and off
void Sekiro::on_cache_stateChanged()
{


    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file
    ofstream fileout("TMP.ini"); //Temp file


    //take entire line as a string

    for(string line; getline( filein, line ); )
    {

        if(line.find("cacheFilePaths=") != string::npos && !(line.find(";") != string::npos)){


            if(ui->cache->isChecked()){

            line = "cacheFilePaths=1";

            }
            else{

            line = "cacheFilePaths=0";

            }

        }




       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(sekDir + "\\" + "modengine.ini"));
    QFile::rename(".\\TMP.ini", QString::fromStdString(sekDir + "\\" + "modengine.ini"));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();

}



//toggles load UXM files in modengine on and off
void Sekiro::on_uxm_stateChanged()
{


    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file
    ofstream fileout("TMP.ini"); //Temp file


    //take entire line as a string

    for(string line; getline( filein, line ); )
    {

        if(line.find("loadUXMFiles=") != string::npos && !(line.find(";") != string::npos)){


            if(ui->uxm->isChecked()){

            line = "loadUXMFiles=1";

            }
            else{

            line = "loadUXMFiles=0";

            }

        }




       fileout << line << endl;
    }



    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(sekDir + "\\" + "modengine.ini"));
    QFile::rename(".\\TMP.ini", QString::fromStdString(sekDir + "\\" + "modengine.ini"));


    //deletes temp files if they arent deleted
    QFile tmpDel(".\\TMP.ini");
    tmpDel.remove();


    QFile iniDel(".\\ini.conf");
    iniDel.remove();

}




//parses settings from modengine.ini
void Sekiro::parseSettings(){


    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file



    //take entire line as a string

    for(string line; getline( filein, line ); )
    {

        //uxm
        if(line.find("loadUXMFiles=") != string::npos && !(line.find(";") != string::npos)){


            if(line.find("loadUXMFiles=") != string::npos && (line.find("1") != string::npos)){

            ui->uxm->setChecked(true);

            }
            else if (line.find("loadUXMFiles=") != string::npos && (line.find("0") != string::npos)){

            ui->uxm->setChecked(false);

            }

        }


        //logo
        if(line.find("skipLogos=") != string::npos && !(line.find(";") != string::npos)){


            if(line.find("skipLogos=") != string::npos && (line.find("1") != string::npos)){

            ui->logo->setChecked(true);

            }
            else if (line.find("skipLogos=") != string::npos && (line.find("0") != string::npos)){

            ui->logo->setChecked(false);

            }

        }



        //debug
        if(line.find("showDebugLog=") != string::npos && !(line.find(";") != string::npos)){


            if(line.find("showDebugLog=") != string::npos && (line.find("1") != string::npos)){

            ui->debug->setChecked(true);

            }
            else if (line.find("showDebugLog=") != string::npos && (line.find("0") != string::npos)){

            ui->debug->setChecked(false);

            }

        }





        //chain
        if(line.find("chainDInput8DLLPath=") != string::npos && !(line.find(";") != string::npos)){


            if(line.find("chainDInput8DLLPath=\"\"") != string::npos && !(line.find(";") != string::npos)){

            ui->chainUnchain->setChecked(false);
            ui->dllNameLabel->setText("None");

            }
            else{


                    //parses dll name from line
                    string toErase = "chainDInput8DLLPath=\"";
                    size_t pos = line.find(toErase);
                    string dllNameFromIni;

                    if (pos != std::string::npos)
                    {
                        // If found then erase it from string
                       dllNameFromIni = line.erase(pos, toErase.length());
                    }


                    toErase = "\\";
                    pos = dllNameFromIni.find(toErase);
                    if (pos != std::string::npos)
                    {
                        // If found then erase it from string
                       dllNameFromIni = dllNameFromIni.erase(pos, toErase.length());
                    }


                    toErase = "\"";
                    pos = dllNameFromIni.find(toErase);
                    if (pos != std::string::npos)
                    {
                        // If found then erase it from string
                       dllNameFromIni = dllNameFromIni.erase(pos, toErase.length());
                    }






                    ui->dllNameLabel->setText(QString::fromStdString(dllNameFromIni));
                    ui->chainUnchain->setChecked(true);

            }

        }



        //cache
        if(line.find("cacheFilePaths=") != string::npos && !(line.find(";") != string::npos)){


            if(line.find("cacheFilePaths=") != string::npos && (line.find("1") != string::npos)){

            ui->cache->setChecked(true);

            }
            else if (line.find("cacheFilePaths=") != string::npos && (line.find("0") != string::npos)){

            ui->cache->setChecked(false);

            }

        }




    }



    //closes file streams
    filein.close();




}



//applies settings to moddenigne.ini
void Sekiro::applySettings(){


    //edits config
    ifstream filein(sekDir + "\\" + "modengine.ini"); //orig file
    ofstream fileout("TMP.ini"); //Temp file



    for(string line; getline( filein, line ); )
    {


    //logo
    if(line.find("skipLogos=") != string::npos && !(line.find(";") != string::npos)){


                if(ui->logo->isChecked()){

                line = "skipLogos=1";

                }
                else{

                line = "skipLogos=0";

                }

            }




    //debug
    if(line.find("showDebugLog=") != string::npos && !(line.find(";") != string::npos)){


                if(ui->debug->isChecked()){

                line = "showDebugLog=1";

                }
                else{

                line = "showDebugLog=0";

                }

            }




    //chain
    if(line.find("chainDInput8DLLPath=") != string::npos && !(line.find(";") != string::npos)){


        if(ui->chainUnchain->isChecked()){


            line = ui->dllNameLabel->text().toLocal8Bit().constData();

            line = "chainDInput8DLLPath=\"" + line + "\"";


        }
        else{


            line = "chainDInput8DLLPath=\"\"";


        }

            }




    //cache
    if(line.find("cacheFilePaths=") != string::npos && !(line.find(";") != string::npos)){


            if(ui->cache->isChecked()){

            line = "cacheFilePaths=1";

            }
            else{

            line = "cacheFilePaths=0";

            }

        }

    //uxm
    if(line.find("loadUXMFiles=") != string::npos && !(line.find(";") != string::npos)){


            if(ui->uxm->isChecked()){

            line = "loadUXMFiles=1";

            }
            else{

            line = "loadUXMFiles=0";

            }

        }



    fileout << line << endl;
    }

    //closes file streams
    filein.close();
    fileout.close();


    //switches orig config with final/edited one
    QFile::remove(QString::fromStdString(sekDir + "\\" + "modengine.ini"));
    QFile::rename(".\\TMP.ini", QString::fromStdString(sekDir + "\\" + "modengine.ini"));

}



//if checked, reinstalls mods after directory change
void Sekiro::on_reinstallAfterDirChange_stateChanged()
{

    settings(5);

}




//if checked, reapplies modengine settings after directory
void Sekiro::on_keepModengineSettings_stateChanged()
{

    settings(4);

}
