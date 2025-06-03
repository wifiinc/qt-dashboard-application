#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>

extern "C" {
#include "packets.h"
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , client("127.0.0.1", 5000)
{
    ui->setupUi(this);
    client.connectToServer();

    connect(ui->btnVerzendLichtkrant, &QPushButton::clicked,
            this, &MainWindow::on_btnVerzendLichtkrant_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnVerzendLichtkrant_clicked()
{
    QString boodschap = ui->lineEditLichtkrant->text().trimmed();

    if (boodschap.isEmpty()) {
        QMessageBox::warning(this, "Leeg bericht", "Voer een boodschap in om te verzenden.");
        return;
    }

    if (boodschap.length() >= sizeof(sensor_packet_lichtkrant::text)) {
        QMessageBox::warning(this, "Te lang", "De boodschap is te lang voor verzending.");
        return;
    }

    sensor_packet pakket;
    pakket.header.ptype = PacketType::DASHBOARD_POST;
    pakket.header.length = sizeof(sensor_packet_lichtkrant);
    pakket.data.lichtkrant.metadata.sensor_type = SensorType::LICHTKRANT;
    pakket.data.lichtkrant.metadata.sensor_id = 99;

    std::memset(pakket.data.lichtkrant.text, 0, sizeof(pakket.data.lichtkrant.text));
    std::strncpy(pakket.data.lichtkrant.text, boodschap.toUtf8().constData(),
                 sizeof(pakket.data.lichtkrant.text) - 1);

    if (client.sendPacket(pakket)) {
        qDebug() << "Lichtkrant verzonden:" << boodschap;
        ui->lineEditLichtkrant->clear();
    } else {
        QMessageBox::critical(this, "Verzenden mislukt", "Kon geen verbinding maken met server.");
    }
}
