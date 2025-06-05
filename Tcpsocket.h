#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include "packets.h"
#include <QByteArray>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QTimer>

class Tcpsocket : public QObject
{
    Q_OBJECT
public:
    Tcpsocket(const QString& host, int port);
    ~Tcpsocket();

    bool connectToServer();
    bool sendPacket(const sensor_packet& packet);
    void updateConnection(const QString& newIp, int newPort);
    void setSensorList(const std::vector<std::pair<SensorType, int>>& sensorList);

signals:
    void packetReceived(const sensor_packet& packet);
    void temperatuurOntvangen(float temperatuur);

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
    std::vector<std::pair<SensorType, int>> sensorList_;
};

#endif // TCPSOCKET_H
