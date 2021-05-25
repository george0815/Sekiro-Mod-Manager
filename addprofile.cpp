#include "addprofile.h"
#include "sekiro.h"
#include "ui_addprofile.h"
#include <QFileDialog>
#include <iostream>
#include <QMessageBox>
#include <fstream>
#include <qdebug.h>


extern struct profile Profile;

string modProfilePath;

bool passed;

extern bool isProfileDone;

addProfile::addProfile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addProfile)
{
    ui->setupUi(this);


    if(res == 0){

        //SIZES
        setFixedSize(756, 259);
        ui->profileLabel->resize(351, 21);
        ui->profileNameLineEdit->resize(571, 51);
        ui->profileNumberLabel->resize(581, 21);
        ui->profileModNumberSpinBox->resize(61, 41);
        ui->profileConfirm->resize(151, 101);
        ui->cancel->resize(211, 111);


        //POSITIONS
        ui->profileLabel->move(190, 15);
        ui->profileNameLineEdit->move(90, 40);
        ui->profileNumberLabel->move(50, 130);
        ui->profileModNumberSpinBox->move(640, 120);
        ui->profileConfirm->move(100, 170);
        ui->cancel->move(430, 170);


        //STYLESHEETS
        ui->profileLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 26pt \"Assassin$\";");
        ui->profileNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 26pt \"Assassin$\";");
        ui->profileNumberLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 26pt \"Assassin$\";");
        ui->profileModNumberSpinBox->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 26pt \"Assassin$\";");
        ui->profileConfirm->setStyleSheet("#profileConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#profileConfirm:hover{\n	background-image: url(:/uielements/uielements/ok.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
        ui->cancel->setStyleSheet("#cancel{\nbackground-image: url(:/uielements/uielements/cancelNoPaint.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#cancel:hover{\n	background-image: url(:/uielements/uielements/cancel.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");


    }
    else if(res == 1){


        //SIZES
        setFixedSize(756 * 1.5, 259 * 1.5);
        ui->profileLabel->resize(351 * 1.5, 21 * 1.5);
        ui->profileNameLineEdit->resize(571 * 1.5, 51 * 1.5);
        ui->profileNumberLabel->resize(581 * 1.5, 21 * 1.5);
        ui->profileModNumberSpinBox->resize(61 * 1.5, 41 * 1.5);
        ui->profileConfirm->resize(151 * 1.5, 101 * 1.5);
        ui->cancel->resize(211 * 1.5, 111 * 1.5);


        //POSITIONS
        ui->profileLabel->move(190 * 1.5, 15 * 1.5);
        ui->profileNameLineEdit->move(90 * 1.5, 40 * 1.5);
        ui->profileNumberLabel->move(50 * 1.5, 130 * 1.5);
        ui->profileModNumberSpinBox->move(640 * 1.5, 120 * 1.5);
        ui->profileConfirm->move(100 * 1.5, 170 * 1.5);
        ui->cancel->move(430 * 1.5, 170 * 1.5);


        //STYLESHEETS
        ui->profileLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 39pt \"Assassin$\";");
        ui->profileNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 39pt \"Assassin$\";");
        ui->profileNumberLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 39pt \"Assassin$\";");
        ui->profileModNumberSpinBox->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 39pt \"Assassin$\";");
        ui->profileConfirm->setStyleSheet("#profileConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint1080p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#profileConfirm:hover{\n	background-image: url(:/uielements/uielements/ok1080p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
        ui->cancel->setStyleSheet("#cancel{\nbackground-image: url(:/uielements/uielements/cancelNoPaint1080p.png) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#cancel:hover{\n	background-image: url(:/uielements/uielements/cancel1080p.png) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");



    }
    else if(res == 2){


        //SIZES
        setFixedSize(756 * 2, 259 * 2);
        ui->profileLabel->resize(351 * 2, 21 * 2);
        ui->profileNameLineEdit->resize(571 * 2, 51 * 2);
        ui->profileNumberLabel->resize(581 * 2, 21 * 2);
        ui->profileModNumberSpinBox->resize(61 * 2, 41 * 2);
        ui->profileConfirm->resize(151 * 2, 101 * 2);
        ui->cancel->resize(211 * 2, 111 * 2);


        //POSITIONS
        ui->profileLabel->move(190 * 2, 15 * 2);
        ui->profileNameLineEdit->move(90 * 2, 40 * 2);
        ui->profileNumberLabel->move(50 * 2, 130 * 2);
        ui->profileModNumberSpinBox->move(640 * 2, 120 * 2);
        ui->profileConfirm->move(100 * 2, 170 * 2);
        ui->cancel->move(430 * 2, 170 * 2);


        //STYLESHEETS
        ui->profileLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 52pt \"Assassin$\";");
        ui->profileNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 52pt \"Assassin$\";");
        ui->profileNumberLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 52pt \"Assassin$\";");
        ui->profileModNumberSpinBox->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 52pt \"Assassin$\";");
        ui->profileConfirm->setStyleSheet("#profileConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint1440p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#profileConfirm:hover{\n	background-image: url(:/uielements/uielements/ok1440p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
        ui->cancel->setStyleSheet("#cancel{\nbackground-image: url(:/uielements/uielements/cancelNoPaint1440p.png) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#cancel:hover{\n	background-image: url(:/uielements/uielements/cancel1440p.png) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");




    }
    else if(res == 3){



        //SIZES
        setFixedSize(756 * 3, 259 * 3);
        ui->profileLabel->resize(351 * 3, 21 * 3);
        ui->profileNameLineEdit->resize(571 * 3, 51 * 3);
        ui->profileNumberLabel->resize(581 * 3, 21 * 3);
        ui->profileModNumberSpinBox->resize(61 * 3, 41 * 3);
        ui->profileConfirm->resize(151 * 3, 101 * 3);
        ui->cancel->resize(211 * 3, 111 * 3);


        //POSITIONS
        ui->profileLabel->move(190 * 3, 15 * 3);
        ui->profileNameLineEdit->move(90 * 3, 40 * 3);
        ui->profileNumberLabel->move(50 * 3, 130 * 3);
        ui->profileModNumberSpinBox->move(640 * 3, 120 * 3);
        ui->profileConfirm->move(100 * 3, 170 * 3);
        ui->cancel->move(430 * 3, 170 * 3);


        //STYLESHEETS
        ui->profileLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 78pt \"Assassin$\";");
        ui->profileNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 78pt \"Assassin$\";");
        ui->profileNumberLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 78pt \"Assassin$\";");
        ui->profileModNumberSpinBox->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 78pt \"Assassin$\";");
        ui->profileConfirm->setStyleSheet("#profileConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint2160p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#profileConfirm:hover{\n	background-image: url(:/uielements/uielements/ok2160p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
        ui->cancel->setStyleSheet("#cancel{\nbackground-image: url(:/uielements/uielements/cancelNoPaint2160p.png) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#cancel:hover{\n	background-image: url(:/uielements/uielements/cancel2160p.png) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");



    }

    setWindowFlags(Qt::WindowTitleHint);



    //sets background of the mod name window
    setStyleSheet("addProfile {background-color: rgb(0, 0, 0);}");


    //sets background of the main window
    setStyleSheet("addProfile {border: 1px solid white}");


    //sets regular expression so that only alphanumeric characters and dash/space can be used, thihs iss because if a usseer uses a slash within the name it fucks everything up
    QRegExpValidator* validator = new QRegExpValidator( QRegExp( "[A-Za-z0-9-\\s]+" ) );
    ui->profileNameLineEdit->setValidator(validator);






}

addProfile::~addProfile()
{
    delete ui;
}


//adds profile to manager
void addProfile::on_profileConfirm_clicked()
{


    int invalidModCounter = 0;

    string nameCheck = ui->profileNameLineEdit->text().toLocal8Bit().constData();

    if(nameCheck.find_first_not_of(' ') != std::string::npos && Sekiro::nameRepeatCheck(false, ui->profileNameLineEdit->text().toLocal8Bit().constData()) == 0){







//adds the data for the profile being added to the profile struct

Profile.name = ui->profileNameLineEdit->text().toLocal8Bit().constData();

Profile.modNum = ui->profileModNumberSpinBox->value();

Profile.profileConfigPath = ".\\configsP\\" + Profile.name + ".ini";

Profile.path = ".\\profiles\\" + Profile.name;

Profile.profileFolder = sekDir + "/" + Profile.name;








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
   //for later extracting

   QFile file(modNAME);

   string modDir = ".\\profiles\\" + to_string(i) + "." + modExt.toLocal8Bit().constData();




   QFile::copy(modNAME, QString::fromStdString(modDir));


    QDir().mkdir(".\\tmp");

    if(modExt == "7z" || modExt == "zip"){

        Sekiro::unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\profiles\\"  + to_string(i) + "." + modExt.toLocal8Bit().constData() + "\"");


    }
    else if(modExt == "rar"){

        Sekiro::unpackRepack("cd %cd%    &    unrar x  -y \"%cd%\\profiles\\"  + to_string(i) + ".rar\" * .\\tmp\\");

    }




    Sekiro::traverse("*.*", ".\\tmp\\", 0, false);



    if (Sekiro::isModPathEmpty(modProfilePath) == false){

    Sekiro::unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\"  + to_string(i) + ".zip\" \"" + modProfilePath + "/*\"");

    QFile fileDel(modDir.c_str());

    fileDel.remove();



    //moves restructured mod files to profiles folder

    QFile::copy(".\\tmp\\" + QString::fromStdString(to_string(i)) + ".zip", ".\\profiles\\" + QString::fromStdString(to_string(i)) + ".zip");



    //deletes tmp

    QDir dir(".\\tmp\\");
    dir.removeRecursively();

    QDir().mkdir(".\\tmp\\");


    modProfilePath = "";

    passed = true;

    }

    else if(Sekiro::isModPathEmpty(modProfilePath) == true){

        isModValid = false;

        Sekiro::traverse("*.*", ".\\tmp\\", 2);

        if (isModValid){


            Sekiro::unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\tmp1\\"  + to_string(i) + ".zip\" \".\\tmp\\tmp1\\*\"");

            QFile fileDel(modDir.c_str());

            fileDel.remove();



            //moves restructured mod files to profiles folder

            QFile::copy(".\\tmp\\tmp1\\" + QString::fromStdString(to_string(i)) + ".zip", ".\\profiles\\" + QString::fromStdString(to_string(i)) + ".zip");



            //deletes tmp

            QDir dir(".\\tmp\\");
            dir.removeRecursively();

            QDir().mkdir(".\\tmp\\");


            modProfilePath = "";

            passed = true;



        }


        else{


            invalidModCounter++;

            QFont sekFont("Assassin$");

            if(res == 0){
            QFont errFont("Segoe UI", 8);
            QApplication::setFont(errFont);
            }
            else if(res == 1){
                QFont errFont("Segoe UI", 12);
                QApplication::setFont(errFont);
            }
            else if(res == 2){
                QFont errFont("Segoe UI", 16);
                QApplication::setFont(errFont);
            }
            else if(res == 3){
                QFont errFont("Segoe UI", 24);
                QApplication::setFont(errFont);
            }


            QMessageBox err;


            err.critical(this, "Error", "No mod files found");

            QFile fileDel(modDir.c_str());

            fileDel.remove();


            QDir dir(".\\tmp\\");
            dir.removeRecursively();

            string delConfig = ".\\profiles\\"+ Profile.name + ".ini";


            if(invalidModCounter == Profile.modNum){

            QFile delModConfig(delConfig.c_str());
            delModConfig.remove();

            passed = false;

        }

           QApplication::setFont(sekFont);

        }










    }


    }

}



if (passed == true){

//dump mod files in tmp

for(int i = 0; i < Profile.modNum; i++){

    Sekiro::unpackRepack("cd \"%cd%\"   &   7za e -spf -y -o\"%cd%\\tmp\" \"%cd%\\profiles\\"  + to_string(i) + ".zip""\"");

}

Sekiro::traverse("*.*", ".\\tmp\\", 1, false);



//creates profile archive

Sekiro::unpackRepack("cd \"%cd%\"   &   7za a -y \".\\tmp\\"  + Profile.name + ".zip\" .\\tmp\\*");







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


Profile.isInstalledP = "n";

profileConfig << Profile.isInstalledP + "\n" + Profile.name + "\n" + Profile.path + "\n" + Profile.profileConfigPath + "\n" + Profile.profileFolder + "\n" + to_string(Profile.modNum) + "\n";

profileConfig << Profile.files.size() << "\n";


for(int i = 0; i < Profile.files.size(); i++){


    profileConfig << Profile.files[i] << endl;



}


profileConfig.close();

profiles.push_back(Profile);


//resets profile structure
Profile.name = "";
Profile.path = "";
Profile.profileConfigPath = "";
Profile.profileFolder = "";
Profile.files.clear();

isProfileDone = true;

    }

else{

    close();

}

}






    else if (!(nameCheck.find_first_not_of(' ') != std::string::npos)){


        QFont sekFont("Assassin$");

        if(res == 0){
        QFont errFont("Segoe UI", 8);
        QApplication::setFont(errFont);
        }
        else if(res == 1){
            QFont errFont("Segoe UI", 12);
            QApplication::setFont(errFont);
        }
        else if(res == 2){
            QFont errFont("Segoe UI", 16);
            QApplication::setFont(errFont);
        }
        else if(res == 3){
            QFont errFont("Segoe UI", 24);
            QApplication::setFont(errFont);
        }


        QMessageBox err;



       err.critical(this, "Error", "No name was entered");

       QApplication::setFont(sekFont);


    }
    else if(Sekiro::nameRepeatCheck(false, ui->profileNameLineEdit->text().toLocal8Bit().constData()) == 1){

        QFont sekFont("Assassin$");
        if(res == 0){
        QFont errFont("Segoe UI", 8);
        QApplication::setFont(errFont);
        }
        else if(res == 1){
            QFont errFont("Segoe UI", 12);
            QApplication::setFont(errFont);
        }
        else if(res == 2){
            QFont errFont("Segoe UI", 16);
            QApplication::setFont(errFont);
        }
        else if(res == 3){
            QFont errFont("Segoe UI", 24);
            QApplication::setFont(errFont);
        }


        QMessageBox err;



       err.critical(this, "Error", "Name already matches a previously installed profile");

       QApplication::setFont(sekFont);

    }




}


//cancels adding a profile
void addProfile::on_cancel_clicked()
{

    isProfileDone = false;
    close();

}







