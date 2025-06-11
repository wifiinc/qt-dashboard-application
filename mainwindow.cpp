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
  QSettings settings(QSettings::IniFormat, QSettings::UserScope,
                     "LBAutomatisatie", "DashboardApp");

  qDebug() << settings.fileName();

  loadSettings();
  writeSettings();

  ui->setupUi(this);

  requestluisteren();

  this->setWindowTitle("L&B Automatisatie Dashboard");
  this->setWindowIcon(QIcon(":/icon.png"));

  QAction* settingsAction = new QAction("Instellingen", this);
  connect(settingsAction, &QAction::triggered, this,
          &MainWindow::on_btnOpenSettings_clicked);

  mapWindow = new MapWindow(this);

  QAction *kaartAction = new QAction("Interactieve kaart", this);
  connect(kaartAction, &QAction::triggered, this, [=]() {
      if (mapWindow) {
          mapWindow->show();
          mapWindow->raise();
          mapWindow->activateWindow();
      }
  });

#ifndef Q_OS_MAC
  ui->menubar->addAction(settingsAction);
  ui->menubar->addAction(kaartAction);
#else
  QMenu* fileMenu = new QMenu("File", this);
  fileMenu->addAction(settingsAction);
  fileMenu->addAction(kaartAction);
  ui->menubar->addMenu(fileMenu);
#endif

  settingsWindow = new SettingsWindow(this);

  connect(settingsWindow, &SettingsWindow::rgbSensorIdChanged, this,
          &MainWindow::updateRgbSensorId);
  connect(settingsWindow, &SettingsWindow::bridgeIpChanged, this,
          &MainWindow::updateBridgeIp);
  connect(settingsWindow, &SettingsWindow::tafelSensorIdChanged, this,
          &MainWindow::updateTafelSensorId);
  connect(settingsWindow, &SettingsWindow::updateSensorList, this,
          &MainWindow::setSensorList);




  QString stylesheet =
      QString("background-color: %1;").arg(huidigekleurRGBLed.name());
  ui->rgbLightColorWidget->setStyleSheet(stylesheet);
}


MainWindow::~MainWindow() {
  delete settingsWindow;
  delete ui;
}

void MainWindow::setSensorList()
{
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
  connect(&client, &Tcpsocket::packetReceived, [](const sensor_packet& packet) {
    qDebug() << "Packet ontvangen van sensor ID:"
             << packet.data.generic.metadata.sensor_id;
  });

  connect(
      &client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT) {
          if (packet.data.generic.metadata.sensor_id == tafel1SensorId) {
            tafel1State = packet.data.light.target_state == 1 ? true : false;
            ui->tafel1Status->setText(tafel1State ? "Tafel staat AAN"
                                                  : "Tafel staat UIT");
            if (mapWindow) {
              mapWindow->updateDeviceStatus(0, tafel1State ? "Lamp 1: AAN" : "Lamp 1: UIT");
            }
          }
        }
      });

  connect(
      &client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT) {
          if (packet.data.generic.metadata.sensor_id == tafel2SensorId) {
            tafel2State = packet.data.light.target_state == 1 ? true : false;
            ui->tafel2Status->setText(tafel2State ? "Tafel staat AAN"
                                                  : "Tafel staat UIT");
            if (mapWindow) {
              mapWindow->updateDeviceStatus(1, tafel2State ? "Lamp 2: AAN" : "Lamp 2: UIT");
            }
          }
        }
      });

  connect(
      &client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::LIGHT) {
          if (packet.data.generic.metadata.sensor_id == tafel3SensorId) {
            tafel3State = packet.data.light.target_state == 1 ? true : false;
            ui->tafel3Status->setText(tafel3State ? "Tafel staat AAN"
                                                  : "Tafel staat UIT");
            if (mapWindow) {
              mapWindow->updateDeviceStatus(2, tafel3State ? "Lamp 3: AAN" : "Lamp 3: UIT");
            }
          }
        }
      });

  connect(
      &client, &Tcpsocket::packetReceived, [this](const sensor_packet& packet) {
        if (packet.data.generic.metadata.sensor_type == SensorType::RGB_LIGHT) {
          if (packet.data.generic.metadata.sensor_id == rgbSensorId) {
            int r = packet.data.rgb_light.red_state;
            int g = packet.data.rgb_light.green_state;
            int b = packet.data.rgb_light.blue_state;
            QString rgbText = QString("Rgbww : R:%1 G:%2 B:%3").arg(r).arg(g).arg(b);
            if (mapWindow) {
              mapWindow->updateDeviceStatus(3, rgbText);
            }
          }
        }
      });
}

void MainWindow::on_btnOpenSettings_clicked() {
  settingsWindow->setRgbSensorId(rgbSensorId);

  settingsWindow->setBridgeIp(bridgeIp);
  settingsWindow->setBridgePort(bridgePort);
  settingsWindow->setTafelSensorId(tafel1SensorId, tafel2SensorId,
                                   tafel3SensorId);

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
  huidigekleurRGBLed = QColorDialog::getColor(huidigekleurRGBLed, this,
                                              "Instellen kleur RGB led");
  QString stylesheet =
      QString("background-color: %1;").arg(huidigekleurRGBLed.name());
  ui->rgbLightColorWidget->setStyleSheet(stylesheet);
}

void MainWindow::on_saveRGBBtn_clicked() {
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
  ui->tafel1Status->setText(tafel1State ? "Tafel staat AAN"
                                        : "Tafel staat UIT");

  sensor_packet pakket;
  pakket.header.ptype = PacketType::DASHBOARD_POST;
  pakket.header.length = sizeof(sensor_packet_light);
  pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
  pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(tafel1SensorId);
  pakket.data.light.target_state = tafel1State ? 1 : 0;
  if (mapWindow) {
      mapWindow->updateDeviceStatus(0, tafel1State ? "Lamp 1: AAN" : "Lamp 1: UIT");
  }


  client.sendPacket(pakket);
}

void MainWindow::on_tafel2Toggle_clicked() {
  tafel2State = !tafel2State;
  ui->tafel2Status->setText(tafel2State ? "Tafel staat AAN"
                                        : "Tafel staat UIT");

  sensor_packet pakket;
  pakket.header.ptype = PacketType::DASHBOARD_POST;
  pakket.header.length = sizeof(sensor_packet_light);
  pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
  pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(tafel2SensorId);
  pakket.data.light.target_state = tafel2State ? 1 : 0;
  if (mapWindow) {
      mapWindow->updateDeviceStatus(1, tafel2State ? "Lamp 2: AAN" : "Lamp 2: UIT");
  }

  client.sendPacket(pakket);
}

void MainWindow::on_tafel3Toggle_clicked() {
  tafel3State = !tafel3State;
  ui->tafel3Status->setText(tafel3State ? "Tafel staat AAN"
                                        : "Tafel staat UIT");
  if (mapWindow) {
      mapWindow->updateDeviceStatus(2, tafel3State ? "Lamp 3: AAN" : "Lamp 3: UIT");
  }

  sensor_packet pakket;
  pakket.header.ptype = PacketType::DASHBOARD_POST;
  pakket.header.length = sizeof(sensor_packet_light);
  pakket.data.light.metadata.sensor_type = SensorType::LIGHT;
  pakket.data.light.metadata.sensor_id = static_cast<uint8_t>(tafel3SensorId);
  pakket.data.light.target_state = tafel3State ? 1 : 0;

  client.sendPacket(pakket);
}

