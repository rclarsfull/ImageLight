#include "serverconnetor.h"
#include "qthread.h"
#include <iostream>
#include <QTcpSocket>
#include <QHostAddress>

ServerConnector::ServerConnector(const char* serverIP, int serverPort)
    : serverIP(serverIP), serverPort(serverPort) {}

void ServerConnector::connectAndSendData(float *data, unsigned short (*responeData)[Global::X_RESELUTION]) {
    QTcpSocket socket;

    // Convert IP address from string to QHostAddress
    QHostAddress hostAddress(serverIP);
    if (hostAddress.isNull()) {
        qDebug() << "Invalid server address" ;
        return;
    }

    socket.connectToHost(hostAddress, serverPort);
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to server" ;
        return;
    }

    // Convert and send data
    QByteArray dataArray(reinterpret_cast<const char*>(data), sizeof(float) * Global::X_RESELUTION * Global::Y_RESELUTION * 3);
    socket.write(dataArray);

    // Wait for the data to be sent
    socket.waitForBytesWritten();

    socket.waitForReadyRead(680000);
    // Receive response data
    QByteArray receivedData;
    int expectedSize = sizeof(unsigned short) * Global::X_RESELUTION * Global::Y_RESELUTION;
    //QThread::sleep(5);
    while (receivedData.size() < expectedSize) {
        // Receive response data in chunks
        if (socket.waitForReadyRead(1000)) {
            receivedData.append(socket.readAll());
        } else {
            qDebug() << "Error: Timeout waiting for additional data from the server";
            return;
        }
    }
    qDebug() << "Recived: " << receivedData.size() << "bytes";
    if (receivedData.size() == expectedSize) {
        std::memcpy(*responeData, receivedData.constData(), expectedSize);
    }else
        qDebug() << "Error: Expects " << expectedSize << " bytes";
    socket.disconnectFromHost();
}
