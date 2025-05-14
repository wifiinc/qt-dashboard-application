#include "Receivetcpsocket.h"


Receivetcpsocket::Receivetcpsocket(QObject* parent)
    : QObject(parent), socket_(new QTcpSocket(this)){
    connect(socket_,  &QTcpSocket::connected,this, &Receivetcpsocket::onConnected);
    connect(socket_, &QTcpSocket::disconnected, this, &Receivetcpsocket::onDisconnected);
    connect(socket_, &QTcpSocket::readyRead, this, &Receivetcpsocket::onReadyRead);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
    this, &Receivetcpsocket::onError);

    connect(&timer_, &QTimer::timeout, this, &Receivetcpsocket::requestSensorPacket);
}

void Receivetcpsocket::connectToServer(const QString& host, quint16 port) {
    socket_->connectToHost(host, port);
}

void Receivetcpsocket::onConnected() {
    qDebug() << "Verbonden met server!";
    timer_.start(5000);
}

void Receivetcpsocket::onDisconnected() {
    qDebug() << "Verbinding met server verbroken.";
    timer_.stop();
}

void Receivetcpsocket::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qDebug() << "Socket fout:" << socket_->errorString();
}

void Receivetcpsocket::onReadyRead(){
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

void Receivetcpsocket::requestSensorPacket() {
    if (!socket_->isOpen()) return;

    sensor_packet request;
    request.header.ptype = PacketType::DASHBOARD_GET;
    request.header.length = sizeof(sensor_packet_generic);
    request.data.generic.metadata.sensor_type = SensorType::TEMPERATURE;
    request.data.generic.metadata.sensor_id = 8;

    QByteArray data(reinterpret_cast<const char*>(&request), sizeof(sensor_header) + request.header.length);
    socket_->write(data);
    socket_->flush();

    qDebug() << "Sensor request verzonden.";
}
