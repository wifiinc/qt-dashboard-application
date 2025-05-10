#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void verzendPakket(const QByteArray& data);

private slots:
    void on_btnSetColor_clicked();
    void on_btnOpenSettings_clicked();
    void updateSensorId(int newID);

private:
    Ui::MainWindow *ui;
    int sensorId = 1;
    SettingsWindow* settingsWindow;

    void applyLightColor(int r, int g, int b);
};

#endif // MAINWINDOW_H
