#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
#include "converter.h"
#include "settingswindow.h"
#include <QMainWindow>
#include <QSettings>
//#include <QThread>

enum programmModes{normalMode, calibrationMode};


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    programmModes getMode();
    int getReferenceValue();
    SettingsWindow* getSettingsWindow(){return &settingsWindow;};
private:
    QSettings settings;
    SettingsWindow settingsWindow;
    programmModes mode;
    Ui::MainWindow *ui;
    QImage* image;
    QImage* falschfarbenBild;
    unsigned short(*candela)[Global::X_RESELUTION];
    Canvas* orginalCanvas;
    Canvas* resultCanvas;
    Converter converter;
    bool isShowingOriginal;


    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void converte();
    void sliderEvent();
    void speichernUnter();
    void changeMode();
    void saveData();
    void openSettings();
    void delDrawabels(){orginalCanvas->delDrawabels(); update();};
    void randlichabfallCorrection();
};
#endif // MAINWINDOW_H
