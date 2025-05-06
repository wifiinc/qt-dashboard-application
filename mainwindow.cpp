#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

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
    // Als het lampje uit staat, stuur RGB = (0,0,0)
    if (!ui->checkBoxLightOn->isChecked()) {
        applyLightColor(0, 0, 0);
        return;
    }

    // Lees de sliderwaarden uit
    int r = ui->sliderR->value();
    int g = ui->sliderG->value();
    int b = ui->sliderB->value();

    // Pas de kleur toe via een aparte functie
    applyLightColor(r, g, b);
}

// Deze functie simuleert het toepassen van de RGB kleur
void MainWindow::applyLightColor(int r, int g, int b) {
    // Voor nu: toon in de debug output
    qDebug() << "RGB ingesteld op R:" << r << " G:" << g << " B:" << b;
}

// Wordt uitgevoerd als je op 'Publiceer Menu' klikt
void MainWindow::on_btnPublishMenu_clicked() {
    // Haal tekst uit de QTextEdit en toon in het menu-label
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

    // Check huidige status en wissel
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
