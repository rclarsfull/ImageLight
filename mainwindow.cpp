#include "perfomancetimer.h"
#include <QMimeData>
#include <ui_mainwindow.h>
#include <converter.h>
#include <mainwindow.h>
#include <qevent.h>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mode(normalMode)
    , ui(new Ui::MainWindow)
    , image(NULL)
    , falschfarbenBild(NULL)
    , candela(new unsigned short[Global::Y_RESELUTION][Global::X_RESELUTION])
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
}

MainWindow::~MainWindow()
{
    if(falschfarbenBild != NULL){
        delete falschfarbenBild;
        falschfarbenBild = NULL;
    }
    if(candela != NULL){
        delete[] candela;
        candela = NULL;
    }
    if(image != NULL){
        delete image;
        image = NULL;
    }
    delete ui;
}

void MainWindow::converte()
{
    //qDebug() << converter.colorToGrey(QColor(255,255,255),0,0);
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    //PerfomanceTimer timer("Converte Time");
    QString fileName = ui->lineEdit->text();
    if(image != NULL){
        delete image;
        delete falschfarbenBild;
        image = NULL;
        falschfarbenBild = NULL;
    }
    image = new QImage(fileName);
    falschfarbenBild = new QImage(fileName);
    if (image->width() != Global::X_RESELUTION || image->height() != Global::Y_RESELUTION){
        qDebug() << "ERROR: Wrong image size!!\nExpected:\t" << Global::X_RESELUTION << "x" << Global::Y_RESELUTION
                 << "\nRecived:\t" << image->width() << "x" << image->height();
        delete image;
        delete falschfarbenBild;
        image = NULL;
        falschfarbenBild = NULL;
        return;
    }
    int minGrey = 0, maxGrey = 255;
    if(getMode() == normalMode){
        converter.updateCandela(candela, image);
        minGrey = converter.getMinCandela(candela);
        maxGrey = converter.getMaxCandela(candela);
        ui->minSlider->setSliderPosition(minGrey);
        ui->maxSlider->setSliderPosition(maxGrey);
        orginalCanvas->setMaxCandela(maxGrey);
        orginalCanvas->setMinCandela(minGrey);
        resultCanvas->setMaxCandela(maxGrey);
        resultCanvas->setMinCandela(minGrey);
        converter.updateFalschfarbenBild(candela, falschfarbenBild, minGrey, maxGrey);
    }
    orginalCanvas->setCandela(&candela);
    resultCanvas->setCandela(&candela);
    orginalCanvas->setImage(image);
    resultCanvas->setImage(falschfarbenBild);
    update();
    //falschfarbenBild->save(fileName.split(".")[0] + "[ReColored]." + fileName.split(".")[1]);
    QGuiApplication::restoreOverrideCursor();
}

void MainWindow::sliderEvent()
{
    qDebug() << "sliderEvent";
    int minCandela = ui->minSlider->value();
    int maxCandela = ui->maxSlider->value();
    if(falschfarbenBild != NULL)
        delete falschfarbenBild;
    falschfarbenBild = new QImage(*image);
    resultCanvas->setImage(falschfarbenBild);
    orginalCanvas->setMaxCandela(maxCandela);
    orginalCanvas->setMinCandela(minCandela);
    resultCanvas->setMaxCandela(maxCandela);
    resultCanvas->setMinCandela(minCandela);
    converter.updateFalschfarbenBild(candela, falschfarbenBild, minCandela, maxCandela);
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
    if(ui->calibrationCheckBox->isChecked())
        mode = calibrationMode;
    else
        mode = normalMode;
    qDebug() << "mode changed: " << mode;
}

void MainWindow::saveData()
{
    if(falschfarbenBild != NULL){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        "untitled.csv",
                                                        tr("CSV (*.csv)"));
        orginalCanvas->saveDataAsCSV(fileName);
    }
}

void MainWindow::randlichabfallCorrection()
{
    converter.calibrateLightCorrectionMatrix(candela, image);
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



