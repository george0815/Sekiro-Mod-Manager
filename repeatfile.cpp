#include "repeatfile.h"
#include "ui_repeatfile.h"
#include "sekiro.h"
#include <QDebug>



repeatFile::repeatFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::repeatFile)
{


    ui->setupUi(this);

    ui->repeatFileNameNotif->setText(QString::fromStdString(repeatFileName) + " found" );

    repeatFileName = "";


    setWindowFlags(Qt::WindowTitleHint);
    //sets background of the mod name window
    setStyleSheet("repeatFile {background-color: rgb(0, 0, 0);}");


    //sets background of the main window
    //setStyleSheet("repeatFile {border: 1px solid black}");


    //sets the size
    setFixedSize(668, 341);

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
