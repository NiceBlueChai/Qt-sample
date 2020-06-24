#ifndef INTERFACE_H
#define INTERFACE_H

#include <QtPlugin>

QT_BEGIN_NAMESPACE
class QImage;
class QPainter;
class QPainterPath;
class QWidget;
class QPoint;
class QRect;
class QString;
class QStringList;
QT_END_NAMESPACE

// 画笔的接口
class IBrush
{
public:
    virtual ~IBrush()=default;

    virtual QStringList brushs()const =0;
    virtual QRect mousePress(const QString &brush, QPainter &painter,
                            const QPoint &pos)=0;
    virtual QRect mouseMove(const QString &brush, QPainter &painter,
                           const QPoint &oldPos, const QPoint &newPos)=0;
    virtual QRect mouseRelease(const QString &brush, QPainter &painter,
                         const QPoint &pos)=0;
};

// 自定义形状画笔
class IShape{
public:
    virtual ~IShape()=default;

    virtual QStringList shapes()const=0;
    virtual QPainterPath generateShape(const QString& shap,
                                       QWidget *parent)=0;
};

// 滤镜
class IFilter{
public:
    virtual ~IFilter()=default;

    virtual QStringList filters()const=0;
    virtual QImage filterImage(const QString& filter, const QImage &image,
                               QWidget *parent);
};

Q_DECLARE_INTERFACE(IBrush, "org.nicebluechai.spirality.IBrush/1.0")

Q_DECLARE_INTERFACE(IShape, "org.nicebluechai.spirality.IShape/1.0")

Q_DECLARE_INTERFACE(IFilter, "org.nicebluechai.spirality.IFilter/1.0")

#endif // INTERFACE_H
