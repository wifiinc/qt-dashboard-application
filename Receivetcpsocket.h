#ifndef RECEIVETCPSOCKET_H
#define RECEIVETCPSOCKET_H

#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QDebug>
#include <cstring>
#include <QTimer.h>
#include "packets.h"

class Receivetcpsocket : public QObject{
    Q_OBJECT
public:
    explicit Receivetcpsocket(QObject* parent = nullptr);
    ~Receivetcpsocket();
    void connectToServer(const QString& host, quint16 port);

signals:
    void packetReceived(const sensor_packet& packet);

private slots:
    void onReadyRead();
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError socketError);
    void requestSensorPacket();

private:
    QTcpSocket* socket_;
    QMap<QTcpSocket*, QByteArray> socketBuffers_;
    QTimer timer_;

};

#endif // RECEIVETCPSOCKET_H
