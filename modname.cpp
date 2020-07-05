#include "modname.h"
#include "ui_modname.h"
#include "sekiro.h"
#include <QMessageBox>


modname::modname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modname)
{
    ui->setupUi(this);


    if(res == 0){

        //SIZES
       setFixedSize(624, 232);
       ui->modNameLabel->resize(491, 71);
       ui->modNameLineEdit->resize(571, 51);
       ui->modNameConfirm->resize(151, 101);
       ui->isModPack->resize(81, 18);


        //POSITIONS
       ui->modNameLabel->move(80, 10);
       ui->modNameLineEdit->move(26, 70);
       ui->modNameConfirm->move(230, 130);
       ui->isModPack->move(30, 200);


        //STYLESHEETS
       ui->modNameLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 26pt \"Assassin$\";");
       ui->modNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 26pt \"Assassin$\";");
       ui->modNameConfirm->setStyleSheet("#modNameConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#modNameConfirm:hover{\n	background-image: url(:/uielements/uielements/ok.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
       ui->isModPack->setStyleSheet("#isModPack{\ncolor: rgb(255, 255, 255);\nfont: 8pt \"MS Shell Dlg 2\";\n}\n\nQToolTip { color: #ffffff; background-color: #000000; border: 0px; }\nQCheckBox::indicator {\n width: 13px;\nheight: 13px;\n }\n\n  QCheckBox::indicator:checked\n  {\n   \n	image: url(:/uielements/uielements/checked.png);\n  }\n  QCheckBox::indicator:unchecked\n  {\n    image: url(:/uielements/uielements/unChecked.png);\n  }\n\n");


    }
    else if(res == 1){


        //SIZES
       setFixedSize(624 * 1.5, 232 * 1.5);
       ui->modNameLabel->resize(491 * 1.5, 71 * 1.5);
       ui->modNameLineEdit->resize(571 * 1.5, 51 * 1.5);
       ui->modNameConfirm->resize(151 * 1.5, 101 * 1.5);
       ui->isModPack->resize(81 * 1.5, 18 * 1.5);


        //POSITIONS
       ui->modNameLabel->move(80 * 1.5, 10 * 1.5);
       ui->modNameLineEdit->move(26 * 1.5, 70 * 1.5);
       ui->modNameConfirm->move(230 * 1.5, 130 * 1.5);
       ui->isModPack->move(30 * 1.5, 200 * 1.5);


        //STYLESHEETS
       ui->modNameLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 39pt \"Assassin$\";");
       ui->modNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 39pt \"Assassin$\";");
       ui->modNameConfirm->setStyleSheet("#modNameConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint1080p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#modNameConfirm:hover{\n	background-image: url(:/uielements/uielements/ok1080p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
       ui->isModPack->setStyleSheet("#isModPack{\ncolor: rgb(255, 255, 255);\nfont: 12pt \"MS Shell Dlg 2\";\n}\n\nQToolTip { color: #ffffff; background-color: #000000; border: 0px; }\nQCheckBox::indicator {\n width: 19px;\nheight: 19px;\n }\n\n  QCheckBox::indicator:checked\n  {\n   \n	image: url(:/uielements/uielements/checked.png);\n  }\n  QCheckBox::indicator:unchecked\n  {\n    image: url(:/uielements/uielements/unChecked.png);\n  }\n\n");




    }
    else if(res == 2){


        //SIZES
       setFixedSize(624 * 2, 232 * 2);
       ui->modNameLabel->resize(491 * 2, 71 * 2);
       ui->modNameLineEdit->resize(571 * 2, 51 * 2);
       ui->modNameConfirm->resize(151 * 2, 101 * 2);
       ui->isModPack->resize(81 * 2, 18 * 2);


        //POSITIONS
       ui->modNameLabel->move(80 * 2, 10 * 2);
       ui->modNameLineEdit->move(26 * 2, 70 * 2);
       ui->modNameConfirm->move(230 * 2, 130 * 2);
       ui->isModPack->move(30 * 2, 200 * 2);


        //STYLESHEETS
       ui->modNameLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 52pt \"Assassin$\";");
       ui->modNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 52pt \"Assassin$\";");
       ui->modNameConfirm->setStyleSheet("#modNameConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint1440p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#modNameConfirm:hover{\n	background-image: url(:/uielements/uielements/ok1440p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
       ui->isModPack->setStyleSheet("#isModPack{\ncolor: rgb(255, 255, 255);\nfont: 16pt \"MS Shell Dlg 2\";\n}\n\nQToolTip { color: #ffffff; background-color: #000000; border: 0px; }\nQCheckBox::indicator {\n width: 26px;\nheight: 26px;\n }\n\n  QCheckBox::indicator:checked\n  {\n   \n	image: url(:/uielements/uielements/checked.png);\n  }\n  QCheckBox::indicator:unchecked\n  {\n    image: url(:/uielements/uielements/unChecked.png);\n  }\n\n");




    }
    else if(res == 3){



        //SIZES
       setFixedSize(624 * 3, 232 * 3);
       ui->modNameLabel->resize(491 * 3, 71 * 3);
       ui->modNameLineEdit->resize(571 * 3, 51 * 3);
       ui->modNameConfirm->resize(151 * 3, 101 * 3);
       ui->isModPack->resize(81 * 3, 18 * 3);


        //POSITIONS
       ui->modNameLabel->move(80 * 3, 10 * 3);
       ui->modNameLineEdit->move(26 * 3, 70 * 3);
       ui->modNameConfirm->move(230 * 3, 130 * 3);
       ui->isModPack->move(30 * 3, 200 * 3);


        //STYLESHEETS
       ui->modNameLabel->setStyleSheet("color: rgb(255, 255, 255);\nfont: 78pt \"Assassin$\";");
       ui->modNameLineEdit->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nfont: 78pt \"Assassin$\";");
       ui->modNameConfirm->setStyleSheet("#modNameConfirm{\nbackground-image: url(:/uielements/uielements/okNoPaint2160p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#modNameConfirm:hover{\n	background-image: url(:/uielements/uielements/ok2160p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");
       ui->isModPack->setStyleSheet("#isModPack{\ncolor: rgb(255, 255, 255);\nfont: 24pt \"MS Shell Dlg 2\";\n}\n\nQToolTip { color: #ffffff; background-color: #000000; border: 0px; }\nQCheckBox::indicator {\n width: 39px;\nheight: 39px;\n }\n\n  QCheckBox::indicator:checked\n  {\n   \n	image: url(:/uielements/uielements/checked.png);\n  }\n  QCheckBox::indicator:unchecked\n  {\n    image: url(:/uielements/uielements/unChecked.png);\n  }\n\n");



    }



    //if the user is already installing a modpack, hide modpack checkbox
    if(modpackBool){

        ui->isModPack->setVisible(false);
        ui->isModPack->setEnabled(false);



    }
    else{

        ui->isModPack->setVisible(true);
        ui->isModPack->setEnabled(true);

    }



setWindowFlags(Qt::WindowTitleHint);
//sets background of the mod name window
setStyleSheet("modname {background-color: rgb(0, 0, 0);}");


//sets background of the main window
setStyleSheet("modname {border: 1px solid white}");




}

modname::~modname()
{
    delete ui;
}




//asks the user for the name of the mod and puts it into modName
void modname::on_modNameConfirm_clicked()
{



    //asks the user for the name of the mod and puts it into modName
    modName = ui->modNameLineEdit->text().toLocal8Bit().constData();

   string repeatNameTest = ui->modNameLineEdit->text().toLocal8Bit().constData();
   repeatNameTest = repeatNameTest + "(" + modpackName + ")";
    if(modName.find_first_not_of(' ') != std::string::npos && Sekiro::nameRepeatCheck(true, ui->modNameLineEdit->text().toLocal8Bit().constData()) == 0 && Sekiro::nameRepeatCheck(true, repeatNameTest) == 0){

    close();

    }
    else if(!(modName.find_first_not_of(' ') != std::string::npos)){

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


   else if(Sekiro::nameRepeatCheck(true, ui->modNameLineEdit->text().toLocal8Bit().constData()) == 1){


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



           err.critical(this, "Error", "Name already matches a previously installed mod");

           QApplication::setFont(sekFont);

        }
    else if(Sekiro::nameRepeatCheck(true, repeatNameTest) == 1){


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



       err.critical(this, "Error", "Name already matches a previously installed mod");

       QApplication::setFont(sekFont);



    }
    }














void modname::on_isModPack_stateChanged()
{

if(ui->isModPack->isChecked()){

    modpackBool = true;
    ui->modNameLabel->setText("Please enter the name of the modpack");

}
else if(!ui->isModPack->isChecked()){

    modpackBool = false;
    ui->modNameLabel->setText("Please enter the name of the mod");

}


}


