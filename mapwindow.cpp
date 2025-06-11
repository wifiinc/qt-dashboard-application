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

    imageLabel = new HoverLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMouseTracking(true);

    QPixmap origMap(":/plattegrond.png");
    //QString resPath = ":images/Plattegrondvandeentreevanhetgebouw.png";
    //if (!QFile::exists(resPath)) {
    //    qWarning() << "MapWindow: resource bestaat NIET op pad:" << resPath;
    //}
    //QPixmap origMap(resPath);
    if (origMap.isNull()) {
        qWarning() << "MapWindow: kon plattegrond PNG niet laden!";
    } else {
        imageLabel->originalPixmap = origMap;
    }

    QPointF lamp1Pos(250.0,  55.0);
    QPointF lamp2Pos(450.0,  50.0);
    QPointF lamp3Pos(515.0, 75.0);
    QPointF RGB1Pos(245.0,  325.0);
    QPointF RGB2Pos(358.0,  227.0);
    QPointF Beweging1Pos(200.0, 245.0);
    QPointF Beweging2Pos(495.0, 245.0);
    QPointF CO2Pos(380.0,  50.0);
    QPointF TEMPPos(168.0,  45.0);
    QPointF VenPos(27.0,  73.0);

    // 7) Maak voor elk apparaat een DeviceInfo met coördinaten + statusText
    HoverLabel::DeviceInfo info1; info1.point = lamp1Pos; info1.statusText = "Lamp 1: UIT";
    HoverLabel::DeviceInfo info2; info2.point = lamp2Pos; info2.statusText = "Lamp 2: UIT";
    HoverLabel::DeviceInfo info3; info3.point = lamp3Pos; info3.statusText = "Lamp 3: UIT";
    HoverLabel::DeviceInfo info4; info4.point = RGB1Pos; info4.statusText = "Rgbww : UIT";
    HoverLabel::DeviceInfo info5; info5.point = RGB2Pos; info5.statusText = "Rgbww : UIT";
    HoverLabel::DeviceInfo info6; info6.point = Beweging1Pos; info6.statusText = "Geen beweging";
    HoverLabel::DeviceInfo info7; info7.point = Beweging2Pos; info7.statusText = "Geen beweging";
    HoverLabel::DeviceInfo info8; info8.point = CO2Pos; info8.statusText = "CO2 waarde kapot";
    HoverLabel::DeviceInfo info9; info9.point = TEMPPos; info9.statusText = "Geen temp";
    HoverLabel::DeviceInfo info10; info10.point = VenPos; info10.statusText = "Ven kapot";

    // 8) Voeg ze toe aan Apparaten
    imageLabel->Apparaten.clear();
    imageLabel->Apparaten << info1 << info2 << info3 << info4 << info5 << info6 << info7 <<info8 <<info9 << info10;

    // 9) Voeg de HoverLabel toe aan de layout
    layout->addWidget(imageLabel);

    setLayout(layout);
}

MapWindow::~MapWindow()
{
    // Omdat we parent=this hebben meegegeven aan imageLabel, wordt deze automatisch opgeruimd.
    // Hier hoeft niets speciaals te gebeuren.
}

void MapWindow::updateDeviceStatus(int deviceIndex, const QString &nieuwTekst)
{
    // Als je later in MainWindow een signaal stuurt dat deviceIndex (0..n) is veranderd:
    if (deviceIndex >= 0 && deviceIndex < imageLabel->Apparaten.size()) {
        imageLabel->Apparaten[deviceIndex].statusText = nieuwTekst;
        qDebug() << imageLabel->Apparaten[deviceIndex].statusText;
        // Repaint niet per se nodig, want de tooltip verschijnt pas bij hover.
        imageLabel->update();
    }
}
