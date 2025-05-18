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
    void setBridgeIp(const QString &ip);
    void setBridgePort(int port);
    void setTafelSensorId(int tafel1, int tafel2, int tafel3);

signals:
    void rgbSensorIdChanged(int newID);
    void tafelSensorIdChanged(int tafel1, int tafel2, int tafel3);
    void bridgeIpChanged(const QString &newIp, int newPort);
    void updateSensorList();

private slots:
    void on_btnSave_clicked();

private:
    Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
