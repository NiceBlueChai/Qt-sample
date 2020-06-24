#ifndef PAINTAREA_H
#define PAINTAREA_H

#include <QWidget>
#include <QImage>

class IBrush;

//TODO: 对称轴

class Paintarea : public QWidget
{
    Q_OBJECT
public:
    explicit Paintarea(QWidget *parent = nullptr);
    bool openImage(const QString &fileName);                        // 打开图片
    bool saveImage(const QString &fileName, const char* fileFormat);// 保存图片
    void setImage(const QImage &image);                             // 设置背景图片
    void setBrushColor(const QColor &color);                        // 设置画笔颜色
    void setBrushWidth(int width);                                  // 设置画笔宽度
    void setBrush(IBrush* iBrush, const QString & brush);            // 设置画笔

    QImage image() const {return theImage;}
    QColor brushColor()const{return color;}
    int brushWidth()const {return thickness;}
    QSize sizeHint() const override;
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void initImage();
    void setupPainter(QPainter& painter);               // 设置画笔反锯齿和颜色

    QImage theImage = {2000, 2000, QImage::Format_RGB32};     // 背景图片/画布
    QColor color = Qt::darkBlue;        // 画笔颜色
    int thickness = 1;                  // 画笔宽度
    int symmetries;
    QPoint center;
    IBrush *brushInterface{nullptr};
    QString brush;          // 画笔名
    QPoint lastPos{-1,-1};  // 记录上一个点

    QPainterPath pendingPath;
};

#endif // PAINTAREA_H
