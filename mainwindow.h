#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
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
    void writeSettings();
    void loadSettings();
    void requestluisteren();
    void on_btnOpenSettings_clicked();
    void updateRgbSensorId(int newID);
    void updateBridgeIp(const QString& newIp, int newPort);


    void on_changeRGBBtn_clicked();
    void on_saveRGBBtn_clicked();

    void on_checkBoxLightOn_toggled(bool checked);

private:
    Ui::MainWindow *ui;
    Tcpsocket client;
    SettingsWindow* settingsWindow;

    int rgbSensorId;
    QString bridgeIp;
    int bridgePort;

    QColor huidigekleurRGBLed = Qt::white;
};

#endif // MAINWINDOW_H
