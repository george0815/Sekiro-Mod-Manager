#include "modname.h"
#include "ui_modname.h"
#include "sekiro.h"
#include <QMessageBox>


modname::modname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modname)
{
    ui->setupUi(this);



setWindowFlags(Qt::WindowTitleHint);
//sets background of the mod name window
setStyleSheet("modname {background-color: rgb(0, 0, 0);}");


//sets background of the main window
setStyleSheet("modname {border: 1px solid white}");


//sets the size

setFixedSize(624, 232);


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


    if(modName.find_first_not_of(' ') != std::string::npos && Sekiro::nameRepeatCheck(true, ui->modNameLineEdit->text().toLocal8Bit().constData()) == 0){

    close();

    }
    else if(!(modName.find_first_not_of(' ') != std::string::npos)){

        QFont sekFont("Assassin$");
        QFont errFont("Segoe UI", 8);

        QApplication::setFont(errFont);

        QMessageBox err;



       err.critical(this, "Error", "No name was entered");

       QApplication::setFont(sekFont);
    }


   else if(Sekiro::nameRepeatCheck(true, ui->modNameLineEdit->text().toLocal8Bit().constData()) == 1){


            QFont sekFont("Assassin$");
            QFont errFont("Segoe UI", 8);

            QApplication::setFont(errFont);

            QMessageBox err;



           err.critical(this, "Error", "Name already matches a previously installed mod");

           QApplication::setFont(sekFont);

        }

    }













