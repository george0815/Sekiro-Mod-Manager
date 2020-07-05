#include "repeatfile.h"
#include "ui_repeatfile.h"
#include "sekiro.h"
#include <QDebug>



repeatFile::repeatFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::repeatFile)
{


    ui->setupUi(this);

    if(res == 0){

        //SIZES
        setFixedSize(668, 341);
        ui->repeatFileNameNotif->resize(511, 41);
        ui->pleaseChoose->resize(671, 81);
        ui->repeatFolder->resize(461, 71);
        ui->repeatFileOk->resize(151, 101);


        //POSITIONS
        ui->repeatFileNameNotif->move(70, 30);
        ui->pleaseChoose->move(70, 70);
        ui->repeatFolder->move(100, 150);
        ui->repeatFileOk->move(250, 200);


        //STYLESHEETS
        ui->repeatFileNameNotif->setStyleSheet("color: rgb(255, 255, 255);\nfont: 26pt \"Assassin$\";");
        ui->pleaseChoose->setStyleSheet("color: rgb(255, 255, 255);\nfont: 26pt \"Assassin$\";");
        ui->repeatFolder->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nborder-color: rgb(255, 255, 255);\nfont: 26pt \"Assassin$\";");
        ui->repeatFileOk->setStyleSheet("#repeatFileOk{\nbackground-image: url(:/uielements/uielements/okNoPaint.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#repeatFileOk:hover{\n	background-image: url(:/uielements/uielements/ok.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");



    }
    else if(res == 1){

        //SIZES
        setFixedSize(668 * 1.5, 341 * 1.5);
        ui->repeatFileNameNotif->resize(511 * 1.5, 41 * 1.5);
        ui->pleaseChoose->resize(671 * 1.5, 81 * 1.5);
        ui->repeatFolder->resize(461 * 1.5, 71 * 1.5);
        ui->repeatFileOk->resize(151 * 1.5, 101 * 1.5);


        //POSITIONS
        ui->repeatFileNameNotif->move(70 * 1.5, 30 * 1.5);
        ui->pleaseChoose->move(70 * 1.5, 70 * 1.5);
        ui->repeatFolder->move(100 * 1.5, 150 * 1.5);
        ui->repeatFileOk->move(250 * 1.5, 200 * 1.5);


        //STYLESHEETS
        ui->repeatFileNameNotif->setStyleSheet("color: rgb(255, 255, 255);\nfont: 39pt \"Assassin$\";");
        ui->pleaseChoose->setStyleSheet("color: rgb(255, 255, 255);\nfont: 39pt \"Assassin$\";");
        ui->repeatFolder->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nborder-color: rgb(255, 255, 255);\nfont: 39pt \"Assassin$\";");
        ui->repeatFileOk->setStyleSheet("#repeatFileOk{\nbackground-image: url(:/uielements/uielements/okNoPaint1080p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#repeatFileOk:hover{\n	background-image: url(:/uielements/uielements/ok1080p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");


    }
    else if(res == 2){

        //SIZES
        setFixedSize(668 * 2, 341 * 2);
        ui->repeatFileNameNotif->resize(511 * 2, 41 * 2);
        ui->pleaseChoose->resize(671 * 2, 81 * 2);
        ui->repeatFolder->resize(461 * 2, 71 * 2);
        ui->repeatFileOk->resize(151 * 2, 101 * 2);


        //POSITIONS
        ui->repeatFileNameNotif->move(70 * 2, 30 * 2);
        ui->pleaseChoose->move(70 * 2, 70 * 2);
        ui->repeatFolder->move(100 * 2, 150 * 2);
        ui->repeatFileOk->move(250 * 2, 200 * 2);


        //STYLESHEETS
        ui->repeatFileNameNotif->setStyleSheet("color: rgb(255, 255, 255);\nfont: 52pt \"Assassin$\";");
        ui->pleaseChoose->setStyleSheet("color: rgb(255, 255, 255);\nfont: 52pt \"Assassin$\";");
        ui->repeatFolder->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nborder-color: rgb(255, 255, 255);\nfont: 52pt \"Assassin$\";");
        ui->repeatFileOk->setStyleSheet("#repeatFileOk{\nbackground-image: url(:/uielements/uielements/okNoPaint1440p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#repeatFileOk:hover{\n	background-image: url(:/uielements/uielements/ok1440p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");



    }
    else if(res == 3){

        //SIZES
        setFixedSize(668 * 3, 341 * 3);
        ui->repeatFileNameNotif->resize(511 * 3, 41 * 3);
        ui->pleaseChoose->resize(671 * 3, 81 * 3);
        ui->repeatFolder->resize(461 * 3, 71 * 3);
        ui->repeatFileOk->resize(151 * 3, 101 * 3);


        //POSITIONS
        ui->repeatFileNameNotif->move(70 * 3, 30 * 3);
        ui->pleaseChoose->move(70 * 3, 70 * 3);
        ui->repeatFolder->move(100 * 3, 150 * 3);
        ui->repeatFileOk->move(250 * 3, 200 * 3);


        //STYLESHEETS
        ui->repeatFileNameNotif->setStyleSheet("color: rgb(255, 255, 255);\nfont: 78pt \"Assassin$\";");
        ui->pleaseChoose->setStyleSheet("color: rgb(255, 255, 255);\nfont: 78pt \"Assassin$\";");
        ui->repeatFolder->setStyleSheet("color: rgb(255, 255, 255);\nbackground-color: rgb(0, 0, 0);\nborder-color: rgb(255, 255, 255);\nfont: 78pt \"Assassin$\";");
        ui->repeatFileOk->setStyleSheet("#repeatFileOk{\nbackground-image: url(:/uielements/uielements/okNoPaint2160p.PNG) 0 0 0 0 stretch stretch;\nbackground-color: rgb(0, 0, 0);\n}\n\nQPushButton#repeatFileOk:hover{\n	background-image: url(:/uielements/uielements/ok2160p.PNG) 0 0 0 0 stretch stretch;\n\nborder: none;\n}");



    }


    ui->repeatFileNameNotif->setText(QString::fromStdString(repeatFileName) + " found" );

    repeatFileName = "";


    setWindowFlags(Qt::WindowTitleHint);
    //sets background of the mod name window
    setStyleSheet("repeatFile {background-color: rgb(0, 0, 0);}");


    //sets background of the main window
    //setStyleSheet("repeatFile {border: 1px solid black}");




    setAutoFillBackground(true);



    if (repeatType == 0){

        ui->repeatFolder->addItem("dandk_map");
        ui->repeatFolder->addItem("dandk_std");
        ui->repeatFolder->addItem("dandk_texteffect");
        ui->repeatFolder->addItem("deude_map");
        ui->repeatFolder->addItem("deude_std");
        ui->repeatFolder->addItem("deude_texteffect");
        ui->repeatFolder->addItem("enggb_map");
        ui->repeatFolder->addItem("enggb_std");
        ui->repeatFolder->addItem("enggb_texteffect");
        ui->repeatFolder->addItem("engus_map");
        ui->repeatFolder->addItem("engus_std");

        ui->repeatFolder->addItem("engus_texteffect");
        ui->repeatFolder->addItem("finfi_map");
        ui->repeatFolder->addItem("finfi_std");
        ui->repeatFolder->addItem("finfi_texteffect");
        ui->repeatFolder->addItem("frafr_map");
        ui->repeatFolder->addItem("frafr_std");
        ui->repeatFolder->addItem("frafr_texteffect");
        ui->repeatFolder->addItem("itait_map");
        ui->repeatFolder->addItem("itait_std");
        ui->repeatFolder->addItem("itait_texteffect");
        ui->repeatFolder->addItem("jpnjp_map");

        ui->repeatFolder->addItem("jpnjp_std");
        ui->repeatFolder->addItem("jpnjp_texteffect");
        ui->repeatFolder->addItem("korkr_map");
        ui->repeatFolder->addItem("korkr_std");
        ui->repeatFolder->addItem("korkr_texteffect");
        ui->repeatFolder->addItem("nldnl_map");
        ui->repeatFolder->addItem("nldnl_std");
        ui->repeatFolder->addItem("nldnl_texteffect");
        ui->repeatFolder->addItem("norno_map");
        ui->repeatFolder->addItem("norno_std");
        ui->repeatFolder->addItem("norno_texteffect");

        ui->repeatFolder->addItem("polpl_map");
        ui->repeatFolder->addItem("polpl_std");
        ui->repeatFolder->addItem("polpl_texteffect");
        ui->repeatFolder->addItem("porbr_map");
        ui->repeatFolder->addItem("porbr_std");
        ui->repeatFolder->addItem("porbr_texteffect");
        ui->repeatFolder->addItem("porpt_map");
        ui->repeatFolder->addItem("porpt_std");
        ui->repeatFolder->addItem("porpt_texteffect");
        ui->repeatFolder->addItem("rusru_map");
        ui->repeatFolder->addItem("rusru_std");

        ui->repeatFolder->addItem("rusru_texteffect");
        ui->repeatFolder->addItem("spaar_map");
        ui->repeatFolder->addItem("spaar_std");
        ui->repeatFolder->addItem("spaar_texteffect");
        ui->repeatFolder->addItem("spaes_map");
        ui->repeatFolder->addItem("spaes_std");
        ui->repeatFolder->addItem("spaes_texteffect");
        ui->repeatFolder->addItem("swese_map");
        ui->repeatFolder->addItem("swese_std");
        ui->repeatFolder->addItem("swese_texteffect");
        ui->repeatFolder->addItem("thath_map");

        ui->repeatFolder->addItem("thath_std");
        ui->repeatFolder->addItem("thath_texteffect");
        ui->repeatFolder->addItem("turtr_map");
        ui->repeatFolder->addItem("turtr_std");
        ui->repeatFolder->addItem("turtr_texteffect");
        ui->repeatFolder->addItem("zhocn_map");
        ui->repeatFolder->addItem("zhocn_std");
        ui->repeatFolder->addItem("zhocn_texteffect");
        ui->repeatFolder->addItem("zhotw_map");
        ui->repeatFolder->addItem("zhotw_std");
        ui->repeatFolder->addItem("zhotw_texteffect");


    }
    else if(repeatType == 1){

ui->repeatFolder->addItem("na");
ui->repeatFolder->addItem("uk");
ui->repeatFolder->addItem("jp");
ui->repeatFolder->addItem("as");
ui->repeatFolder->addItem("eu");

    }
    else if(repeatType == 2){


        ui->repeatFolder->addItem("deude");
        ui->repeatFolder->addItem("engus");
        ui->repeatFolder->addItem("frafr");
        ui->repeatFolder->addItem("itait");
        ui->repeatFolder->addItem("japanese");
        ui->repeatFolder->addItem("jpnjp");
        ui->repeatFolder->addItem("korkr");
        ui->repeatFolder->addItem("polpl");
        ui->repeatFolder->addItem("porbr");
        ui->repeatFolder->addItem("rusru");
        ui->repeatFolder->addItem("spaar");
        ui->repeatFolder->addItem("spaes");
        ui->repeatFolder->addItem("thath");
        ui->repeatFolder->addItem("zhocn");
        ui->repeatFolder->addItem("zhotw");


    }
    else if(repeatType == 3){

        ui->repeatFolder->addItem("hi");
        ui->repeatFolder->addItem("low");

    }
    else if (repeatType == 4) {

        ui->repeatFolder->addItem("mapimage(hi)");
        ui->repeatFolder->addItem("mapimage(low)");


    }

}



repeatFile::~repeatFile()
{
    delete ui;
}




void repeatFile::on_repeatFileOk_clicked()
{

    close();

}

void repeatFile::on_repeatFolder_currentIndexChanged()
{



  repeatFileName = ui->repeatFolder->currentText().toLocal8Bit().constData();
qDebug() << QString::fromStdString(repeatFileName);

}
