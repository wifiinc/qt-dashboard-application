#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

// Toevoegen voor toegang tot de structs
extern "C" {
#include "packets.h"
}

// Constructor van MainWindow
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

    // Koppelt de UI-bestand aan deze class
    ui->setupUi(this);

    // RGB-sliders instellen op bereik 0–255
    ui->sliderR->setRange(0, 255);
    ui->sliderG->setRange(0, 255);
    ui->sliderB->setRange(0, 255);

    // Dummy sensorwaarden tonen op LCD displays
    ui->lcdTemp->display(21.3);
    ui->lcdCO2->display(580);
    ui->lcdHumidity->display(42);

    // Initieel menu en statuslabels instellen
    ui->labelCurrentMenu->setText("-");
    ui->labelVentilatorStatus->setText("Uit");
    ui->labelLichtkrant->setText("Welkom");
    ui->labelTafelStatus->setText("Tafel staat UIT");
}

// Destructor: zorgt ervoor dat geheugen van de UI wordt opgeruimd
MainWindow::~MainWindow() {
    delete ui;
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
    pakket.header.ptype = PacketType::DATA;
    pakket.header.length = sizeof(sensor_header) + sizeof(sensor_packet_rgb_light);

    pakket.data.rgb_light.metadata.sensor_type = SensorType::LIGHT;
    pakket.data.rgb_light.metadata.sensor_id = 1;

    pakket.data.rgb_light.red_state = static_cast<uint8_t>(r);
    pakket.data.rgb_light.green_state = static_cast<uint8_t>(g);
    pakket.data.rgb_light.blue_state = static_cast<uint8_t>(b);

    // Zet om naar QByteArray
    QByteArray data(reinterpret_cast<const char*>(&pakket), pakket.header.length);

    // Verzend via dummy functie
    verzendPakket(data);
}

// TCP
void MainWindow::verzendPakket(const QByteArray& data) {
    qDebug() << "Pakket verzonden (" << data.size() << " bytes):";
    qDebug() << data.toHex(' ');
}

// Deze functie simuleert het toepassen van de RGB kleur
void MainWindow::applyLightColor(int r, int g, int b) {
    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;
}

// Wordt uitgevoerd als je op 'Publiceer Menu' klikt
void MainWindow::on_btnPublishMenu_clicked() {
    QString menu = ui->textEditMenuInput->toPlainText();
    ui->labelCurrentMenu->setText(menu);
}

// Knop om ventilator AAN te zetten
void MainWindow::on_btnVentilatorAan_clicked() {
    ui->labelVentilatorStatus->setText("Aan");
    qDebug() << "Ventilator aan";
}

// Knop om ventilator UIT te zetten
void MainWindow::on_btnVentilatorUit_clicked() {
    ui->labelVentilatorStatus->setText("Uit");
    qDebug() << "Ventilator uit";
}

// Zet een tafelstatus aan/uit (toggle)
void MainWindow::on_btnTafelToggle_clicked() {
    QString current = ui->labelTafelStatus->text();

    if (current.contains("UIT")) {
        ui->labelTafelStatus->setText("Tafel staat AAN");
    } else {
        ui->labelTafelStatus->setText("Tafel staat UIT");
    }
}

// Update tekst van de lichtkrant
void MainWindow::on_btnUpdateLichtkrant_clicked() {
    QString tekst = ui->lineEditLichtkrant->text();
    ui->labelLichtkrant->setText(tekst);
}
