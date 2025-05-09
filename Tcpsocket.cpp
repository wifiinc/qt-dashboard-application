#include "Tcpsocket.h"
#include <QTcpSocket>


Tcpsocket::Tcpsocket(const QString &host, int port)
    : serverHost(host), serverPort(port) {}

QString Tcpsocket::verzendPakket(const QString &cmd) {
    QTcpSocket socket;
    socket.connectToHost(serverHost, serverPort);
    if (!socket.waitForConnected(1000)) return "Failed to connect";

    socket.write(cmd.toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    QString response = socket.readAll();
    socket.disconnectFromHost();
    return response;
    //packet.h sturct aan maken
    // verstuurt een pointer wijsten naar de sturct
}
