#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);


    ui->spinBoxRgbSensorId->setRange(1, 255);
    ui->spinBoxRgbSensorId->setValue(1);


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
