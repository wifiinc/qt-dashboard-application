#ifndef HOVERLABEL_H
#define HOVERLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QToolTip>
#include <QPainter>
#include <QResizeEvent>

class HoverLabel : public QLabel
{
    Q_OBJECT

public:
    explicit HoverLabel(QWidget *parent = nullptr)
        : QLabel(parent),
        hoveredLampIndex(-1),
        hoverRadius(20.0)
    {
        // Zorg dat we altijd mouseMoveEvent krijgen, ook zonder klik
        setMouseTracking(true);
    }

    ~HoverLabel() override = default;

    // Struct om de positie (in originele pixmap‐pixels) en de tooltip‐tekst op te slaan
    struct DeviceInfo {
        QPointF point;       // (x,y) in de originele PNG‐pixels
        QString statusText;  // tooltip die getoond moet worden bij hover
    };

    // Lijst van alle apparaten in originele afbeeldings‐coördinaten
    QVector<DeviceInfo> Apparaten;

    // De niet‐geschaalde QPixmap (originele plattegrond), bijv. 1200×800 pixels
    QPixmap originalPixmap;

    // Hoe ver (in ORIGINELE pixels) je van apparaat‐point mag zitten om als "hover" te gelden
    qreal hoverRadius;

protected:
    // Wordt aangeroepen bij elke muisbeweging (ook zonder klikken)
    void mouseMoveEvent(QMouseEvent *event) override
    {
        // 1) Als we geen originele pixmap hebben of geen apparaten, laat base class afhandelen
        if (originalPixmap.isNull() || Apparaten.isEmpty()) {
            QLabel::mouseMoveEvent(event);
            return;
        }

        // 2) Bepaal de afmetingen van de label vs de originele afbeelding
        QSize labelSize = size();                // b.v. 800×600
        QSize origSize  = originalPixmap.size(); // b.v. 1200×800

        // 3) Bereken hoe groot de pixmap wérkelijk is binnen de label (KeepAspectRatio)
        QSize scaledSize = origSize.scaled(labelSize, Qt::KeepAspectRatio);
        int xOffset = (labelSize.width()  - scaledSize.width())  / 2;
        int yOffset = (labelSize.height() - scaledSize.height()) / 2;

        // 4) Muispositie in label‐coördinaten
        QPoint posLabel = event->pos();

        // 5) Controleer of de muis binnen het pixmap‐gebied zit
        if (posLabel.x() < xOffset ||
            posLabel.x() > xOffset + scaledSize.width() ||
            posLabel.y() < yOffset ||
            posLabel.y() > yOffset + scaledSize.height())
        {
            // Muis buiten afbeelding: verberg tooltip en reset hoverIndex
            if (hoveredLampIndex != -1) {
                hoveredLampIndex = -1;
                update();  // repaint om cirkel te verwijderen
            }
            QToolTip::hideText();
            return;
        }

        // 6) Vertaal mouse‐coördinaat naar originele afbeelding‐pixel
        qreal scaleX = static_cast<qreal>(origSize.width())  / static_cast<qreal>(scaledSize.width());
        qreal scaleY = static_cast<qreal>(origSize.height()) / static_cast<qreal>(scaledSize.height());
        QPointF posInScaled(posLabel.x() - xOffset,
                            posLabel.y() - yOffset);
        QPointF posInOrig(posInScaled.x() * scaleX,
                          posInScaled.y() * scaleY);

        // 7) Loop door alle apparaten: kijk of afstand ≤ hoverRadius
        bool found = false;
        for (int i = 0; i < Apparaten.size(); ++i) {
            const DeviceInfo &li = Apparaten[i];
            qreal afstand = QLineF(li.point, posInOrig).length();
            if (afstand <= hoverRadius) {
                // We hoveren over apparaat i
                if (hoveredLampIndex != i) {
                    hoveredLampIndex = i;
                    update(); // trigger paintEvent om cirkel te tekenen
                }
                // Toon de tooltip
                QToolTip::showText(event->globalPos(), li.statusText);
                found = true;
                break;
            }
        }
        if (!found) {
            // Muiskoord niet binnen hoverRadius van wélk apparaat
            if (hoveredLampIndex != -1) {
                hoveredLampIndex = -1;
                update(); // cirkel verwijderen
            }
            QToolTip::hideText();
        }
    }

    // PaintEvent: tekent eerst de pixmap (via QLabel::paintEvent) en dan (optioneel) een cirkel
    void paintEvent(QPaintEvent *ev) override
    {
        // 1) Laat QLabel de pixmap tekenen
        QLabel::paintEvent(ev);

        // 2) Als we op een apparaat hoveren (hoveredLampIndex ≥ 0), teken een cirkel
        if (hoveredLampIndex >= 0 && hoveredLampIndex < Apparaten.size()) {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);

            QSize labelSize = size();
            QSize origSize  = originalPixmap.size();
            QSize scaledSize = origSize.scaled(labelSize, Qt::KeepAspectRatio);
            int xOffset = (labelSize.width()  - scaledSize.width())  / 2;
            int yOffset = (labelSize.height() - scaledSize.height()) / 2;
            qreal scaleX = static_cast<qreal>(scaledSize.width())  / static_cast<qreal>(origSize.width());
            qreal scaleY = static_cast<qreal>(scaledSize.height()) / static_cast<qreal>(origSize.height());

            QPointF origPt = Apparaten[hoveredLampIndex].point;
            QPoint  screenPt(
                qRound(origPt.x() * scaleX) + xOffset,
                qRound(origPt.y() * scaleY) + yOffset
                );

            // Teken een gele cirkel (radius 20 pixels) rond het apparaat
            painter.setPen(QPen(Qt::yellow, 2));
            painter.drawEllipse(screenPt, 20, 20);
        }
    }

    // Zorg dat bij elke resize de originele pixmap opnieuw wordt geschaald en getoond
    void resizeEvent(QResizeEvent *ev) override
    {
        QLabel::resizeEvent(ev);
        if (!originalPixmap.isNull()) {
            QPixmap pm = originalPixmap.scaled(size(),
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation);
            setPixmap(pm);
        }
    }

private:
    int hoveredLampIndex;  // index van het apparaat waar we op dit moment over hoveren (-1 = geen)
};

#endif // HOVERLABEL_H
