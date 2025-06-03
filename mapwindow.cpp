#include "mapwindow.h"
#include <QVBoxLayout>
#include <QPixmap>

MapWindow::MapWindow(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Plattegrond");
    resize(800, 600);  // Pas aan indien gewenst

    QVBoxLayout *layout = new QVBoxLayout(this);
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);

    QPixmap map("image/Plattegrondvandeentreevanhetgebouw.png");  // Er zit een png/foto in image die word uitgetekend
    imageLabel->setPixmap(map.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    layout->addWidget(imageLabel);
    setLayout(layout);
}

MapWindow::~MapWindow() {}
