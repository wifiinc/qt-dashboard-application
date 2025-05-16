#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QColorDialog>
#include <QSettings>

extern "C" {
#include "packets.h"
}

MainWindow::MainWindow(QWidget *parent)

    : QMainWindow(parent), ui(new Ui::MainWindow), client("127.0.0.1", 5000)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LBAutomatisatie", "DashboardApp");

    qDebug() << settings.fileName();

    loadSettings();
    writeSettings();

    ui->setupUi(this);

    requestluisteren();

    this->setWindowTitle("L&B Automatisatie Dashboard");
    this->setWindowIcon(QIcon(":/icon.png"));

    QAction* settingsAction = new QAction("Instellingen", this);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::on_btnOpenSettings_clicked);

    ui->menubar->addAction(settingsAction);

    settingsWindow = new SettingsWindow(this);

    connect(settingsWindow, &SettingsWindow::rgbSensorIdChanged, this, &MainWindow::updateRgbSensorId);
    connect(settingsWindow, &SettingsWindow::bridgeIpChanged, this, &MainWindow::updateBridgeIp);

    QString stylesheet = QString("background-color: %1;").arg(huidigekleurRGBLed.name());
    ui->rgbLightColorWidget->setStyleSheet(stylesheet);
}

MainWindow::~MainWindow()
{
    delete settingsWindow;
    delete ui;
}


void MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LBAutomatisatie", "DashboardApp");

    settings.beginGroup("Connection");
    settings.setValue("bridgeIp", bridgeIp);
    settings.setValue("bridgePort", bridgePort);
    settings.endGroup();

    settings.beginGroup("Sensors");
    settings.setValue("rgbSensorId", rgbSensorId);
    settings.endGroup();
}

void MainWindow::loadSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "LBAutomatisatie", "DashboardApp");

    settings.beginGroup("Connection");
    bridgeIp = settings.value("bridgeIp", "127.0.0.1").toString();
    bridgePort = settings.value("bridgePort", 5000).toInt();
    settings.endGroup();

    settings.beginGroup("Sensors");
    rgbSensorId = settings.value("rgbSensorId", 1).toInt();
    settings.endGroup();
}

void MainWindow::requestluisteren(){
    client.connectToServer();
    QObject::connect(&client, &Tcpsocket::packetReceived, [](const sensor_packet& packet) {
        qDebug() << "Packet ontvangen van sensor ID:"
                 << packet.data.generic.metadata.sensor_id;
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

    writeSettings();
}

void MainWindow::updateBridgeIp(const QString& newIp, int newPort)
{
    bridgeIp = newIp;
    bridgePort = newPort;

    client.updateConnection(newIp, newPort);
    qDebug() << "Bridge IP aangepast naar:" << bridgeIp;
    qDebug() << "Bridge Port aangepast naar:" << bridgePort;

    writeSettings();
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
