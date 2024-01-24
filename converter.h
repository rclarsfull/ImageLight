#ifndef CONVERTER_H
#define CONVERTER_H
#include "global.h"
#include "serverconnetor.h"
#include <QImage>
#include <QColor>

class MainWindow;
class Converter
{
    MainWindow* mainWindow ;
    float(*lightCorrectionMatrix)[Global::Y_RESELUTION];
    void resetLightCorrectionMatrix();
    ServerConnector pythonServer;
public:
    Converter(MainWindow* mainWindow);;
    ~Converter();
    float *colorToRGBArray(QColor color, unsigned int x, unsigned int y);
    QColor candelaToColor(unsigned short candela, unsigned int minGrey, unsigned int maxGrey);

    void updateCandela(unsigned short (*candela)[Global::X_RESELUTION], QImage *image);
    unsigned int sRGBtoLinearRGB(QColor color, unsigned x, unsigned y);
    unsigned int getMinCandela(unsigned short (*candela)[Global::X_RESELUTION]);
    unsigned int getMaxCandela(unsigned short (*candela)[Global::X_RESELUTION]);
    void calibrateLightCorrectionMatrix(unsigned short (*candela)[Global::X_RESELUTION], QImage *image);
    void updateFalschfarbenBild(unsigned short (*candela)[Global::X_RESELUTION], QImage *falschfarbenBild, int minGrey, int maxGrey);
    static int scaleCordtoCanvas(int cord, int sizeCanvas, int sizeImage);
private:
    double sRGBToLinear(double value);
};


#endif // CONVERTER_H
