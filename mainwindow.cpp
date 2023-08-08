#include <QMimeData>
#include "mainwindow.h"
#include "qevent.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , originalPic(new Canvas())
    , resultPic(new Canvas())
{
    ui->setupUi(this);
    originalPic->setOtherCanvas(resultPic);
    resultPic->setOtherCanvas(originalPic);
    ui->verticalLayout_3->replaceWidget(ui->frame,originalPic);
    ui->verticalLayout_3->replaceWidget(ui->frame_2,resultPic);
    delete ui->frame;
    delete ui->frame_2;
    originalPic->setDebugLabel(ui->label);
    resultPic->setDebugLabel(ui->label);
    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete greyScalePic;
    delete image;
    delete greyScaledChannels[0];
    delete greyScaledChannels[1];
    delete greyScaledChannels[2];
    delete ui;
}

void MainWindow::converte()
{
    QString fileName = ui->lineEdit->text();
    image = new QImage(fileName);
    greyScaledChannels = converter.getAsGreyScale(image);
    //greyScaledChannels[0]->save("redChannel.jpg");
    //greyScaledChannels[1]->save("greenChannel.jpg");
    //greyScaledChannels[2]->save("blueChannel.jpg");

    QImage* greyScalePic = converter.combineChannels(greyScaledChannels);
    greyScalePic->save(fileName.split(".")[0] + "[gray]." + fileName.split(".")[1]);
    qDebug() << "Picture saved as:" << fileName.split(".")[0] + "[gray]." + fileName.split(".")[1];

    originalPic->setImage((*image).scaledToWidth(originalPic->width(),Qt::SmoothTransformation));
    resultPic->setImage((*greyScalePic).scaledToWidth(resultPic->width(),Qt::SmoothTransformation));
    update();

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
}

