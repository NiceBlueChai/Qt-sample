#ifndef BASICTOOLS_H
#define BASICTOOLS_H

#include <interface.h>

#include <QImage>
#include <QObject>
#include <QPainterPath>
#include <QRect>
#include <QStringList>
#include <QtPlugin>

class BasicToolsPlugin :public QObject, public IBrush
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.nicebluechai.spirality.IBrush" FILE "basictools.json")

    Q_INTERFACES(IBrush)

public:
    // IBrush interface
    virtual QStringList brushs() const override;
    virtual QRect mousePress(const QString &brush, QPainter &painter, const QPoint &pos) override;
    virtual QRect mouseMove(const QString &brush, QPainter &painter,
                            const QPoint &oldPos, const QPoint &newPos) override;
    virtual QRect mouseRelease(const QString &brush, QPainter &painter, const QPoint &pos) override;
};

#endif // BASICTOOLS_H
