// MapWindow.cpp
#include "mapwindow.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QFile>

// Constructor + destructor
MapWindow::MapWindow(QWidget *parent)
    : QDialog(parent),
    imageLabel(nullptr)
{
    setWindowTitle("Plattegrond");
    resize(800, 600);

    // 1) Maak de layout voor dit venster
    QVBoxLayout *layout = new QVBoxLayout(this);

    // 2) Creëer de HoverLabel
    imageLabel = new HoverLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMouseTracking(true);

    // 3) Laad de originele PNG (let erop dat je het juiste resource‐pad hebt)
    //    In je .qrc moet staan:
    //      <qresource prefix="/image">
    //          <file>Plattegrondvandeentreevanhetgebouw.png</file>
    //      </qresource>
    //    Vervolgens laad je met “:/image/…”
    QPixmap origMap("image/Plattegrondvandeentreevanhetgebouw.png");
    //QString resPath = ":images/Plattegrondvandeentreevanhetgebouw.png";
    //if (!QFile::exists(resPath)) {
    //    qWarning() << "MapWindow: resource bestaat NIET op pad:" << resPath;
    //}
    //QPixmap origMap(resPath);
    if (origMap.isNull()) {
        qWarning() << "MapWindow: kon plattegrond PNG niet laden!";
    } else {
        // 4) Sla de originele pixmap op in HoverLabel
        imageLabel->originalPixmap = origMap;
        // Let op: we *schalen niet* hier. Dat gebeurt in HoverLabel::resizeEvent.
    }

    // 6) Vul de DRIE lamp‐punten in met originele pixel‐coördinaten
    //    **Belangrijk**: meet deze waarden in GIMP/Photoshop/Preview in de originele PNG
    //    exact af. Onderstaande coördinaten zijn slechts een voorbeeld en moeten aangepast.
    QPointF lamp1Pos(250.0,  55.0);
    QPointF lamp2Pos(450.0,  50.0);
    QPointF lamp3Pos(515.0, 75.0);

    // 7) Maak voor elk lampje een LampInfo met coördinaten + statusText
    HoverLabel::LampInfo info1; info1.point = lamp1Pos; info1.statusText = "Lamp 1: UIT";
    HoverLabel::LampInfo info2; info2.point = lamp2Pos; info2.statusText = "Lamp 2: AAN";
    HoverLabel::LampInfo info3; info3.point = lamp3Pos; info3.statusText = "Lamp 3: DIM 50%";

    // 8) Voeg ze toe aan lampList
    imageLabel->lampList.clear();
    imageLabel->lampList << info1 << info2 << info3;

    // 9) Voeg de HoverLabel toe aan de layout
    layout->addWidget(imageLabel);

    setLayout(layout);
}

MapWindow::~MapWindow()
{
    // Omdat we parent=this hebben meegegeven aan imageLabel, wordt deze automatisch opgeruimd.
    // Hier hoeft niets speciaals te gebeuren.
}

void MapWindow::updateLampStatus(int lampIndex, const QString &nieuwTekst)
{
    // Als je later in MainWindow een signaal stuurt dat lampIndex (0..2) is veranderd:
    if (lampIndex >= 0 && lampIndex < imageLabel->lampList.size()) {
        imageLabel->lampList[lampIndex].statusText = nieuwTekst;
        // Repaint niet per se nodig, want de tooltip verschijnt pas bij hover.
        imageLabel->update();
    }
}
