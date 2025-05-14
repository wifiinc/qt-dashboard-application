#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    ui->spinBoxRgbSensorId->setRange(1, 255);
    ui->spinBoxRgbSensorId->setValue(1);

    ui->spinBoxBridgePort->setRange(1, 65535);
    ui->spinBoxBridgePort->setValue(5000);

    ui->lineEditBridgeIp->setText("10.0.0.2");
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_btnSave_clicked()
{
    int newSensorId = ui->spinBoxRgbSensorId->value();
    QString newIp = ui->lineEditBridgeIp->text();
    int newPort = ui->spinBoxBridgePort->value();

    emit rgbSensorIdChanged(newSensorId);
    emit bridgeIpChanged(newIp, newPort);
    close();
}


void SettingsWindow::setRgbSensorId(int id)
{
    ui->spinBoxRgbSensorId->setValue(id);
}

void SettingsWindow::setBridgeIp(const QString &ip)
{
    ui->lineEditBridgeIp->setText(ip);
}

void SettingsWindow::setBridgePort(int port)
{
    ui->spinBoxBridgePort->setValue(port);
}
