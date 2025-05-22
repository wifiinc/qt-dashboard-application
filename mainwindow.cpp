#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setWindowTitle("Lichtkrant Dashboard");

    connect(ui->btnVerzendLichtkrant, &QPushButton::clicked,
            this, &MainWindow::on_btnVerzendLichtkrant_clicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_btnVerzendLichtkrant_clicked() {
    QString boodschap = ui->lineEditLichtkrant->text().trimmed();
    if (boodschap.isEmpty()) {
        qDebug() << "Geen tekst ingevoerd voor lichtkrant.";
        return;
    }

    qDebug() << "Lichtkrant boodschap verzenden:" << boodschap;


}
