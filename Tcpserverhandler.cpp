#include "Tcpserverhandler.h"
#include <QDebug>

TcpServerHandler::TcpServerHandler(quint16 port,QObject* parent)
    : QObject(parent), server_(new QTcpServer(this)), port_(port){
    connect(server_, &QTcpServer::newConnection, this, &TcpServerHandler::onNewConnection);
}

TcpServerHandler::~TcpServerHandler(){
    server_->close();
    qDeleteAll(clients_);
}

bool TcpServerHandler::startListening(){
    if(!server_->listen(QHostAddress::Any,port_)){
        qDebug() << "kan niet luisteren op poort" << port_ << ":" << server_-> errorString();
        return false;
    }
    qDebug() << "Server luisteren op poort" << port_;
    return true;
}

void TcpServerHandler::onReadyRead(){
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if(!client){
        return;
    }

    QByteArray& buffer = socketBuffers_[client];
    buffer.append(client->readAll());

    while(true){
        //Hebben we minstens genoeg voor de header?
        if(buffer.size() <static_cast<int>(sizeof(sensor_header))){
            return;
        }

        //lees header
        sensor_header header;
        std::memcpy(&header, buffer.constData(), sizeof(sensor_header));
        int expectedSize = sizeof(sensor_header) + header.length;

        //hebben we al genoeg voor het hele pakket?
        if(buffer.size() < expectedSize){
            return;
        }

        //hebben we een volledig pakket
        QByteArray packetData = buffer.left(expectedSize);
        buffer.remove(0, expectedSize);

        //zet om naar struct
        sensor_packet packet;
        std::memcpy(&packet, packetData.constData(), expectedSize);

        // emit signaal naar rest
        emit packetReceived(packet);

    }
}
