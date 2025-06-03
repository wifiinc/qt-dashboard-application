#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QDialog>
#include <QLabel>

class MapWindow : public QDialog {
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = nullptr);
    ~MapWindow();

private:
    QLabel *imageLabel;
};

#endif // MAPWINDOW_H
