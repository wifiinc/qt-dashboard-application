#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Sliders
    ui->sliderR->setRange(0, 255);
    ui->sliderG->setRange(0, 255);
    ui->sliderB->setRange(0, 255);

    // Dummy sensorwaarden
    ui->lcdTemp->display(21.3);
    ui->lcdCO2->display(580);
    ui->lcdHumidity->display(42);
    ui->labelCurrentMenu->setText("-");
    ui->labelVentilatorStatus->setText("Uit");
    ui->labelLichtkrant->setText("Welkom");
    ui->labelTafelStatus->setText("Tafel staat UIT");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btnSetColor_clicked() {
    if (!ui->checkBoxLightOn->isChecked()) {
        applyLightColor(0, 0, 0);
        return;
    }
    int r = ui->sliderR->value();
    int g = ui->sliderG->value();
    int b = ui->sliderB->value();
    applyLightColor(r, g, b);
}

void MainWindow::applyLightColor(int r, int g, int b) {
    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;
}

void MainWindow::on_btnPublishMenu_clicked() {
    QString menu = ui->textEditMenuInput->toPlainText();
    ui->labelCurrentMenu->setText(menu);
}

void MainWindow::on_btnVentilatorAan_clicked() {
    ui->labelVentilatorStatus->setText("Aan");
    qDebug() << "Ventilator aan";
}

void MainWindow::on_btnVentilatorUit_clicked() {
    ui->labelVentilatorStatus->setText("Uit");
    qDebug() << "Ventilator uit";
}

void MainWindow::on_btnTafelToggle_clicked() {
    QString current = ui->labelTafelStatus->text();
    if (current.contains("UIT")) {
        ui->labelTafelStatus->setText("Tafel staat AAN");
    } else {
        ui->labelTafelStatus->setText("Tafel staat UIT");
    }
}

void MainWindow::on_btnUpdateLichtkrant_clicked() {
    QString tekst = ui->lineEditLichtkrant->text();
    ui->labelLichtkrant->setText(tekst);
}
