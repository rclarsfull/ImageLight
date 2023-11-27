#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
#include "converter.h"
#include <QMainWindow>
//#include <QThread>

enum programmModes{withoutReference, withReference, referenceSelection};

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
private:
    programmModes mode;
    Ui::MainWindow *ui;
    QImage* image;
    QImage* greyImage;
    QImage* falschfarbenBild;
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
    void selectReference();
    void saveData();
    void delDrawabels(){orginalCanvas->delDrawabels(); update();};
    void randlichabfallCorrection();
};
#endif // MAINWINDOW_H
