#include "Tcpsocket.h"
#include <QDebug>

Tcpsocket::Tcpsocket(const QString &host, int port)
    : serverHost(host), serverPort(port), socket_(new QTcpSocket) {}

Tcpsocket::~Tcpsocket()
{
    socket_->disconnect();
    socket_->deleteLater();
}

bool Tcpsocket::connectToServer()
{
    socket_->connectToHost(serverHost,serverPort);
    return socket_->waitForConnected(3000);
}

bool Tcpsocket::sendPacket(const sensor_packet& packet) {

    if (!socket_->isOpen() && !connectToServer()) {
        qDebug() << "Connectie mislukt:" << serverHost << ":" << serverPort ;
        return false;
    }

    int totalSize = sizeof(sensor_header) + packet.header.length;
    QByteArray buffer(reinterpret_cast<const char*>(&packet), totalSize);

    qint64 bytesWritten = socket_->write(buffer);
    socket_->flush();
    return socket_->waitForBytesWritten() && bytesWritten == totalSize;
}
