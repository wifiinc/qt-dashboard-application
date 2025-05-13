#ifndef TCPSERVERHANDLER_H
#define TCPSERVERHANDLER_H

#include <QTcpServer>
#include <QTcpSocket>
#include "packets.h"

class TcpServerHandler : public QObject{
    Q_OBJECT
public:
    explicit TcpServerHandler(quint16 port, QObject* parent = nullptr);
    ~TcpServerHandler();
    bool startListening();

signals:
    void packetReceived(const sensor_packet& packet);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer* server_;
    QList<QTcpSocket*> clients_;
    quint16 port_;
    QMap<QTcpSocket*, QByteArray> socketBuffers_;

};

#endif // TCPSERVERHANDLER_H
