#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "canvas.h"
#include "converter.h"
#include <QMainWindow>

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
    Converter converter;
    QImage* image;
    QImage* greyScalePic;
    QVector<QImage*> greyScaledChannels;
    Canvas* originalPic;
    Canvas* resultPic;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void converte();
};
#endif // MAINWINDOW_H
