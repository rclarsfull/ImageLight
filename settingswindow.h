
#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QLocale>
#include <QDialog>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QSettings>
#include <QComboBox>

class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent, QSettings* settings);

    int getMaxCandela();
    QLocale getLocation();;
    int getTimeoutPythonServer();
    int getPortPythonServer();
    QString getIpPythonServer();
private slots:
    void saveSettings();


private:
    const QStringList LANGUAGE_OPTIONS = { "German", "English"};
    QSettings *settings;
    QLineEdit *maxCandelaLineEdit;
    QLineEdit *ipLineEdit;
    QLineEdit *portLineEdit;
    QLineEdit *timeoutLineEdit;
    QComboBox* languageComboBox;

    void createWidgets();
};

#endif // SETTINGSWINDOW_H
