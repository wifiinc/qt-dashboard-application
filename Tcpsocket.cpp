#include "Tcpsocket.h"
#include <cstring>
#include <QDebug>

Tcpsocket::Tcpsocket(const QString &host, int port)
    : socketHost(host), socketPort(port), socket_(new QTcpSocket) {

    connect(socket_,  &QTcpSocket::connected,this, &Tcpsocket::onConnected);
    connect(socket_, &QTcpSocket::disconnected, this, &Tcpsocket::onDisconnected);
    connect(socket_, &QTcpSocket::readyRead, this, &Tcpsocket::onReadyRead);
    connect(socket_, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
    this, &Tcpsocket::onError);

    connect(&timer_, &QTimer::timeout, this, &Tcpsocket::requestSensorPacket);
}

Tcpsocket::~Tcpsocket()
{
    if (socket_->state() != QAbstractSocket::ConnectedState) {
        socket_->disconnect();
    }
    socket_->deleteLater();
}

bool Tcpsocket::connectToServer()
{
    socket_->connectToHost(socketHost,socketPort);
    return socket_->waitForConnected(3000);
}

bool Tcpsocket::sendPacket(const sensor_packet& packet) {
    if (socket_->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Socket is not connected. Cannot send packet." << socketHost << ":" << socketPort;
        return false;
    }

    int totalSize = sizeof(sensor_header) + packet.header.length;
    QByteArray buffer(reinterpret_cast<const char*>(&packet), totalSize);

    qint64 bytesWritten = socket_->write(buffer);
    socket_->flush();
    return socket_->waitForBytesWritten() && bytesWritten == totalSize;
}

void Tcpsocket::onConnected() {
    qDebug() << "Verbonden met server!";
    timer_.start(5000);
}

void Tcpsocket::onDisconnected() {
    qDebug() << "Verbinding met server verbroken.";
    timer_.stop();
}

void Tcpsocket::onError(QAbstractSocket::SocketError socketError) {
    Q_UNUSED(socketError);
    qDebug() << "Socket fout:" << socket_->errorString();
}

void Tcpsocket::onReadyRead(){
    QTcpSocket* client = qobject_cast<QTcpSocket*>(sender());
    if(!client){
        return;
    }

    QByteArray& buffer = socketBuffers_[client];
    buffer.append(client->readAll());

    while(true){
        //Hebben we minstens genoeg voor de header?
        if(buffer.size() < static_cast<int>(sizeof(sensor_header))){
            return;
        }

        //lees header
        sensor_header header;
        memcpy(&header, buffer.constData(), sizeof(sensor_header));
        int fullPacketSize = sizeof(sensor_header) + header.length;

        //hebben we al genoeg voor het hele pakket?
        if(buffer.size() < fullPacketSize){
            return;
        }

        //hebben we een volledig pakket
        QByteArray packetData = buffer.mid(0, fullPacketSize);
        buffer.remove(0, fullPacketSize);

        //zet om naar struct
        sensor_packet packet;
        memcpy(&packet, packetData.constData(), fullPacketSize);

        // emit signaal naar rest
        if (header.ptype == PacketType::DASHBOARD_RESPONSE) {
            qDebug() << "Dashboard response ontvangen.";
            qDebug() << "Ontvangen bytes:" << packetData.toHex(' ');
            emit packetReceived(packet);
        } else {
            qDebug() << "Ontvangen packet van ander type:" << static_cast<int>(packet.header.ptype);
        }
    }
}

void Tcpsocket::setSensorList(const std::vector<std::pair<SensorType, int>>& sensorList)
{
    sensorList_ = sensorList;
}

void Tcpsocket::requestSensorPacket() {
    if (socket_->state() != QAbstractSocket::ConnectedState) {
        qDebug() << "Socket is not connected. Cannot send request.";
        return;
    }

    for (const auto& sensor : sensorList_) {
        sensor_packet request;
        request.header.ptype = PacketType::DASHBOARD_GET;
        request.header.length = sizeof(sensor_packet_generic);
        request.data.generic.metadata.sensor_type = static_cast<SensorType>(sensor.first);
        request.data.generic.metadata.sensor_id = static_cast<uint8_t>(sensor.second);

        qDebug() << "Verzoek verzonden voor sensor type:" << static_cast<int>(sensor.first) << "ID:" << sensor.second;

        QByteArray data(reinterpret_cast<const char*>(&request), sizeof(sensor_header) + request.header.length);
        socket_->write(data);
    }

    socket_->flush();

    qDebug() << "Sensor request verzonden.";
}

void Tcpsocket::updateConnection(const QString& newIp, int newPort) {
    if (socket_->state() != QAbstractSocket::ConnectedState) {
        socket_->disconnect();
    }
    this->socketHost = newIp;
    this->socketPort = newPort;
    connectToServer();
}
