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



    void on_modNameConfirm_clicked();

private:


    Ui::modname *ui;
};

#endif // MODNAME_H
