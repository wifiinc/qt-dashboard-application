#include "LightClient.h"
#include <QTcpSocket>

LightClient::LightClient(const QString &host, int port)
    : serverHost(host), serverPort(port) {}

QString LightClient::sendCommand(const QString &cmd) {
    QTcpSocket socket;
    socket.connectToHost(serverHost, serverPort);
    if (!socket.waitForConnected(1000)) return "Failed to connect";

    socket.write(cmd.toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    QString response = socket.readAll();
    socket.disconnectFromHost();
    return response;
}
