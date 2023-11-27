#ifndef CANVAS_H
#define CANVAS_H

#include "drawable.h"
#include <QLabel>
#include <QFrame>
class MainWindow;

class Canvas : public QFrame
{
    bool isOriginalImage;
    int minGrey, maxGrey;
    Converter* converter;
    QImage* image;
    QImage* resizedImage;
    QImage* greyImage;
    QImage* resizedGreyImage;
    QPoint* pressedLocation;
    Canvas* otherCanvas;
    QLabel* debugLabel;
    QVector<Drawable*> drawabels;
    QPixmap canvas;
    MainWindow* mainWindow;
    bool isDelAction;

public:
    Canvas(bool isOriginalImage, Converter* converter, MainWindow* mainWindow);
    ~Canvas();
    void resize();

    void setImage(QImage *newImage);
    void setOtherCanvas(Canvas* other);
    void setPressedLocation(QPoint* point);
    QPoint* getPressedLocation();
    void setDebugLabel(QLabel *newDebugLabel);
    QVector<Drawable *>& getDrawabels();
    bool getIsOriginalImage() const;
    QImage* getImage();
    int getMinGrey() const;
    void setMinGrey(int newMinGrey);
    int getMaxGrey() const;
    void setMaxGrey(int newMaxGrey);
    QPixmap* getCanvas();
    void saveDataAsCSV(QString fileName);

    void delDrawabels();

    QImage *getResizedImage() const;

    void setGreyImage(QImage *newGreyImage);

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CANVAS_H
