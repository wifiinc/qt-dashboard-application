// mapwindow.h
#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QDialog>
#include "hoverlabel.h"

class MapWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = nullptr);
    ~MapWindow();

public slots:
    void updateDeviceStatus(int deviceIndex, const QString &nieuwTekst);

private:
    HoverLabel *imageLabel;
};

#endif // MAPWINDOW_H
