#ifndef SEKIRO_H
#define SEKIRO_H

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING

#include <Windows.h>
#include <QMainWindow>
#include <set>



using namespace std;



//hold which resolution is currently enabled
extern short res;

//hold whether mod is valid
extern bool isModValid;

//hold sekiro directory
extern string sekDir;

//holds folder for repeated file name
extern string repeatFileName;

//decides the combo box options for the repaet file dialogue
extern short repeatType;

//bool that hold whethers or not warnings are displayed
extern bool warning;

//bool that hold whethers or not the user is installing a modpack
extern bool modpackBool;

//hold name of mod that is being installed
extern string modName;

//log file
extern ofstream logFile;

//holds modpack name
extern string modpackName;

//holds data for mod
extern struct mod{


    string name;

    string path;

    string modConfigPath;

    vector<string> files;

    string isInstalled;

};

//hold data for profile
extern struct profile{


    string name;

    string isInstalledP;

    string path;

    string profileConfigPath;

    short modNum;

    vector<string> files;

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

    //unpacks and repacks compressed archives
    static void unpackRepack(string line);

    //looks in directory for mod files
    static void traverse(const QString &pattern, const QString &dirname, int mode, bool = true);

     //checks if entered name matches a name of a previously installed mod or profile
     static int nameRepeatCheck(bool mode, string lineedit);


private slots:




    //adds mod to the mod manager
    void on_addMod_clicked();

    //on program startup, checks the dir.ini file for the sekiro directory, if there isnt an entry, then its prompts the user for it
    void checkDir();

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
    int sekiroCheck(int mode = 0);

    //displays error if no mpds/profiles selected
    void error(int mode);

    //logs the program
    void log(string log);

    //sets log on or off
    void on_logOn_stateChanged();

    //handles settings
    void settings(int mode);

    //closes program on launch
    void on_closeOnLaunch_stateChanged();

    //gets active profile
    void getActiveProfile();

    //edits modengine.ini
    void modengineEdit(string dll, bool unchain = false);

    //chains and unchains dll
    void on_chainUnchain_stateChanged();

    //toggle show debug files in modengine on and off
    void on_debug_stateChanged();

    //if checked, skips logos for sekiro
    void on_logo_stateChanged();

    //toggles cache file paths in modengine on and off
    void on_cache_stateChanged();

    //toggles load UXM files in modengine on and off
    void on_uxm_stateChanged();

    //parses settings from modengine.ini
    void parseSettings();

    //applies settings to moddenigne.ini
    void applySettings();

    //if checked, reinstalls mods after directory change
    void on_reinstallAfterDirChange_stateChanged();

    //if checked, reapplies modengine settings after directory
    void on_keepModengineSettings_stateChanged();

    //handles choosing resolution
    void on_resolution_currentIndexChanged();

    //handles resizing UI when resolution is chosen
    void resChange();

    //checks which resolution the icon for installed modds should be
    void iconCheck(int mode, int i= 0);

private:

    Ui::Sekiro *ui;
};
#endif // SEKIRO_H
