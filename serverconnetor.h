#ifndef SERVERCONNETOR_H
#define SERVERCONNETOR_H
#include "global.h"


class ServerConnector {
public:
    ServerConnector(const char* serverIP, int serverPort);
    void connectAndSendData(float *data, unsigned short (*responeData)[Global::Y_RESELUTION]);

private:
    const char* serverIP;
    int serverPort;
};

#endif // SERVERCONNETOR_H
