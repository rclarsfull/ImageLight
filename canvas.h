#ifndef CANVAS_H
#define CANVAS_H

#include <QFrame>

class Canvas : public QFrame
{
    QImage image;
public:
    Canvas();
    void setImage(const QImage &newImage);

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // CANVAS_H
