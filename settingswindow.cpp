#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    // Sensor ID instellen
    ui->spinBoxRgbSensorId->setRange(1, 255);
    ui->spinBoxRgbSensorId->setValue(1);

    // Optioneel: standaard IP (optioneel aanpasbaar)
    ui->lineEditBridgeIp->setText("192.168.1.100");
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_btnSave_clicked()
{
    int newSensorId = ui->spinBoxRgbSensorId->value();
    QString newIp = ui->lineEditBridgeIp->text();

    emit rgbSensorIdChanged(newSensorId);
    emit bridgeIpChanged(newIp);
    close();
}
