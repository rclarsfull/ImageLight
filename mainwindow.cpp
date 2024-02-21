#include <QMimeData>
#include <ui_mainwindow.h>
#include <converter.h>
#include <mainwindow.h>
#include <qevent.h>
#include <QFileDialog>
#include <QCoreApplication>
#include <QImageReader>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , settings("Deutsche Flugsicherung","ImageLight")
    , settingsWindow(this, &settings)
    , mode(normalMode)
    , ui(new Ui::MainWindow)
    , image(NULL)
    , falschfarbenBild(NULL)
    , candela(new unsigned short[Global::Y_RESELUTION][Global::X_RESELUTION])
    , converter(new Converter(this))
    , orginalCanvas(new Canvas(true, converter, this))
    , resultCanvas(new Canvas(false, converter, this))
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
    delete converter;
    delete ui;
}

void MainWindow::converte()
{
    QGuiApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    QString fileName = ui->lineEdit->text();
    if(fileName!=""){
        if(image != NULL){
            delete image;
            delete falschfarbenBild;
            image = NULL;
            falschfarbenBild = NULL;
        }
        image = new QImage(fileName);
        falschfarbenBild = new QImage(fileName);
        if (image->width() != Global::X_RESELUTION || image->height() != Global::Y_RESELUTION){
            //qDebug() << "ERROR: Wrong image size!!\nExpected:\t" << Global::X_RESELUTION << "x" << Global::Y_RESELUTION
            //         << "\nRecived:\t" << image->width() << "x" << image->height();
            delete image;
            delete falschfarbenBild;
            image = NULL;
            falschfarbenBild = NULL;
            ui->lineEdit->setText("");
            QGuiApplication::restoreOverrideCursor();
            return;
        }
        orginalCanvas->setImage(image);
        orginalCanvas->setOriginalImage(image);
        resultCanvas->setImage(falschfarbenBild);
        resultCanvas->setOriginalImage(image);
        orginalCanvas->update();
        int minGrey = 0, maxGrey = 255;
        if(getMode() == normalMode){
            converter->updateCandela(candela, image);
            minGrey = converter->getMinCandela(candela);
            maxGrey = converter->getMaxCandela(candela);
            ui->minSlider->setSliderPosition(minGrey);
            ui->maxSlider->setSliderPosition(maxGrey);
            orginalCanvas->setMaxCandela(maxGrey);
            orginalCanvas->setMinCandela(minGrey);
            resultCanvas->setMaxCandela(maxGrey);
            resultCanvas->setMinCandela(minGrey);
            converter->updateFalschfarbenBild(candela, falschfarbenBild, minGrey, maxGrey);
        }
        orginalCanvas->setCandela(&candela);
        resultCanvas->setCandela(&candela);
        update();
    }
    QGuiApplication::restoreOverrideCursor();
}


void MainWindow::sliderEvent()
{
    if(ui->minSlider->value() < ui->maxSlider->value()){
        int minCandela = ui->minSlider->value();
        int maxCandela = ui->maxSlider->value();
        resultCanvas->setImage(falschfarbenBild);
        orginalCanvas->setMaxCandela(maxCandela);
        orginalCanvas->setMinCandela(minCandela);
        resultCanvas->setMaxCandela(maxCandela);
        resultCanvas->setMinCandela(minCandela);
        converter->updateFalschfarbenBild(candela, falschfarbenBild, minCandela, maxCandela);
        resultCanvas->update();
    }
}
void MainWindow::changeSliderTracking()
{
    bool tracking = !ui->maxSlider->hasTracking();
    ui->maxSlider->setTracking(tracking);
    ui->minSlider->setTracking(tracking);
}

void MainWindow::speichernUnter()
{
    if(falschfarbenBild != NULL){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                        "untitled.png",
                                                        tr("Images (*.png *.jpg)"));
        if(resultCanvas->saveCanvas(fileName))
            qDebug() << "Saved in:" << fileName;
    }
}

void MainWindow::changeMode()
{
    if(settingsWindow.isClaibrationMode())
        mode = calibrationMode;
    else
        mode = normalMode;
}

void MainWindow::saveData()
{
    if(falschfarbenBild != NULL){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled.csv", "CSV (*.csv)");
        orginalCanvas->saveDataAsCSV(fileName);
        qDebug() << "Saved in:" << fileName;
    }
}

void MainWindow::openSettings()
{
    settingsWindow.exec();
}

void MainWindow::randlichabfallCorrection()
{
    converter->calibrateLightCorrectionMatrix(candela, image);
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



