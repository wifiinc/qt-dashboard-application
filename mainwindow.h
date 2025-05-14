#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Tcpsocket.h"

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
    void on_btnSetColor_clicked();


private:
    Ui::MainWindow *ui;
    Tcpsocket client;
    void applyLightColor(int r, int g, int b);

};

#endif // MAINWINDOW_H
