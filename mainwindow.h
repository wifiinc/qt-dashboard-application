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
    void updateRgbSensorId(int newID);
    void updateBridgeIp(const QString& newIp);

private:
    Ui::MainWindow *ui;
    int rgbSensorId = 1;
    QString bridgeIp = "192.168.1.100";
    SettingsWindow* settingsWindow;

    void applyLightColor(int r, int g, int b);
};

#endif // MAINWINDOW_H
