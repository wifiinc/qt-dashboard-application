#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QColorDialog>

extern "C" {
#include "packets.h"
}

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent), ui(new Ui::MainWindow), client("127.0.0.1", 5000)
{
    ui->setupUi(this);

    requestluisteren();

    this->setWindowTitle("L&B Automatisatie Dashboard");
    this->setWindowIcon(QIcon(":/icon.png"));

    settingsWindow = new SettingsWindow(this);

    connect(settingsWindow, &SettingsWindow::rgbSensorIdChanged, this, &MainWindow::updateRgbSensorId);
    connect(settingsWindow, &SettingsWindow::bridgeIpChanged, this, &MainWindow::updateBridgeIp);
    connect(settingsWindow, &SettingsWindow::tafelSensorIdChanged, this, &MainWindow::updateTafelSensorId);
}

MainWindow::~MainWindow()
{
    delete settingsWindow;
    delete ui;
}

void MainWindow::requestluisteren()
{
    client.connectToServer();
    connect(&client, &Tcpsocket::packetReceived, [](const sensor_packet& packet) {
        qDebug() << "Packet ontvangen van sensor ID:" << packet.data.generic.metadata.sensor_id;
    });

    connect(&client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT) {
            if (packet.data.generic.metadata.sensor_id == tafel1SensorId) {
                tafel1State = packet.data.light.target_state == 1 ? true : false;
                ui->tafel1Status->setText(tafel1State ? "Tafel staat AAN" : "Tafel staat UIT");
            }
        }
    });

    connect(&client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT) {
            if (packet.data.generic.metadata.sensor_id == tafel2SensorId) {
                tafel2State = packet.data.light.target_state == 1 ? true : false;
                ui->tafel2Status->setText(tafel2State ? "Tafel staat AAN" : "Tafel staat UIT");
            }
        }
    });

    connect(&client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT) {
            if (packet.data.generic.metadata.sensor_id == tafel3SensorId) {
                tafel3State = packet.data.light.target_state == 1 ? true : false;
                ui->tafel3Status->setText(tafel3State ? "Tafel staat AAN" : "Tafel staat UIT");
            }
        }
    });
}

void MainWindow::on_btnOpenSettings_clicked()
{
    settingsWindow->setRgbSensorId(rgbSensorId);
    settingsWindow->setBridgeIp(bridgeIp);
    settingsWindow->setBridgePort(bridgePort);
    settingsWindow->show();
}

void MainWindow::updateRgbSensorId(int newID)
{
    qDebug() << "Sensor ID aangepast naar:" << newID;
    rgbSensorId = newID;

    client.setSensorList({
      {SensorType::LIGHT, rgbSensorId},
      {SensorType::RGB_LIGHT, rgbSensorId},
      {SensorType::LIGHT, tafel1SensorId},
      {SensorType::LIGHT, tafel2SensorId},
      {SensorType::LIGHT, tafel3SensorId},
    });
}

void MainWindow::updateBridgeIp(const QString& newIp, int newPort)
{
    bridgeIp = newIp;
    bridgePort = newPort;
    client.updateConnection(newIp, newPort);
    qDebug() << "Bridge IP aangepast naar:" << bridgeIp;
    qDebug() << "Bridge Port aangepast naar:" << bridgePort;
}

void MainWindow::updateTafelSensorId(int tafel1, int tafel2, int tafel3)
{
    tafel1SensorId = tafel1;
    tafel2SensorId = tafel2;
    tafel3SensorId = tafel1;

    client.setSensorList({
        {SensorType::LIGHT, rgbSensorId},
        {SensorType::RGB_LIGHT, rgbSensorId},
        {SensorType::LIGHT, tafel1SensorId},
        {SensorType::LIGHT, tafel2SensorId},
        {SensorType::LIGHT, tafel3SensorId},
    });
}

void MainWindow::on_changeRGBBtn_clicked()
{
    huidigekleurRGBLed = QColorDialog::getColor(huidigekleurRGBLed, this, "Instellen kleur RGB led");
    QString stylesheet = QString("background-color: %1;").arg(huidigekleurRGBLed.name());
    ui->rgbLightColorWidget->setStyleSheet(stylesheet);
}


void MainWindow::on_saveRGBBtn_clicked()
{
    int r = 0, g = 0, b = 0;

    r = huidigekleurRGBLed.red();
    g = huidigekleurRGBLed.green();
    b = huidigekleurRGBLed.blue();

    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_rgb_light);

    pakket.data.rgb_light.metadata.sensor_type = SensorType::RGB_LIGHT;
    pakket.data.rgb_light.metadata.sensor_id = static_cast<uint8_t>(rgbSensorId);
    pakket.data.rgb_light.red_state = static_cast<uint8_t>(r);
    pakket.data.rgb_light.green_state = static_cast<uint8_t>(g);
    pakket.data.rgb_light.blue_state = static_cast<uint8_t>(b);

    client.sendPacket(pakket);
}


void MainWindow::on_checkBoxLightOn_toggled(bool checked)
{
    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_light);
    pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
    pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(rgbSensorId);
    pakket.data.light.target_state = checked ? 1 : 0;

    client.sendPacket(pakket);
}

void MainWindow::on_tafel1Toggle_clicked()
{
    tafel1State = !tafel1State;
    ui->tafel1Status->setText(tafel1State ? "Tafel staat AAN" : "Tafel staat UIT");

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_light);
    pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
    pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(tafel1SensorId);
    pakket.data.light.target_state = tafel1State ? 1 : 0;

    client.sendPacket(pakket);
}

void MainWindow::on_tafel2Toggle_clicked()
{
    tafel2State = !tafel2State;
    ui->tafel2Status->setText(tafel2State ? "Tafel staat AAN" : "Tafel staat UIT");

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_light);
    pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
    pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(tafel2SensorId);
    pakket.data.light.target_state = tafel2State ? 1 : 0;

    client.sendPacket(pakket);
}

void MainWindow::on_tafel3Toggle_clicked()
{
    tafel3State = !tafel3State;
    ui->tafel3Status->setText(tafel3State ? "Tafel staat AAN" : "Tafel staat UIT");

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_light);
    pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
    pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(tafel3SensorId);
    pakket.data.light.target_state = tafel3State ? 1 : 0;

    client.sendPacket(pakket);
}
