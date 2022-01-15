#include "paintarea.h"

#include <QMouseEvent>
#include <QPainter>
#include "interface.h"


Paintarea::Paintarea(QWidget *parent) : QWidget(parent)
{
    symmetries = 10;
    initImage();
    center = QPoint(theImage.width()/2, theImage.height()/2);
}

bool Paintarea::openImage(const QString &fileName)
{
    QImage image;
    if(!image.load(fileName))
        return false;

    setImage(image);
    return true;
}

bool Paintarea::saveImage(const QString &fileName, const char* fileFormat)
{
    return theImage.save(fileName, fileFormat);
}

void Paintarea::setImage(const QImage &image)
{
    theImage = image.convertToFormat(QImage::Format_RGB32);
    update();
    updateGeometry();
}

void Paintarea::setBrushColor(const QColor &color)
{
    this->color = color;
}

void Paintarea::setBrushWidth(int width)
{
    thickness = width;
}

void Paintarea::setBrush(IBrush *iBrush, const QString &brush)
{
    this->brushInterface = iBrush;
    this->brush = brush;
}

QSize Paintarea::sizeHint() const
{
    return theImage.size();
}

void Paintarea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawImage(QPoint(0,0), theImage);
}

void Paintarea::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if(!pendingPath.isEmpty())
        {
            QPainter painter(&theImage);
            setupPainter(painter);

            const QRectF boundingRect = pendingPath.boundingRect();
            QLinearGradient gradient(boundingRect.topRight(), boundingRect.bottomLeft());

            gradient.setColorAt(0.0, QColor(color.red(),color.green(),color.blue(), 63));
            gradient.setColorAt(1.0, QColor(color.red(), color.green(), color.blue(),
                                            190));
            painter.setBrush(gradient);
            painter.translate(event->pos()-boundingRect.center());
            painter.drawPath(pendingPath);

            pendingPath  = QPainterPath();

            update();
        } else{
            if(brushInterface)
            {
                QPainter painter(&theImage);
                setupPainter(painter);
                const QRect rect = brushInterface->mousePress(brush, painter,
                                                              event->pos()-center);
                update(rect);
            } // if
        } // else
        lastPos = event->pos();
    } // if
}

void Paintarea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && lastPos != QPoint(-1,-1)){
        if(brushInterface){
            QPainter painter(&theImage);
            setupPainter(painter);
            for(int i=0;i<symmetries;++i){
            const auto rect = brushInterface->mouseMove(brush, painter,
                                                        lastPos-center, event->pos()-center);
             painter.rotate(360/symmetries);
            }update();
        }
        lastPos = event->pos();
    }
}

void Paintarea::mouseReleaseEvent(QMouseEvent *event)
{
    if( (event->button() == Qt::LeftButton) && lastPos != QPoint(-1,-1)){
        if(brushInterface){
            QPainter painter(&theImage);
            setupPainter(painter);

            const auto rect = brushInterface->mouseRelease(brush, painter,
                                                           event->pos()-center);

            update(rect);
        }
        lastPos = QPoint(-1,-1);
    }
}

void Paintarea::initImage()
{
    theImage.fill(Qt::black);
    QPainter painter(&theImage);

    int w = theImage.width(),h= theImage.height();

    painter.translate(w/2,h/2);

    painter.setPen(QPen(QColor(qRgb(100,100,100)), 0.5));

    for(int i=0;i<symmetries;++i){
        painter.drawLine(0,0,w/2, 0);
        painter.rotate(360/symmetries);
    }
}

inline void Paintarea::setupPainter(QPainter &painter)
{
    painter.setRenderHints(QPainter::Antialiasing| QPainter::TextAntialiasing);
    painter.setPen(QPen(color, thickness, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.translate(theImage.width()/2, theImage.height()/2);
}
