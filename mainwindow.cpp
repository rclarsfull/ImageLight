#include <QMimeData>
#include <ui_mainwindow.h>
#include <converter.h>
#include <mainwindow.h>
#include <qevent.h>
#include <QFileDialog>
#include "perfomancetimer.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mode(withoutReference)
    , ui(new Ui::MainWindow)
    , image(NULL)
    , flaschfarbenBild(NULL)
    , orginalCanvas(new Canvas(true, &converter))
    , resultCanvas(new Canvas(false, &converter))
    , converter(this)
    , isShowingOriginal(false)
{
    ui->setupUi(this);
    orginalCanvas->setOtherCanvas(resultCanvas);
    resultCanvas->setOtherCanvas(orginalCanvas);
    ui->tabWidget->removeTab(1);
    ui->tabWidget->removeTab(0);
    ui->tabWidget->addTab(orginalCanvas," Original");
    ui->tabWidget->addTab(resultCanvas,"Falschfarben");
    ui->tabWidget->setTabPosition(QTabWidget::South);

    orginalCanvas->setDebugLabel(ui->label);
    resultCanvas->setDebugLabel(ui->label);
    setAcceptDrops(true);

    ui->referenceLineEdit->setDisabled(true);
    ui->berreichSetzenButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    if(flaschfarbenBild != NULL){
        delete flaschfarbenBild;
    }
    if(image != NULL)
        delete image;

    delete ui;
}

programmModes MainWindow::getMode()
{
    return mode;
}

int MainWindow::getReferenceValue()
{
    if(ui->referenceLineEdit->text() == "" )
        return 0;

    bool ok;
    int reference = ui->referenceLineEdit->text().toInt(&ok);
    if(ok)
        return reference;
    else{
        qDebug() << "Error: Reference not an Integer! replaced with 0";
        return 0;
    }
}


void MainWindow::converte()
{
    PerfomanceTimer timer("Converte Time");
    QString fileName = ui->lineEdit->text();

    image = new QImage(fileName);
    flaschfarbenBild = new QImage(fileName);
    int minGrey = 0, maxGrey = 255;
    minGrey = converter.getMinGrey(flaschfarbenBild);
    maxGrey = converter.getMaxGrey(flaschfarbenBild);
    ui->minSlider->setSliderPosition(minGrey);
    ui->maxSlider->setSliderPosition(maxGrey);
    orginalCanvas->setMaxGrey(maxGrey);
    orginalCanvas->setMinGrey(minGrey);
    resultCanvas->setMaxGrey(maxGrey);
    resultCanvas->setMinGrey(minGrey);
    converter.recolorImage(flaschfarbenBild,minGrey,maxGrey);
    orginalCanvas->setImage(image);
    resultCanvas->setImage(flaschfarbenBild);
    update();
    flaschfarbenBild->save(fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1]);
}

void MainWindow::sliderEvent()
{
    qDebug() << "sliderEvent";
    int minGrey = ui->minSlider->value();
    int maxGrey = ui->maxSlider->value();
    delete resultCanvas->getImage();
    flaschfarbenBild = new QImage(*orginalCanvas->getImage());
    resultCanvas->setImage(flaschfarbenBild);
    orginalCanvas->setMaxGrey(maxGrey);
    orginalCanvas->setMinGrey(minGrey);
    resultCanvas->setMaxGrey(maxGrey);
    resultCanvas->setMinGrey(minGrey);
    converter.recolorImage(resultCanvas->getImage(),minGrey,maxGrey);
    resultCanvas->update();
}

void MainWindow::speichernUnter()
{
    if(flaschfarbenBild != NULL){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "/home/jana/untitled.png",
                                                    tr("Images (*.png *.jpg)"));
        qDebug() << resultCanvas->getCanvas()->save(fileName);
    }

}

void MainWindow::changeMode()
{
    if(ui->referenceCheckBox->isChecked())
        mode = withReference;
    else
        mode = withoutReference;

    ui->referenceLineEdit->setDisabled(mode == withoutReference);
    ui->berreichSetzenButton->setDisabled(mode == withoutReference);
    qDebug() << "mode changed: " << mode;
}

void MainWindow::selectReference()
{
    mode = referenceSelection;
    qDebug() << "mode changed: " << mode;
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
//    orginalCanvas->resize();
//    resultCanvas->resize();
    update();
}





