#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
#include <QMainWindow>
#include <QThread>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage* image;
    QImage* flaschfarbenBild;
    Canvas* orginalCanvas;
    Canvas* resultCanvas;
    bool isShowingOriginal;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void converte();
    void sliderEvent();
    void speichernUnter();
};
#endif // MAINWINDOW_H
