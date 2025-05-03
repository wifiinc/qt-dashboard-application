#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSetColor_clicked();
    void on_btnPublishMenu_clicked();
    void on_btnVentilatorAan_clicked();
    void on_btnVentilatorUit_clicked();
    void on_btnTafelToggle_clicked();
    void on_btnUpdateLichtkrant_clicked();

private:
    Ui::MainWindow *ui;

    void applyLightColor(int r, int g, int b);
};

#endif // MAINWINDOW_H
