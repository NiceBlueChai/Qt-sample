#ifndef GEOCOORDINATEEDIT_H
#define GEOCOORDINATEEDIT_H

#include <QWidget>
#include <memory>

#ifdef quc
#if (QT_VERSION < QT_VERSION_CHECK(5,7,0))
#include <QtDesigner/QDesignerExportWidget>
#else
#include <QtUiPlugin/QDesignerExportWidget>
#endif

class QDESIGNER_WIDGET_EXPORT GeoCoordinateEdit : public QWidget
#else
class GeoCoordinateEdit : public QWidget
#endif
{
    Q_OBJECT
    Q_CLASSINFO("author", "NiceBlueChai")
    Q_ENUMS(ValueType)
    Q_PROPERTY(ValueType type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(double value READ getValue WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor)
    Q_PROPERTY(QColor bgColor READ getBgColor WRITE setBgColor)
public:

    enum ValueType{
        Longitude,
        Latitude
    };


    GeoCoordinateEdit(QWidget* parent = nullptr);
    virtual ~GeoCoordinateEdit();

    // \brief 度分秒 -> double
    // \param[in] str  QString("12°3'4.2345\"")
    // \param[in] type 经纬度类型
    static double degressMinuteSecond2Double(const QString& str, GeoCoordinateEdit::ValueType type);

    // \brief double -> 度分秒
    // param[in] value
    // param[in] type 经纬度类型
    static QString double2DegressMinuteSecond(double value, GeoCoordinateEdit::ValueType type);

    // \brief 验证经纬度
    static bool isValidValue(double value, GeoCoordinateEdit::ValueType type);

public:
    virtual QSize sizeHint() const override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual QSize minimumSizeHint() const override;

public:
    GeoCoordinateEdit::ValueType getType() const;
    QString text() const;
    int getDegress() const;
    int getMinutes() const;
    double getSeconds() const;
    double getValue() const;
    QColor getColor() const;
    QColor getBgColor() const;

public Q_SLOTS:
    void setType(GeoCoordinateEdit::ValueType type);
    void setValue(double value);
    void setText(const QString & str);
    void setColor(QColor color);
    void setBgColor(QColor color);
    void clear();

private Q_SLOTS:
    void onTextEdited(const QString&);

Q_SIGNALS:
    void typeChanged(GeoCoordinateEdit::ValueType);
    void valueChanged(double);
    void valueChanged(const QString&);

private:
    void initUI();
    void updateColor();
    void valueChanged();
    void getDMS(QString& d, QString& m, QString& s);

private:
    struct Impl;
    std::unique_ptr<Impl> data;
};

#endif // GEOCOORDINATEEDIT_H
