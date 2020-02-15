#ifndef ADDPROFILE_H
#define ADDPROFILE_H

#include <QDialog>
#include <sekiro.h>


using namespace std;

namespace Ui {
class addProfile;
}

class addProfile : public QDialog
{
    Q_OBJECT

public:
    explicit addProfile(QWidget *parent = nullptr);
    ~addProfile();


    void unpackRepackProfiles(string line);

    void traverseProfiles(const QString &pattern, const QString &dirname, int mode);


private slots:
    void on_profileConfirm_clicked();

private:
    Ui::addProfile *ui;
};

#endif // ADDPROFILE_H
