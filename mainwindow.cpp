#include "perfomancetimer.h"
#include <QMimeData>
#include <ui_mainwindow.h>
#include <converter.h>
#include <mainwindow.h>
#include <qevent.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mode(withoutReference)
    , ui(new Ui::MainWindow)
    , image(NULL)
    , greyImage(NULL)
    , falschfarbenBild(NULL)
    , orginalCanvas(new Canvas(true, &converter, this))
    , resultCanvas(new Canvas(false, &converter, this))
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
    if(falschfarbenBild != NULL){
        delete falschfarbenBild;
        falschfarbenBild = NULL;
    }
    if(greyImage != NULL){
        delete greyImage;
        greyImage = NULL;
    }
    if(image != NULL){
        delete image;
        image = NULL;
    }
    delete ui;
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
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    //PerfomanceTimer timer("Converte Time");
    QString fileName = ui->lineEdit->text();
    if(image != NULL){
        delete image;
        delete greyImage;
        delete falschfarbenBild;
        image = NULL;
        greyImage = NULL;
        falschfarbenBild = NULL;
    }
    image = new QImage(fileName);
    greyImage = new QImage(fileName);
    falschfarbenBild = new QImage(fileName);
    int minGrey = 0, maxGrey = 255;
    minGrey = converter.getMinGrey(greyImage);
    maxGrey = converter.getMaxGrey(greyImage);
    ui->minSlider->setSliderPosition(minGrey);
    ui->maxSlider->setSliderPosition(maxGrey);
    orginalCanvas->setMaxGrey(maxGrey);
    orginalCanvas->setMinGrey(minGrey);
    resultCanvas->setMaxGrey(maxGrey);
    resultCanvas->setMinGrey(minGrey);
    converter.recolorImage(greyImage,falschfarbenBild, minGrey, maxGrey);
    orginalCanvas->setImage(image);
    orginalCanvas->setGreyImage(greyImage);
    resultCanvas->setImage(falschfarbenBild);
    resultCanvas->setGreyImage(greyImage);
    update();
    //falschfarbenBild->save(fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1]);
    QGuiApplication::restoreOverrideCursor();
}

void MainWindow::sliderEvent()
{
    qDebug() << "sliderEvent";
    int minGrey = ui->minSlider->value();
    int maxGrey = ui->maxSlider->value();
    if(falschfarbenBild != NULL)
        delete falschfarbenBild;
    falschfarbenBild = new QImage(*image);
    resultCanvas->setImage(falschfarbenBild);
    orginalCanvas->setMaxGrey(maxGrey);
    orginalCanvas->setMinGrey(minGrey);
    resultCanvas->setMaxGrey(maxGrey);
    resultCanvas->setMinGrey(minGrey);
    converter.updateFalschfarbenBild(greyImage,falschfarbenBild,minGrey,maxGrey);
//    Converter::redModifer = (double)ui->redVerticalSlider->value()/500;
//    Converter::greenModifer = (double)ui->GreenVerticalSlider_2->value()/500;
//    Converter::blueModifer = (double)ui->BlueVerticalSlider_3->value()/500;
    // R:  0.68 G:  0.88 B:  1.6
    //R:  1.014 G:  0.988 B:  1.014
    qDebug() << "R: " << Converter::redModifer << "G: " << Converter::greenModifer << "B: " << Converter::blueModifer;
    update();
    resultCanvas->update();
}

void MainWindow::speichernUnter()
{
    if(falschfarbenBild != NULL){
        resultCanvas->update();
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

void MainWindow::saveData()
{
    if(falschfarbenBild != NULL){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        "untitled.csv",
                                                        tr("Images (*.csv)"));
        orginalCanvas->saveDataAsCSV(fileName);
    }
}

void MainWindow::randlichabfallCorrection()
{
    converter.calibrateLightCorrectionMatrix(image);
    converte();
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
    update();
}

programmModes MainWindow::getMode()
{
    return mode;
}



