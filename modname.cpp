#include "modname.h"
#include "ui_modname.h"
#include "sekiro.h"


modname::modname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::modname)
{
    ui->setupUi(this);

setWindowFlags(Qt::Window | Qt::FramelessWindowHint);



}

modname::~modname()
{
    delete ui;
}



void modname::on_modNameConfirm_clicked()
{


    modName = ui->modNameLineEdit->text().toLocal8Bit().constData();

    close();

}
