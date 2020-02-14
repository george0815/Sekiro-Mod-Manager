#include "sekiro.h"
#include "ui_sekiro.h"
#include <iostream>
#include <string>
#include <QDebug>
#include <QFileDialog>
#include <fstream>
#include <ostream>
#include <QDirIterator>
#include <vector>
#include <cstdlib>
#include "direct.h"
#include <QDir>
#include <modname.h>
#include <QFileInfoList>
#include <QMessageBox>

#define AND_CMD +" & "+

using namespace std;

string sekDir;

string modName;

struct mod Mod;

vector<mod> mods;

string trueModPath;


Sekiro::Sekiro(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sekiro)
{
    ui->setupUi(this);

    setFixedSize(423, 165);



    checkDir();

    getSettings();






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


void Sekiro::debugFileList(){


    for(int i = 0; i < Mod.files.size(); i++){


        cout << Mod.files[i] << endl;



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

        debugFileList();





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
