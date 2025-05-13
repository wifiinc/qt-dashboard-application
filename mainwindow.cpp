#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

extern "C" {
#include "packets.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sliderR->setRange(0, 255);
    ui->sliderG->setRange(0, 255);
    ui->sliderB->setRange(0, 255);

    settingsWindow = new SettingsWindow(this);
    connect(settingsWindow, &SettingsWindow::rgbSensorIdChanged, this, &MainWindow::updateRgbSensorId);
    connect(settingsWindow, &SettingsWindow::bridgeIpChanged, this, &MainWindow::updateBridgeIp);  // Nieuw
}

MainWindow::~MainWindow()
{
    delete settingsWindow;
    delete ui;
}

void MainWindow::on_btnSetColor_clicked()
{
    int r = 0, g = 0, b = 0;

    if (ui->checkBoxLightOn->isChecked()) {
        r = ui->sliderR->value();
        g = ui->sliderG->value();
        b = ui->sliderB->value();
    }

    applyLightColor(r, g, b);

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_header) + sizeof(sensor_packet_rgb_light);

    pakket.data.rgb_light.metadata.sensor_type = SensorType::RGB_LIGHT;
    pakket.data.rgb_light.metadata.sensor_id = static_cast<uint8_t>(rgbSensorId);
    pakket.data.rgb_light.red_state = static_cast<uint8_t>(r);
    pakket.data.rgb_light.green_state = static_cast<uint8_t>(g);
    pakket.data.rgb_light.blue_state = static_cast<uint8_t>(b);

    QByteArray data(reinterpret_cast<const char*>(&pakket), pakket.header.length);
    verzendPakket(data);
}

void MainWindow::verzendPakket(const QByteArray& data)
{
    qDebug() << "Pakket verzonden (" << data.size() << " bytes):";
    qDebug() << data.toHex(' ');

    // Later kun je hier bridgeIp gebruiken voor socketcommunicatie
    qDebug() << "Target IP (bridge):" << bridgeIp;
}

void MainWindow::applyLightColor(int r, int g, int b)
{
    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;
}

void MainWindow::on_btnOpenSettings_clicked()
{
    settingsWindow->show();
}

void MainWindow::updateRgbSensorId(int newID)
{
    qDebug() << "Sensor ID aangepast naar:" << newID;
    rgbSensorId = newID;
}

void MainWindow::updateBridgeIp(const QString& newIp)
{
    bridgeIp = newIp;
    qDebug() << "Bridge IP aangepast naar:" << bridgeIp;
}
