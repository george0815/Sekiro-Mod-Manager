#ifndef ADDPROFILE_H
#define ADDPROFILE_H

#include <QDialog>
#include <sekiro.h>


using namespace std;


//holds string for the path to the mod folder of the mod being added to a profile
extern string modProfilePath;

namespace Ui {
class addProfile;
}

class addProfile : public QDialog
{
    Q_OBJECT

public:
    explicit addProfile(QWidget *parent = nullptr);
    ~addProfile();









private slots:

    //adds profile to manager
    void on_profileConfirm_clicked();


    //cancels adding a profile
    void on_cancel_clicked();

private:
    Ui::addProfile *ui;
};

#endif // ADDPROFILE_H
