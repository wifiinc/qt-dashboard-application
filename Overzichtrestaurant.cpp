#include "Overzichtrestaurant.h"
#include <QVBoxLayout>
#include <QPixmap>

OverzichtRestaurant::OverzichtRestaurant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OverzichtRestaurant)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    // Achtergrond
    QPixmap bg(":/images/restaurant_bg.png"); // Zet dit bestand in je resource
    scene->addPixmap(bg)->setZValue(-1);

    // Voorbeeld: bewegingsdetector
    QPixmap motionIcon(":/images/motion.png");
    InteractiveItem* motion = new InteractiveItem(motionIcon, "Bewegingsdetector: Detecteert beweging", this);
    motion->setPos(200, 150);
    scene->addItem(motion);
    connect(motion, &InteractiveItem::requestOpenWindow, this, &OverzichtRestaurant::onItemClicked);

    // Voeg meerdere items toe zoals tafels, CO2, temperatuur, etc.
}

OverzichtRestaurant::~OverzichtRestaurant() {
    delete ui;
}

void OverzichtRestaurant::onItemClicked(const QString& info) {
    QDialog dlg(this);
    dlg.setWindowTitle("Item Informatie");
    QVBoxLayout *lay = new QVBoxLayout(&dlg);
    QLabel *lbl = new QLabel(info, &dlg);
    lbl->setWordWrap(true);
    lay->addWidget(lbl);
    dlg.exec();
}
