#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Tcpsocket.h"
#include "settingswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void requestluisteren();
    void on_btnOpenSettings_clicked();
    void updateRgbSensorId(int newID);
    void updateBridgeIp(const QString& newIp, int newPort);


    void on_changeRGBBtn_clicked();
    void on_saveRGBBtn_clicked();
    void on_checkBoxLightOn_toggled(bool checked);

    void on_tafel1Toggle_clicked();
    void on_tafel2Toggle_clicked();
    void on_tafel3Toggle_clicked();

private:
    Ui::MainWindow *ui;
    Tcpsocket client;
    int rgbSensorId = 1;
    QString bridgeIp = "10.0.0.2";
    int bridgePort = 5000;
    SettingsWindow* settingsWindow;
    void applyLightColor(int r, int g, int b);
    QColor huidigekleurRGBLed = Qt::white;
    int tafel1SensorId = 2;
    bool tafel1State = false;
    int tafel2SensorId = 1;
    bool tafel2State = false;
    int tafel3SensorId = 2;
    bool tafel3State = false;
};

#endif // MAINWINDOW_H
