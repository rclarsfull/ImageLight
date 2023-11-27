#ifndef CONVERTER_H
#define CONVERTER_H

#include <QImage>
#include <QColor>
//#include <QThreadPool>
//#include <QMutex>
#define xReselution 2352
#define yReselution 1568

class MainWindow;
class Converter
{
    //static QThreadPool workerPool;
   // static QMutex imageMutex;
    MainWindow* mainWindow ;
    //float* lightCorrectionMatrix[xReselution][yReselution];
    float(*lightCorrectionMatrix)[yReselution];
    void resetLightCorrectionMatrix();
public:
    static double redModifer;
    static double greenModifer;
    static double blueModifer;
    Converter(MainWindow* mainWindow);;
    ~Converter();

    unsigned int greyToCandela(unsigned int);
    void recolorImage(QImage* greyImage, QImage *falschfarbenBild, int minGrey, int maxGrey);
    QColor greyToColor(unsigned int grey, unsigned int minGrey, unsigned int maxGrey);
    unsigned int colorToGrey(QColor color, unsigned x, unsigned y);
    unsigned int getMinGrey(QImage* greyImage);
    unsigned int getMaxGrey(QImage* greyImage);
    void calibrateLightCorrectionMatrix(QImage* image);
};


#endif // CONVERTER_H
