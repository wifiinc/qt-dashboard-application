#include "mainwindow.h"

#include <QColorDialog>
#include <QDebug>
#include <QSettings>
#include "./ui_mainwindow.h"

extern "C" {
#include "packets.h"
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), client("127.0.0.1", 5000) {
    ui->setupUi(this);
    setWindowTitle("Lichtkrant Dashboard");

    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "LBAutomatisatie", "DashboardApp");

    qDebug() << settings.fileName();

    loadSettings();
    writeSettings();
    requestluisteren();

    this->setWindowTitle("L&B Automatisatie Dashboard");
    this->setWindowIcon(QIcon(":/icon.png"));

    QAction* settingsAction = new QAction("Instellingen", this);
    connect(settingsAction, &QAction::triggered, this,
            &MainWindow::on_btnOpenSettings_clicked);
    ui->menubar->addAction(settingsAction);

    settingsWindow = new SettingsWindow(this);
    connect(settingsWindow, &SettingsWindow::rgbSensorIdChanged, this, &MainWindow::updateRgbSensorId);
    connect(settingsWindow, &SettingsWindow::bridgeIpChanged, this, &MainWindow::updateBridgeIp);
    connect(settingsWindow, &SettingsWindow::tafelSensorIdChanged, this, &MainWindow::updateTafelSensorId);
    connect(settingsWindow, &SettingsWindow::updateSensorList, this, &MainWindow::setSensorList);

    QString stylesheet = QString("background-color: %1;").arg(huidigekleurRGBLed.name());
    ui->rgbLightColorWidget->setStyleSheet(stylesheet);

    connect(ui->btnVerzendLichtkrant, &QPushButton::clicked,
            this, &MainWindow::on_btnVerzendLichtkrant_clicked);
}

MainWindow::~MainWindow() {
    delete settingsWindow;
    delete ui;
}

void MainWindow::setSensorList() {
    client.setSensorList({
                          {SensorType::LIGHT, rgbSensorId},
                          {SensorType::RGB_LIGHT, rgbSensorId},
                          {SensorType::LIGHT, tafel1SensorId},
                          {SensorType::LIGHT, tafel2SensorId},
                          {SensorType::LIGHT, tafel3SensorId},
                          });
}

void MainWindow::writeSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "LBAutomatisatie", "DashboardApp");

    settings.beginGroup("Connection");
    settings.setValue("bridgeIp", bridgeIp);
    settings.setValue("bridgePort", bridgePort);
    settings.endGroup();

    settings.beginGroup("Sensors");
    settings.setValue("rgbSensorId", rgbSensorId);
    settings.setValue("tafel1SensorId", tafel1SensorId);
    settings.setValue("tafel2SensorId", tafel2SensorId);
    settings.setValue("tafel3SensorId", tafel3SensorId);
    settings.endGroup();
}

void MainWindow::loadSettings() {
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                       "LBAutomatisatie", "DashboardApp");

    settings.beginGroup("Connection");
    bridgeIp = settings.value("bridgeIp", "127.0.0.1").toString();
    bridgePort = settings.value("bridgePort", 5000).toInt();
    settings.endGroup();

    settings.beginGroup("Sensors");
    rgbSensorId = settings.value("rgbSensorId", 1).toInt();
    tafel1SensorId = settings.value("tafel1SensorId", 1).toInt();
    tafel2SensorId = settings.value("tafel2SensorId", 1).toInt();
    tafel3SensorId = settings.value("tafel3SensorId", 1).toInt();
    settings.endGroup();
}

void MainWindow::requestluisteren() {
    client.connectToServer();

    auto updateStatus = [this](int sensorId, bool& stateVar, QLabel* label) {
        connect(&client, &Tcpsocket::packetReceived, [this, sensorId, &stateVar, label](const sensor_packet& packet) {
            if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT &&
                packet.data.generic.metadata.sensor_id == sensorId) {
                stateVar = packet.data.light.target_state == 1;
                label->setText(stateVar ? "Tafel staat AAN" : "Tafel staat UIT");
            }
        });
    };

    updateStatus(tafel1SensorId, tafel1State, ui->tafel1Status);
    updateStatus(tafel2SensorId, tafel2State, ui->tafel2Status);
    updateStatus(tafel3SensorId, tafel3State, ui->tafel3Status);
}

void MainWindow::on_btnOpenSettings_clicked() {
    settingsWindow->setRgbSensorId(rgbSensorId);
    settingsWindow->setBridgeIp(bridgeIp);
    settingsWindow->setBridgePort(bridgePort);
    settingsWindow->setTafelSensorId(tafel1SensorId, tafel2SensorId, tafel3SensorId);
    settingsWindow->show();
}

void MainWindow::updateRgbSensorId(int newID) {
    qDebug() << "Sensor ID aangepast naar:" << newID;
    rgbSensorId = newID;
    writeSettings();
}

void MainWindow::updateBridgeIp(const QString& newIp, int newPort) {
    bridgeIp = newIp;
    bridgePort = newPort;
    client.updateConnection(newIp, newPort);
    qDebug() << "Bridge IP aangepast naar:" << bridgeIp;
    qDebug() << "Bridge Port aangepast naar:" << bridgePort;
    writeSettings();
}

void MainWindow::updateTafelSensorId(int tafel1, int tafel2, int tafel3) {
    tafel1SensorId = tafel1;
    tafel2SensorId = tafel2;
    tafel3SensorId = tafel3;
    writeSettings();
}

void MainWindow::on_changeRGBBtn_clicked() {
    huidigekleurRGBLed = QColorDialog::getColor(huidigekleurRGBLed, this, "Instellen kleur RGB led");
    QString stylesheet = QString("background-color: %1;").arg(huidigekleurRGBLed.name());
    ui->rgbLightColorWidget->setStyleSheet(stylesheet);
}

void MainWindow::on_saveRGBBtn_clicked() {
    int r = huidigekleurRGBLed.red();
    int g = huidigekleurRGBLed.green();
    int b = huidigekleurRGBLed.blue();

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

void MainWindow::on_checkBoxLightOn_toggled(bool checked) {
    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_light);
    pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
    pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(rgbSensorId);
    pakket.data.light.target_state = checked ? 1 : 0;

    client.sendPacket(pakket);
}

void MainWindow::on_tafel1Toggle_clicked() {
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

void MainWindow::on_tafel2Toggle_clicked() {
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

void MainWindow::on_tafel3Toggle_clicked() {
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

void MainWindow::on_btnVerzendLichtkrant_clicked() {
    QString boodschap = ui->lineEditLichtkrant->text().trimmed();
    if (boodschap.isEmpty()) {
        qDebug() << "Geen tekst ingevoerd voor lichtkrant.";
        return;
    }

    qDebug() << "Lichtkrant boodschap verzenden:" << boodschap;

    sensor_packet pakket;
    pakket.header.length = sizeof(sensor_packet_lichtkrant);
    pakket.header.ptype = PacketType::DASHBOARD_POST;

    pakket.data.lichtkrant.metadata.sensor_type = SensorType::LICHTKRANT;
    pakket.data.lichtkrant.metadata.sensor_id = 255;
    std::strncpy(pakket.data.lichtkrant.text, boodschap.toStdString().c_str(), sizeof(pakket.data.lichtkrant.text) - 1);
    pakket.data.lichtkrant.text[sizeof(pakket.data.lichtkrant.text) - 1] = '\0';
    client.sendPacket(pakket);
}
