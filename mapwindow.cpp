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
    QPointF RGB1Pos(245.0,  325.0);
    QPointF RGB2Pos(358.0,  227.0);
    QPointF Beweging1Pos(200.0, 245.0);
    QPointF Beweging2Pos(495.0, 245.0);
    QPointF CO2Pos(200.0, 245.0);
    QPointF TEMPPos(495.0, 245.0);

    // 7) Maak voor elk lampje een LampInfo met coördinaten + statusText
    HoverLabel::LampInfo info1; info1.point = lamp1Pos; info1.statusText = "Lamp 1: UIT";
    HoverLabel::LampInfo info2; info2.point = lamp2Pos; info2.statusText = "Lamp 2: AAN";
    HoverLabel::LampInfo info3; info3.point = lamp3Pos; info3.statusText = "Lamp 3: DIM 50%";
    HoverLabel::LampInfo info4; info4.point = RGB1Pos; info4.statusText = "Rgbww : UIT";
    HoverLabel::LampInfo info5; info5.point = RGB2Pos; info5.statusText = "Rgbww : UIT";
    HoverLabel::LampInfo info6; info6.point = Beweging1Pos; info6.statusText = "Geen beweging";
    HoverLabel::LampInfo info7; info7.point = Beweging2Pos; info7.statusText = "Geen beweging";
    HoverLabel::LampInfo info8; info8.point = CO2Pos; info8.statusText = "CO2 waarde kapot";
    HoverLabel::LampInfo info9; info9.point = TEMPPos; info9.statusText = "Geen temp";

    // 8) Voeg ze toe aan lampList
    imageLabel->lampList.clear();
    imageLabel->lampList << info1 << info2 << info3 << info4 << info5 << info6 << info7;

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
