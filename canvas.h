#ifndef CANVAS_H
#define CANVAS_H

#include "drawable.h"
#include <QLabel>
#include <QFrame>
class MainWindow;

class Canvas : public QFrame
{
    bool isOriginalImage;
    int minCandela, maxCandela;
    Converter* converter;
    QImage* image;
    QImage* resizedImage;
    unsigned short (**candela)[Global::X_RESELUTION];
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
    int getMinCandela() const;
    void setMinCandela(int newMinCandela);
    int getMaxCandela() const;
    void setMaxCandela(int newMaxCandela);
    QPixmap* getCanvas();
    void saveDataAsCSV(QString fileName);
    void delDrawabels();
    QImage *getResizedImage() const;
    void setCandela(unsigned short (**candelaPtr)[Global::X_RESELUTION]){candela = candelaPtr;}
    bool saveCanvas(QString filename);

public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CANVAS_H
