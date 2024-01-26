// settingswindow.cpp
#include "settingswindow.h"
#include "mainwindow.h"
#include <QSettings>
#include <QMessageBox>
#include <QFormLayout>

SettingsWindow::SettingsWindow(MainWindow *parent, QSettings* settings)
    : QDialog(parent)
    , settings(settings)
    , maxCandelaLineEdit(new QLineEdit(this))
    , ipLineEdit(new QLineEdit(this))
    , portLineEdit(new QLineEdit(this))
    , timeoutLineEdit(new QLineEdit(this))
    , skalaLabelCount(new QLineEdit(this))
    , calibrateButton(new QPushButton(this))
    , languageComboBox(new QComboBox(this))
    , calibrationModeCheckbox(new QCheckBox(this))
    , mainWindow(parent)
{    
    languageComboBox->addItems(LANGUAGE_OPTIONS);
    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Label on every x Value in Skala:", skalaLabelCount);
    formLayout->addRow("Max Candela:", maxCandelaLineEdit);
    formLayout->addRow("IP Address:", ipLineEdit);
    formLayout->addRow("Port:", portLineEdit);
    formLayout->addRow("Timeout:", timeoutLineEdit);
    formLayout->addRow("Language for '.csv' filesave", languageComboBox);
    formLayout->addRow("Calibrate Lense Correction", calibrateButton);
    formLayout->addRow("Calibration mode", calibrationModeCheckbox);
    layout->addLayout(formLayout);
    QPushButton *saveButton = new QPushButton("Save", this);
    connect(saveButton, &QPushButton::clicked, this, &SettingsWindow::saveSettings);
    connect(calibrateButton, &QPushButton::clicked, this, [](){ &MainWindow::randlichabfallCorrection;});
    connect(calibrationModeCheckbox, &QCheckBox::stateChanged, this, [](){ &MainWindow::changeMode;});
    layout->addWidget(saveButton);
    skalaLabelCount->setText(settings->value("LABEL_COUNT",50).toString());
    maxCandelaLineEdit->setText(settings->value("MAX_CANDELA", 1500).toString());
    ipLineEdit->setText(settings->value("IP_PYTHON_SERVER", "127.0.0.1").toString());
    portLineEdit->setText(settings->value("PORT_PYTHON_SERVER", 12345).toString());
    timeoutLineEdit->setText(settings->value("TIMEOUT_FOR_PYTHON_SERVER", 2000).toString());
    languageComboBox->setCurrentText(settings->value("LANGUAGE","German").toString());
    if(settings->value("LANGUAGE").toString() == "German"){
        languageComboBox->setItemText(0,LANGUAGE_OPTIONS[0]);
        languageComboBox->setItemText(1,LANGUAGE_OPTIONS[1]);
    }else{
        languageComboBox->setItemText(0,LANGUAGE_OPTIONS[1]);
        languageComboBox->setItemText(1,LANGUAGE_OPTIONS[0]);
    }
    settings->setValue("MAX_CANDELA", maxCandelaLineEdit->text().toInt());
    settings->setValue("IP_PYTHON_SERVER", ipLineEdit->text());
    settings->setValue("PORT_PYTHON_SERVER", portLineEdit->text().toInt());
    settings->setValue("TIMEOUT_FOR_PYTHON_SERVER", timeoutLineEdit->text().toInt());
    settings->setValue("LANGUAGE", languageComboBox->currentText());
    settings->setValue("LABEL_COUNT", skalaLabelCount->text().toInt());
    calibrationModeCheckbox->setChecked(false);
}
void SettingsWindow::createWidgets() {

}
QLocale SettingsWindow::getLocation()
{
    if(settings->value("LANGUAGE").toString() == "German")
        return QLocale::German;
    return QLocale::English;
}
int SettingsWindow::getMaxCandela()
{
    return settings->value("MAX_CANDELA").toInt();
}
QString SettingsWindow::getIpPythonServer()
{
    return settings->value("IP_PYTHON_SERVER").toString();
}

bool SettingsWindow::isClaibrationMode()
{
    return calibrationModeCheckbox->isChecked();
}
int SettingsWindow::getPortPythonServer()
{
    return settings->value("PORT_PYTHON_SERVER").toInt();
}
int SettingsWindow::getLabelCount()
{
    return settings->value("LABEL_COUNT").toInt();
}
int SettingsWindow::getTimeoutPythonServer()
{
    return settings->value("TIMEOUT_FOR_PYTHON_SERVER").toInt();
}
void SettingsWindow::saveSettings() {
    settings->setValue("MAX_CANDELA", maxCandelaLineEdit->text().toInt());
    settings->setValue("IP_PYTHON_SERVER", ipLineEdit->text());
    settings->setValue("PORT_PYTHON_SERVER", portLineEdit->text().toInt());
    settings->setValue("TIMEOUT_FOR_PYTHON_SERVER", timeoutLineEdit->text().toInt());
    settings->setValue("LANGUAGE", languageComboBox->currentText());
    settings->setValue("LABEL_COUNT", skalaLabelCount->text().toInt());
    mainWindow->changeMode();
    QMessageBox::information(this, "Settings Saved", "Settings have been saved.");
}
