#include "addprofile.h"
#include "sekiro.h"
#include "ui_addprofile.h"
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <qdebug.h>


extern struct profile Profile;

string modProfilePath;

addProfile::addProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addProfile)
{
    ui->setupUi(this);
}

addProfile::~addProfile()
{
    delete ui;
}

void addProfile::on_profileConfirm_clicked()
{



Profile.name = ui->profileNameLineEdit->text().toLocal8Bit().constData();

Profile.modNum = ui->profileModNumberSpinBox->value();

Profile.profileConfigPath = ".\\configsP\\" + Profile.name + ".ini";

Profile.path = ".\\profiles\\" + Profile.name;


for(int i = 0; i < Profile.modNum; i++){

    //open file dialog

    //sets available filters for file extensions when choosing file
    QString filter = "All archives (*.rar *.zip *.7z) ;; RAR (*.rar)  ;;  Zip (*.zip) ;; 7z (*.7z) ";

    //Allows user to choose sekiro directory
    QString modNAME = QFileDialog::getOpenFileName(this, "Open Mod","C://",filter);

    QFileInfo ext = modNAME;
    QString modExt = ext.suffix();


qDebug() << Profile.modNum;


    //restructures mod


   QFile file(modNAME);


   string modDir = ".\\profiles\\" + to_string(i) + "." + modExt.toLocal8Bit().constData();

   file.rename(modDir.c_str());





    QDir().mkdir(".\\tmp");

    if(modExt == "7z" || modExt == "zip"){

        unpackRepackProfiles("cd %cd%   &   7za e -spf -y -o%cd%\\tmp \"%cd%\\profiles\\"  + to_string(i) + "." + modExt.toLocal8Bit().constData() + "\" & PAUSE");


    }
    else if(modExt == "rar"){

        unpackRepackProfiles("cd %cd%    &    unrar x  -y \"%cd%\\profiles\\"  + to_string(i) + ".rar\" * .\\tmp\\");

    }





    traverseProfiles("*.*", ".\\tmp\\", 0);



    unpackRepackProfiles("cd %cd%   &   7za a -y \".\\tmp\\"  + to_string(i) + ".zip\" \"" + modProfilePath + "/*\"");



}

//dump mod files in tmp

//get all files in tmp and put them in Profiles.files



}



void addProfile::unpackRepackProfiles(string line){

    ofstream unpack;
    unpack.open( "commands.cmd", ios::trunc );

    unpack <<
        line << endl;

    unpack.close();



    system( "cmd.exe /c commands.cmd" );


    remove( "commands.cmd" );

}



void addProfile::traverseProfiles(const QString &pattern, const QString &dirname, int mode)
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


                modProfilePath = fileInfo.path().toLocal8Bit().constData();
                qDebug() << modProfilePath.c_str();


            }

            traverseProfiles(pattern, fileInfo.filePath(), mode);



        }

        if(mode == 1){




        QString file = fileInfo.filePath().remove(0, 5);

        //qDebug() << file;


        if(fileInfo.isFile()){

        Profile.files.push_back(file.toLocal8Bit().constData());

        i++;

        }

      }



    }
}
