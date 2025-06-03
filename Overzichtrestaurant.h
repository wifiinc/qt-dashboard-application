#ifndef OVERZICHTRESTAURANT_H
#define OVERZICHTRESTAURANT_H

#pragma once

#include <QDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>

namespace Ui {
class OverzichtRestaurant;
}

class InteractiveItem : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    InteractiveItem(const QPixmap& pix, const QString& infoText, QWidget* parentWindow)
        : QGraphicsPixmapItem(pix), m_info(infoText), m_parent(parentWindow)
    {
        setAcceptHoverEvents(true);
    }

signals:
    void requestOpenWindow(const QString& id);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent* e) override {
        setToolTip(m_info);
        QGraphicsPixmapItem::hoverEnterEvent(e);
    }
    void mousePressEvent(QGraphicsSceneMouseEvent* e) override {
        emit requestOpenWindow(m_info);
        QGraphicsPixmapItem::mousePressEvent(e);
    }

private:
    QString m_info;
    QWidget* m_parent;
};

class OverzichtRestaurant : public QDialog {
    Q_OBJECT

public:
    explicit OverzichtRestaurant(QWidget *parent = nullptr);
    ~OverzichtRestaurant();

private slots:
    void onItemClicked(const QString& info);

private:
    Ui::OverzichtRestaurant *ui;
    QGraphicsScene *scene;
};

#endif // OVERZICHTRESTAURANT_H
