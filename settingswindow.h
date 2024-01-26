
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include "qcheckbox.h"
#include <QLocale>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QSettings>
#include <QComboBox>
class MainWindow;
class SettingsWindow : public QDialog {
    Q_OBJECT
public:
    SettingsWindow(MainWindow *parent, QSettings* settings);

    int getMaxCandela();
    QLocale getLocation();;
    int getTimeoutPythonServer();
    int getPortPythonServer();
    int getLabelCount();
    QString getIpPythonServer();
    bool isClaibrationMode();
private slots:
    void saveSettings();


private:
    const QStringList LANGUAGE_OPTIONS = { "German", "English"};
    QSettings *settings;
    QLineEdit *maxCandelaLineEdit;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *timeoutLineEdit;
    QLineEdit *skalaLabelCount;
    QPushButton *calibrateButton;
    QComboBox* languageComboBox;
    QCheckBox* calibrationModeCheckbox;
    MainWindow* mainWindow;

    void createWidgets();
};

#endif // SETTINGSWINDOW_H
