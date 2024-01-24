#ifndef SERVERCONNETOR_H
#define SERVERCONNETOR_H
#include "global.h"


class MainWindow;
class ServerConnector {
public:
    ServerConnector(MainWindow * mainWindow);
    void connectAndSendData(float *data, unsigned short (*responeData)[Global::X_RESELUTION]);

private:
    MainWindow *mainWindow;
};

#endif // SERVERCONNETOR_H
