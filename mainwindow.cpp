#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client("192.168.0.100", 8080)  // IP of Raspberry Pi
{
    ui->setupUi(this);

    sensor_packet packet;
}

MainWindow::~MainWindow() {
    delete ui;
}


