#include <QMimeData>
#include <ui_mainwindow.h>
#include <converter.h>
#include <mainwindow.h>
#include <qevent.h>
#include "perfomancetimer.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newPic(NULL)
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
    if(newPic != NULL)
        delete newPic;
    delete greyScaledChannels[0];
    delete greyScaledChannels[1];
    delete greyScaledChannels[2];
    delete ui;
}

void MainWindow::converte()
{
    PerfomanceTimer timer("Converte Time");
    QString fileName = ui->lineEdit->text();
    image = QImage(fileName);
    greyScaledChannels = Converter::getAsGreyScale(&image);
    //greyScaledChannels[0]->save("redChannel.jpg");
    //greyScaledChannels[1]->save("greenChannel.jpg");
    //greyScaledChannels[2]->save("blueChannel.jpg");

    newPic = Converter::combineChannels(greyScaledChannels);
    newPic->save(fileName.split(".")[0] + "[gray]." + fileName.split(".")[1]);
    qDebug() << "Picture saved as:" << fileName.split(".")[0] + "[gray]." + fileName.split(".")[1];
    Converter::greyImageToColorImage(newPic, Converter::getMinGrey(newPic), Converter::getMaxGrey(newPic));
    newPic->save(fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1]);
    qDebug() << "Picture saved as:" << fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1];

    originalPic->setImage(image);
    resultPic->setImage(*newPic);
    originalPic->resize();
    resultPic->resize();
    update();
    //qDebug() <<timer.getTimeNS();

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
    originalPic->resize();
    resultPic->resize();
    update();
}



