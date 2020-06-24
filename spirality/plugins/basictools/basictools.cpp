#include "basictools.h"
#include <QPainter>

QStringList BasicToolsPlugin::brushs() const
{
    return {tr("Pencil")};
}

QRect BasicToolsPlugin::mousePress(const QString &brush, QPainter &painter,
                             const QPoint &pos)
{
    return mouseMove(brush, painter, pos, pos);
}

QRect BasicToolsPlugin::mouseMove(const QString &brush, QPainter &painter,
                            const QPoint &oldPos, const QPoint &newPos)
{
    painter.save();

    int rad = painter.pen().width()/2;
    QRect boundingRect = QRect(oldPos, newPos).normalized()
                             .adjusted(-rad, -rad, rad, rad);
    QColor color = painter.pen().color();
    int thickness = painter.pen().width();
    QColor transparentColor(color.red(), color.green(), color.blue(), 0);

    if(brush == tr("Pencil"))
    {
            painter.drawLine(oldPos, newPos);
    }

    painter.restore();
    return boundingRect;
}

QRect BasicToolsPlugin::mouseRelease(const QString &/*brush*/, QPainter &/*painter*/,
                               const QPoint &/*pos*/)
{
    return QRect(0,0,0,0);
}


