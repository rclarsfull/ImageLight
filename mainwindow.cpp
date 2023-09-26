#include <QMimeData>
#include <ui_mainwindow.h>
#include <converter.h>
#include <mainwindow.h>
#include <qevent.h>
#include "perfomancetimer.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , flaschfarbenBild(NULL)
    , orginalCanvas(new Canvas())
    , resultCanvas(new Canvas())

{
    ui->setupUi(this);
    orginalCanvas->setOtherCanvas(resultCanvas);
    resultCanvas->setOtherCanvas(orginalCanvas);
    ui->verticalLayout_3->replaceWidget(ui->frame,orginalCanvas);
    ui->verticalLayout_3->replaceWidget(ui->frame_2,resultCanvas);
    delete ui->frame;
    delete ui->frame_2;
    orginalCanvas->setDebugLabel(ui->label);
    resultCanvas->setDebugLabel(ui->label);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    if(flaschfarbenBild != NULL)
        delete flaschfarbenBild;
    delete ui;
}

void MainWindow::converte()
{
    PerfomanceTimer timer("Converte Time");
    QString fileName = ui->lineEdit->text();

    image = QImage(fileName);
    flaschfarbenBild = new QImage(fileName);

    Converter::recolorImage(flaschfarbenBild);
    orginalCanvas->setImage(image);
    resultCanvas->setImage(*flaschfarbenBild);
    orginalCanvas->resize();
    resultCanvas->resize();
    update();

    flaschfarbenBild->save(fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1]);
    qDebug() << "Picture saved as:" << fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1];

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
//    foreach (const QUrl &url, event->mimeData()->urls()) {
//        QString fileName = url.toLocalFile();
//        qDebug() << "Dropped file:" << fileName;
//    }
    ui->lineEdit->setText(event->mimeData()->urls()[0].toLocalFile());
    converte();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    orginalCanvas->resize();
    resultCanvas->resize();
    update();
}



