#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Tcpsocket.h"
#include "Receivetcpsocket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Toegevoegd: declaratie van verzendPakket
    void verzendPakket(const QByteArray& data);

private slots:
    void requestluisteren();
    void on_btnSetColor_clicked();


private:
    Ui::MainWindow *ui;
    Tcpsocket client;
    Receivetcpsocket request;
    void applyLightColor(int r, int g, int b);

};

#endif // MAINWINDOW_H
