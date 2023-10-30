#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>
#include <QColor>
//#include <QThreadPool>
//#include <QMutex>

class MainWindow;
class Converter
{
    //static QThreadPool workerPool;
   // static QMutex imageMutex;
    MainWindow* mainWindow ;
public:
    Converter(MainWindow* mainWindow):mainWindow(mainWindow){};

    unsigned int greyToCandela(unsigned int);
    unsigned int getConversionPresition (unsigned int);
    void recolorImage(QImage* image, int minGrey, int maxGrey);
    inline QColor greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey);
    unsigned int colorToGrey(QColor color);
    unsigned int getMinGrey(QImage* greyImage);;
    unsigned int getMaxGrey(QImage* greyImage);
};

#endif // CONVERTER_H
