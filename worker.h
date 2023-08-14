#ifndef WORKER_H
#define WORKER_H
#include <QImage>
#include <QRunnable>


class Worker :public QRunnable
{
public:
    Worker(void* func, QImage* image, int row);
};

#endif // WORKER_H
