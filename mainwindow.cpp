#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client("192.168.0.100", 8080)// IP of Raspberry Pi
{
    ui->setupUi(this);

    sensor_packet packet;
    requestluisteren();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::requestluisteren(){
    request.connectToServer("127.0.0.1", 8080);
    QObject::connect(&request, &Receivetcpsocket::packetReceived, [](const sensor_packet& packet) {
        qDebug() << "Packet ontvangen van sensor ID:"
                 << packet.data.generic.metadata.sensor_id;
    });
}
