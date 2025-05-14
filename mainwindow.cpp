#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

// Toevoegen voor toegang tot de structs
extern "C" {
#include "packets.h"
}

// Constructor van MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) 
    , client("192.168.0.100", 8080)  // IP of Raspberry Pi
    {

    // Koppelt de UI-bestand aan deze class
    ui->setupUi(this);

    // RGB-sliders instellen op bereik 0–255
    ui->sliderR->setRange(0, 255);
    ui->sliderG->setRange(0, 255);
    ui->sliderB->setRange(0, 255);
    requestluisteren();
}

// Destructor: zorgt ervoor dat geheugen van de UI wordt opgeruimd
MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::requestluisteren(){
    request.connectToServer("127.0.0.1", 8080);
    QObject::connect(&request, &Receivetcpsocket::packetReceived, [](const sensor_packet& packet) {
        qDebug() << "Packet ontvangen van sensor ID:"
                 << packet.data.generic.metadata.sensor_id;
    });
}

// Wordt uitgevoerd als op 'Stel kleur in' wordt geklikt
void MainWindow::on_btnSetColor_clicked() {
    int r = 0, g = 0, b = 0;

    if (ui->checkBoxLightOn->isChecked()) {
        r = ui->sliderR->value();
        g = ui->sliderG->value();
        b = ui->sliderB->value();
    }

    // Pas de kleur toe in de GUI
    applyLightColor(r, g, b);

    // Bouw het sensor_packet voor RGB
    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_header) + sizeof(sensor_packet_rgb_light);

    pakket.data.rgb_light.metadata.sensor_type = SensorType::RGB_LIGHT;
    pakket.data.rgb_light.metadata.sensor_id = 1;

    pakket.data.rgb_light.red_state = static_cast<uint8_t>(r);
    pakket.data.rgb_light.green_state = static_cast<uint8_t>(g);
    pakket.data.rgb_light.blue_state = static_cast<uint8_t>(b);

    // Verzend Tcpsocket.h
    client.sendPacket(pakket);
}

// Simuleert het verzenden van het pakket
void MainWindow::verzendPakket(const QByteArray& data) {
    qDebug() << "Pakket verzonden (" << data.size() << " bytes):";
    qDebug() << data.toHex(' ');
}

// RGB kleur lokaal toepassen
void MainWindow::applyLightColor(int r, int g, int b) {
    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;
}
