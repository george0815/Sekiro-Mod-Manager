#ifndef SEKIRO_H
#define SEKIRO_H

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include <QMainWindow>
#include <Windows.h>




using namespace std;


extern string sekDir;

extern string modName;

extern struct mod{


    string name;

    string path;

    string modConfigPath;

    vector<string> files;

};



extern struct profile{


    string name;

    string path;

    string profileConfigPath;

    short modNum;

    vector<string> files;

};

extern vector<profile> profiles;

extern vector<mod> mods;


extern string trueModPath;


QT_BEGIN_NAMESPACE
namespace Ui { class Sekiro; }
QT_END_NAMESPACE



void key(DWORD);

class Sekiro : public QMainWindow
{
    Q_OBJECT

public:
    Sekiro(QWidget *parent = nullptr);
    ~Sekiro();





private slots:





    void on_addMod_clicked();


    void checkDir();

    void traverse(const QString &pattern, const QString &dirname, int mode);





    void debugFileList();

    void on_Install_clicked();

    void on_Uninstall_clicked();


    void unpackRepack(string line);

    void getSettings();

    void on_removeMod_clicked();

    void on_changeSekDir_clicked();

    void on_addProfile_clicked();

private:

    Ui::Sekiro *ui;
};
#endif // SEKIRO_H
