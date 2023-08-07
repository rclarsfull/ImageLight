#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "canvas.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , guiManager()
{
    ui->setupUi(this);
    Canvas* originalPic = new Canvas();
    Canvas* resultPic = new Canvas();
    ui->horizontalLayout->replaceWidget(ui->frame,originalPic);
    ui->horizontalLayout->replaceWidget(ui->frame_2,resultPic);
    delete ui->frame;
    delete ui->frame_2;
    guiManager.init(originalPic,resultPic);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::converte()
{
    guiManager.converte(ui->lineEdit->text());
}

