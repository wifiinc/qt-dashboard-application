#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

    void setRgbSensorId(int id);
    void setBridgeIp(const QString& ip);
    void setBridgePort(int port);

signals:
    void rgbSensorIdChanged(int newID);
    void bridgeIpChanged(const QString& newIp, int newPort);

private slots:
    void on_btnSave_clicked();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
