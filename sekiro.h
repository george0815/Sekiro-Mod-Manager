#ifndef SEKIRO_H
#define SEKIRO_H

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING


#include <QMainWindow>
#include <Windows.h>
#include <set>



using namespace std;



//hold sekiro directory
extern string sekDir;



//bool that hold whethers or not warnings are displayed
extern bool warning;


//hold name of mod that is being installed
extern string modName;





//holds data for mod
extern struct mod{


    string name;

    string path;

    string modConfigPath;

    vector<string> files;

};




//hold data for profile
extern struct profile{


    string name;

    string path;

    string profileConfigPath;

    short modNum;

    vector<string> files;

    string modengineConfig;

    string profileFolder;

};



//holds all profiles
extern vector<profile> profiles;



//holds all mods
extern vector<mod> mods;



//bool that holds whether the add profiles process is done
extern bool isProfileDone;


//bool that holds whether the profile had a valid file hierarchy
extern bool passed;



//holds string for the path to the mod folder of the mod being added
extern string trueModPath;




extern set<string> modengineFolders;

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


    //checks if mod path is empty
    static bool isModPathEmpty(string truemodpath);

    //looks in directory for profile files
    static void traverseProfiles(const QString &pattern, const QString &dirname, int mode);

    //unpacks and repacks compressed archives
    static void unpackRepack(string line);




     //checks if entered name matches a name of a previously installed mod or profile
     static int nameRepeatCheck(bool mode, string lineedit);


private slots:




    //adds mod to the mod manager
    void on_addMod_clicked();




    //on program startup, checks the dir.ini file for the sekiro directory, if there isnt an entry, then its prompts the user for it
    void checkDir();




    //looks in directory for mod files
    void traverse(const QString &pattern, const QString &dirname, int mode);










    //prints files in files vector for profiles and mods
    void debugFileList(int mode);



    //installs mod to sekiro directory
    void on_Install_clicked();




    //uninstalls mod from sekiro directory
    void on_Uninstall_clicked();








    //loads the installed mods
    void getSettings();



    //removes mod from manager
    void on_removeMod_clicked();



    //changes sekiro directory
    void on_changeSekDir_clicked();





    //adds profile(basically a merged mod that you can enable/disable without completely uninstalling)
    void on_addProfile_clicked();



    //installs profile to the sekiro directory
    void on_installProfile_clicked();








    //loads the installed profiles
    void getSettingsProfile();





    //uninstalls profile from the sekiro directory
    void on_uninstallProfile_clicked();



    //removes profile from the mod manager
    void on_removeProfile_clicked();



    //sets the folder modenegine uses to the folder for the profile that is at the current index of the combo box
    void on_setActiveProfile_clicked();




    //sets the folder modenegine uses to the default, which is "mods" in the sekiro directory
    void on_defaultProfile_clicked();




    //launches sekiro
    void on_launchSekiro_clicked();




    //on startup, checks if modengine is installed, if not it downloads and installs it for the user
    void modEngineCheck();



    //enables or disables warning messages
    void on_warnings_stateChanged();




    //checks if folder has sekiro.exe
    int sekiroCheck();




    //displays error if no mpds/profiles selected
    void error(int mode);





private:

    Ui::Sekiro *ui;
};
#endif // SEKIRO_H
