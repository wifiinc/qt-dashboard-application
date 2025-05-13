#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client("192.168.0.100", 8080)// IP of Raspberry Pi
    , Server(8080)
{
    ui->setupUi(this);

    sensor_packet packet;
    if(!Server.startListening()){

    }
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::serverluisteren(){
    if(!Server.startListening()){
        return -1;
    }

QObject:connect(&Server, &TcpServerHandler::packetReceived, [](const sensor_packet& packet){
    qDebug() << "Verwerkt pakket van sensor ID" << packet.data.generic.metadata.sensor_id;
    });
}
