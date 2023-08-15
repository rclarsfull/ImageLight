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
    QImage image;
    QImage greyScalePic;
    QVector<QImage*> greyScaledChannels;
    QImage* newPic;
    Canvas* originalPic;
    Canvas* resultPic;

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void resizeEvent(QResizeEvent *event);

private slots:
    void converte();
};
#endif // MAINWINDOW_H
