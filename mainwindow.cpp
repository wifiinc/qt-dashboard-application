#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client("192.168.0.100", 8080)  // IP of Raspberry Pi
{
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_onButton_clicked() {
    QString res = client.sendCommand("ON");
    ui->statusLabel->setText("Light is: " + res);
}

void MainWindow::on_offButton_clicked() {
    QString res = client.sendCommand("OFF");
    ui->statusLabel->setText("Light is: " + res);
}
