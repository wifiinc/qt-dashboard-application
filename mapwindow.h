// mapwindow.h
#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QDialog>
#include "hoverlabel.h"   // Maak HoverLabel in deze header beschikbaar

class MapWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = nullptr);
    ~MapWindow();

public slots:
    // (optioneel) slot om dynamisch de status van lampjes bij te werken
    void updateLampStatus(int lampIndex, const QString &nieuwTekst);

private:
    HoverLabel *imageLabel;
};

#endif // MAPWINDOW_H
