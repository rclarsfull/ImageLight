#ifndef CANVAS_H
#define CANVAS_H

#include "drawable.h"
#include <QLabel>
#include <QFrame>

class Canvas : public QFrame
{
    bool isOriginalImage;
    QImage image;
    QImage resizedImage;
    QPoint* pressedLocation;
    Canvas* otherCanvas;
    QLabel* debugLabel;
    QVector<Drawable*> drawabels;
public:
    Canvas(bool isOriginalImage = false);
    ~Canvas();
    void setImage(const QImage &newImage);
    void setOtherCanvas(Canvas* other);
    void setPressedLocation(QPoint* point);
    QPoint* getPressedLocation();
    void setDebugLabel(QLabel *newDebugLabel);
    void resize();

    QVector<Drawable *>& getDrawabels();

    bool getIsOriginalImage() const;


public slots:

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};

#endif // CANVAS_H
