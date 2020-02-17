#include "modname.h"
#include "ui_modname.h"
#include "sekiro.h"


modname::modname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modname)
{
    ui->setupUi(this);

//makes window frameless
setWindowFlags(Qt::Window | Qt::FramelessWindowHint);



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

    close();

}
