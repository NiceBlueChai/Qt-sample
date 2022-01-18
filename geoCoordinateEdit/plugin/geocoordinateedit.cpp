#pragma execution_character_set("utf-8")
#include "geocoordinateedit.h"
#include <QLineEdit>
#include <QPointer>
#include <QRegularExpressionValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QKeyEvent>
#include <math.h>

class GeoCoordinateEditPrivate{
    Q_DECLARE_PUBLIC(GeoCoordinateEdit)
public:
    GeoCoordinateEdit* q_ptr;
    QWidget* m_parent;
    GeoCoordinateEdit::ValueType type;
    QPointer<QValidator> curValueTypeDegressValidator;
    QPointer<QRegularExpressionValidator> minuteValidator;
    QPointer<QRegularExpressionValidator> secondValidator;
    double value;
    QColor color;
    QColor bgColor;
    QLineEdit* lineEditDegress;
    QLineEdit* lineEditMinutes;
    QLineEdit* lineEditSeconds;


    explicit GeoCoordinateEditPrivate(GeoCoordinateEdit* parent)
        :q_ptr(parent),m_parent(parent),type(GeoCoordinateEdit::Latitude),
          curValueTypeDegressValidator(createValidator(type)),
    minuteValidator(new QRegularExpressionValidator(parent)),
      secondValidator(new QRegularExpressionValidator(parent)),
      color(0,0,0),bgColor(200,200,200)
    {
        QRegularExpression reg("^[0-5]?[0-9]{1}\\.\\d{0,4}");
        QRegularExpression reg2("^[0-5]?[0-9]{1}");
        minuteValidator->setRegularExpression(reg2);
        secondValidator->setRegularExpression(reg);
    }
    static QPointer<QRegularExpressionValidator>
    createValidator(GeoCoordinateEdit::ValueType type, QObject* parent=nullptr)
    {
        if(type == GeoCoordinateEdit::Latitude){
            QRegularExpression reg("^-?[1-9]?\\d");
            return new QRegularExpressionValidator(reg, parent);
        }else{
            QRegularExpression reg("^-?(180|1?[0-7]?\\d)$ -180< $ <180");
             return new QRegularExpressionValidator(reg, parent);
        }
    }
};


GeoCoordinateEdit::GeoCoordinateEdit(QWidget *parent) :
    QWidget(parent),d_ptr(new GeoCoordinateEditPrivate(this))
{
    initUI();
    clear();
    updateColor();
}

GeoCoordinateEdit::~GeoCoordinateEdit()
{
}

double GeoCoordinateEdit::degressMinuteSecond2Double(const QString &str,
                                            GeoCoordinateEdit::ValueType type)
{
    auto strList = str.split(QStringLiteral("°"));
    QString degStr = strList.at(0);
    strList = strList.at(1).split("'");
    QString minStr = strList.at(0);
    QString secStr = strList.at(1).split("\"").at(0);
    double ret = degStr.toDouble()+minStr.toDouble()*60.0+secStr.toDouble()*3600.0;
    if (isValidValue(ret, type)) {
        return ret;
    }
    return -200.0;//无效值
}

QString GeoCoordinateEdit::double2DegressMinuteSecond(double value,
                                            GeoCoordinateEdit::ValueType type)
{
    if(!isValidValue(value, type))
        return QString();
    int deg = floor(value);
    int min = floor(60.0*(value-deg));
    double sec = 60.0*(60.0*(value-deg)-min);
    return QStringLiteral("%1°%2'%3\"").arg(deg).arg(min).arg(sec);
}

bool GeoCoordinateEdit::isValidValue(double value,
                             GeoCoordinateEdit::ValueType type)
{
    if(type == GeoCoordinateEdit::Latitude)
        return (value>=-90.0 && value <=90.0);
    return (value>=-180.0 && value <=180.0);
}

QSize GeoCoordinateEdit::sizeHint() const
{
    return QSize(180, 30);
}

bool GeoCoordinateEdit::eventFilter(QObject *watched, QEvent *event)
{
    return QWidget::eventFilter(watched, event);
}

QSize GeoCoordinateEdit::minimumSizeHint() const
{
    return QSize(164,20);
}

GeoCoordinateEdit::ValueType GeoCoordinateEdit::getType() const
{
    auto d = d_func();
    return d->type;
}

QString GeoCoordinateEdit::text() const
{
    return QStringLiteral("%1°%2'%3\"").arg(d_ptr->lineEditDegress->text())
            .arg(d_ptr->lineEditMinutes->text())
            .arg(d_ptr->lineEditSeconds->text());
}

int GeoCoordinateEdit::getDegress() const
{
    return d_ptr->lineEditDegress->text().toInt();
}

int GeoCoordinateEdit::getMinutes() const
{
    return d_ptr->lineEditMinutes->text().toUInt();
}

double GeoCoordinateEdit::getSeconds() const
{
    return d_ptr->lineEditSeconds->text().toDouble();
}

double GeoCoordinateEdit::getValue() const
{
    return d_ptr->value;
}

QColor GeoCoordinateEdit::getColor() const
{
    return d_ptr->color;
}

QColor GeoCoordinateEdit::getBgColor() const
{
    return d_ptr->bgColor;
}

void GeoCoordinateEdit::setType(GeoCoordinateEdit::ValueType type)
{
    Q_D(GeoCoordinateEdit);
    d->type = type;
    d->curValueTypeDegressValidator.clear();
    d->curValueTypeDegressValidator = GeoCoordinateEditPrivate::createValidator(d->type);
    d->lineEditDegress->setValidator(d->curValueTypeDegressValidator.data());
    emit typeChanged(d->type);
}

void GeoCoordinateEdit::setValue(double value)
{
    Q_D(GeoCoordinateEdit);
    d->value = value;
    valueChanged();
}

void GeoCoordinateEdit::setText(const QString &str)
{
    Q_D(GeoCoordinateEdit);
    double ret = degressMinuteSecond2Double(str, d->type);
    if(ret <= -200.0)return;
    d->value = ret;
    valueChanged();
}

void GeoCoordinateEdit::setColor(QColor color)
{
    Q_D(GeoCoordinateEdit);
    d->color = color;
    updateColor();
}

void GeoCoordinateEdit::setBgColor(QColor color)
{
    Q_D(GeoCoordinateEdit);
    d->bgColor = color;
    updateColor();
}

void GeoCoordinateEdit::clear()
{
    Q_D(GeoCoordinateEdit);
    d->value = 0.0;
    valueChanged();
}

void GeoCoordinateEdit::onTextEdited(const QString &)
{
    Q_D(GeoCoordinateEdit);
    d->value = degressMinuteSecond2Double(text(), d->type);
}

void GeoCoordinateEdit::initUI()
{
    Q_D(GeoCoordinateEdit);
    auto labelDregress = new QLabel;
    auto labelMinutes = new QLabel;
    auto labelSeconds = new QLabel;
    labelDregress->setText("°");
    labelMinutes->setText("'");
    labelSeconds->setText("\"");
    labelDregress->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    labelMinutes->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    labelSeconds->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    labelDregress->setMaximumWidth(15);
    labelMinutes->setMaximumWidth(15);
    labelSeconds->setMaximumWidth(15);
    labelDregress->setAlignment(Qt::AlignCenter);
    labelMinutes->setAlignment(Qt::AlignCenter);
    labelSeconds->setAlignment(Qt::AlignCenter);

    // TODO(NiceBlueChai): 完成功能，连接信号槽
    // 发射相关信号
    d->lineEditDegress = new QLineEdit(this);
    d->lineEditMinutes = new QLineEdit(this);
    d->lineEditSeconds = new QLineEdit(this);
    d->lineEditDegress->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    d->lineEditMinutes->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    d->lineEditSeconds->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    d->lineEditDegress->setMinimumWidth(36);
    d->lineEditMinutes->setMinimumWidth(36);
    d->lineEditSeconds->setMinimumWidth(70);
    d->lineEditDegress->setAlignment(Qt::AlignCenter);
    d->lineEditMinutes->setAlignment(Qt::AlignCenter);
    d->lineEditSeconds->setAlignment(Qt::AlignCenter);

    d->lineEditDegress->setValidator(d->curValueTypeDegressValidator.data());
    d->lineEditMinutes->setValidator(d->minuteValidator.data());
    d->lineEditSeconds->setValidator(d->secondValidator.data());

    QHBoxLayout* lay = new QHBoxLayout;
    lay->setSpacing(0);
    lay->setContentsMargins(3,0,5,0);
    lay->addWidget(d->lineEditDegress);
    lay->addWidget(labelDregress);
    lay->addWidget(d->lineEditMinutes);
    lay->addWidget(labelMinutes);
    lay->addWidget(d->lineEditSeconds);
    lay->addWidget(labelSeconds);

    setLayout(lay);
}

void GeoCoordinateEdit::updateColor()
{
    Q_D(GeoCoordinateEdit);
    QString sty=QString("*{color: %1; background-color: %2; border:0px;outline:0px;"
                        "margin:0px;padding:0px;}").arg(d->color.name(QColor::HexArgb))
            .arg(d->bgColor.name(QColor::HexArgb));
    setStyleSheet(sty);
}

void GeoCoordinateEdit::valueChanged()
{
    Q_D(GeoCoordinateEdit);
    QString deg, min, sec;
    getDMS(deg, min, sec);
    d->lineEditDegress->setText(deg);
    d->lineEditMinutes->setText(min);
    d->lineEditSeconds->setText(sec);
}

void GeoCoordinateEdit::getDMS(QString &degree , QString &minute, QString &second)
{
    Q_D(GeoCoordinateEdit);
    double value = d->value;
    int deg = floor(value);
    int min = floor(60.0*(value-deg));
    double sec = 60.0*(60.0*(value-deg)-min);
    degree = QString::number(deg);
    minute = QString::number(min);
    second = QString::number(sec);
}
