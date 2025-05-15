#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
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
    void on_btnOpenColorPicker_clicked();
    void updateRgbSensorId(int newID);
    void updateBridgeIp(const QString& newIp, int newPort);

private:
    Ui::MainWindow *ui;
    int rgbSensorId = 1;
    QString bridgeIp = "10.0.0.2";
    int bridgePort = 5000;
    SettingsWindow* settingsWindow;
    QColor huidigeKleur = Qt::white;
    void applyLightColor(int r, int g, int b);
};

#endif // MAINWINDOW_H
