#include "serverconnetor.h"
#include <iostream>
#include <QTcpSocket>
#include <QHostAddress>

ServerConnector::ServerConnector(const char* serverIP, int serverPort)
    : serverIP(serverIP), serverPort(serverPort) {}

void ServerConnector::connectAndSendData(float *data, unsigned short (*responeData)[Global::Y_RESELUTION]) {
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

    socket.waitForReadyRead(280000);
    // Receive response data
    QByteArray receivedData = socket.readAll();

    if (receivedData.isEmpty()) {
        qDebug() << "Error reading data from the server" ;
        return;
    }

    if (receivedData.size() == sizeof(unsigned short) * Global::Y_RESELUTION * Global::X_RESELUTION) {
        std::memcpy(*responeData, receivedData.constData(), sizeof(*responeData));
        // Process the received data as needed
    }

    socket.disconnectFromHost();
}
