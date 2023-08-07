#include "mainwindow.h"

#include <QApplication>
#include <converter.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Converter converter;
    QImage* image = new QImage("test.jpg");
    QImage ** greyScaledChannels = converter.getAsGreyScale(image);
    greyScaledChannels[0]->save("redChannel.jpg");
    greyScaledChannels[1]->save("greenChannel.jpg");
    greyScaledChannels[2]->save("blueChannel.jpg");

    QImage* greyScalePic = converter.combineChannels(greyScaledChannels);
    greyScalePic->save("greySaledPic.jpg");

    delete greyScalePic;
    delete image;
    delete[] greyScaledChannels;
    return a.exec();
}
