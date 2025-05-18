#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
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

    int tafel1SensorId = ui->spinBoxTafel1SensorId->value();
    int tafel2SensorId = ui->spinBoxTafel2SensorId->value();
    int tafel3SensorId = ui->spinBoxTafel3SensorId->value();

    emit rgbSensorIdChanged(newSensorId);
    emit bridgeIpChanged(newIp, newPort);
    emit tafelSensorIdChanged(tafel1SensorId, tafel2SensorId, tafel3SensorId);
    emit updateSensorList();

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

void SettingsWindow::setTafelSensorId(int tafel1, int tafel2, int tafel3)
{
    ui->spinBoxTafel1SensorId->setValue(tafel1);
    ui->spinBoxTafel2SensorId->setValue(tafel2);
    ui->spinBoxTafel3SensorId->setValue(tafel3);
}
