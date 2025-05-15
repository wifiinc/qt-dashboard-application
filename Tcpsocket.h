#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <QTimer.h>
#include <QString>
#include <QTcpSocket>
#include "packets.h"

class Tcpsocket : public QObject{
    Q_OBJECT
public:
    Tcpsocket(const QString &host, int port);
    ~Tcpsocket();

    bool connectToServer(); // Maakt connectie
    bool sendPacket(const sensor_packet& packet); // Kijkt of connectie is gelukt en verstuurd daarna de pointer
    void updateConnection(const QString& newIp, int newPort);

signals:
    void packetReceived(const sensor_packet& packet);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void requestSensorPacket();


private:
    QString socketHost;
    int socketPort;
    QTcpSocket* socket_; // pointer socket_
    QMap<QTcpSocket*, QByteArray> socketBuffers_;
    QTimer timer_;
};

#endif // TCPSOCKET_H
