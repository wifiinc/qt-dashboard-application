#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QColorDialog>

extern "C" {
#include "packets.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    settingsWindow = new SettingsWindow(this);

    connect(settingsWindow, &SettingsWindow::rgbSensorIdChanged, this, &MainWindow::updateRgbSensorId);
    connect(settingsWindow, &SettingsWindow::bridgeIpChanged, this, &MainWindow::updateBridgeIp);
}

MainWindow::~MainWindow()
{
    delete settingsWindow;
    delete ui;
}

void MainWindow::on_btnOpenColorPicker_clicked()
{
    QColor gekozenKleur = QColorDialog::getColor(huidigeKleur, this, "Kies een kleur");

    if (gekozenKleur.isValid()) {
        huidigeKleur = gekozenKleur;
        qDebug() << "Gekozen kleur:" << huidigeKleur;
    }
}

void MainWindow::on_btnSetColor_clicked()
{
    if (!ui->checkBoxLightOn->isChecked()) {
        huidigeKleur = QColor(0, 0, 0); // Lamp uit
    }

    applyLightColor(huidigeKleur.red(), huidigeKleur.green(), huidigeKleur.blue());

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_header) + sizeof(sensor_packet_rgb_light);

    pakket.data.rgb_light.metadata.sensor_type = SensorType::RGB_LIGHT;
    pakket.data.rgb_light.metadata.sensor_id = static_cast<uint8_t>(rgbSensorId);
    pakket.data.rgb_light.red_state = static_cast<uint8_t>(huidigeKleur.red());
    pakket.data.rgb_light.green_state = static_cast<uint8_t>(huidigeKleur.green());
    pakket.data.rgb_light.blue_state = static_cast<uint8_t>(huidigeKleur.blue());

    QByteArray data(reinterpret_cast<const char*>(&pakket), pakket.header.length);
    verzendPakket(data);
}

void MainWindow::verzendPakket(const QByteArray& data)
{
    qDebug() << "Pakket verzonden (" << data.size() << " bytes):";
    qDebug() << data.toHex(' ');
    qDebug() << "Target IP (bridge):" << bridgeIp << ":" << bridgePort;
}

void MainWindow::applyLightColor(int r, int g, int b)
{
    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;
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
}

void MainWindow::updateBridgeIp(const QString& newIp, int newPort)
{
    bridgeIp = newIp;
    bridgePort = newPort;
    qDebug() << "Bridge IP aangepast naar:" << bridgeIp;
    qDebug() << "Bridge Port aangepast naar:" << bridgePort;
}
