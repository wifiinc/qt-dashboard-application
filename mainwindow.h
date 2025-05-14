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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void requestluisteren();

private:
    Ui::MainWindow *ui;
    Tcpsocket client;
    Receivetcpsocket request;
};

#endif // MAINWINDOW_H
