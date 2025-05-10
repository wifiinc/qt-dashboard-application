#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ui->spinBoxSensorId->setRange(1, 255);


    ui->spinBoxSensorId->setValue(1);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_btnSave_clicked()
{
    int newSensorId = ui->spinBoxSensorId->value();
    emit sensorIDChanged(newSensorId);
    close();
}
