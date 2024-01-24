#include "serverconnetor.h"
#include "mainwindow.h"
#include <iostream>
#include <QTcpSocket>
#include <QHostAddress>

ServerConnector::ServerConnector(MainWindow *mainWindow)
    : mainWindow(mainWindow) {}

void ServerConnector::connectAndSendData(float *data, unsigned short (*responeData)[Global::X_RESELUTION]) {
    QTcpSocket socket;
    QHostAddress hostAddress(mainWindow->getSettingsWindow()->getIpPythonServer());

    if (hostAddress.isNull()) {
        qDebug() << "Invalid server address" ;
        return;
    }
    socket.connectToHost(hostAddress, mainWindow->getSettingsWindow()->getPortPythonServer());
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to server" ;
        return;
    }

    // Convert and send data
    QByteArray dataArray(reinterpret_cast<const char*>(data), sizeof(float) * Global::X_RESELUTION * Global::Y_RESELUTION * 3);
    socket.write(dataArray);
    socket.waitForBytesWritten();

    QByteArray receivedData;
    int expectedSize = sizeof(unsigned short) * Global::X_RESELUTION * Global::Y_RESELUTION;
    socket.waitForReadyRead(mainWindow->getSettingsWindow()->getTimeoutPythonServer());
    while (receivedData.size() < expectedSize) {
        // Receive response data in chunks
        if (socket.waitForReadyRead(1000)) {
            receivedData.append(socket.readAll());
        } else {
            qDebug() << "Error: Timeout waiting for additional data from the server\nRecived: " <<receivedData.size() << " / " << expectedSize << " bytes\nMissing "
                     << expectedSize-receivedData.size() << "bytes";
            return;
        }
    }
    if (receivedData.size() == expectedSize) {
        std::memcpy(*responeData, receivedData.constData(), expectedSize);
    }else
        qDebug() << "Error: Expects " << expectedSize << " bytes";
    socket.disconnectFromHost();
}
