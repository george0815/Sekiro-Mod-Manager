#ifndef MODNAME_H
#define MODNAME_H

#include <QDialog>

using namespace std;



namespace Ui {
class modname;
}

class modname : public QDialog
{
    Q_OBJECT

public:
    explicit modname(QWidget *parent = nullptr);
    ~modname();


private slots:


    //asks the user for the name of the mod and puts it into modName
    void on_modNameConfirm_clicked();






    //decides whether the user is installing a modpack or not
    void on_isModPack_stateChanged();



private:


    Ui::modname *ui;
};

#endif // MODNAME_H
