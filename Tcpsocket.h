#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QString>
#include <QTcpSocket>
#include "packets.h"

class Tcpsocket {
public:
    Tcpsocket(const QString &host, int port);
    ~Tcpsocket();

    bool connectToServer(); // Maakt connectie
    bool sendPacket(const sensor_packet& packet); // Kijkt of connectie is gelukt en verstuurd daarna de pointer

private:
    QString serverHost;
    int serverPort;
    QTcpSocket* socket_; // pointer socket_
};

#endif // TCPSOCKET_H
