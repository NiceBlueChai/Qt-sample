#include "geocoordinateedit.h"
#include <QLineEdit>
#include <QPointer>
#include <QRegularExpressionValidator>
#include <QHBoxLayout>
#include <QLabel>
#include <QSizePolicy>
#include <QKeyEvent>
#include <math.h>

struct GeoCoordinateEdit::Impl{
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

    explicit Impl(QWidget* parent)
        :m_parent(parent),type(GeoCoordinateEdit::Latitude),
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
    QWidget(parent),data(new Impl(this))
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
    auto tmpStr = str;
    auto strList = str.split(QStringLiteral("°"));
    QString degStr = strList.at(0);
    strList = strList.at(1).split("'");
    QString minStr = strList.at(0);
    QString secStr = strList.at(1).split("\"").at(0);
    double ret = degStr.toDouble()+minStr.toDouble()*60.0+secStr.toDouble()*3600.0;
    if(isValidValue(ret, type))
        return ret;
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
    return data->type;
}

QString GeoCoordinateEdit::text() const
{
    return QStringLiteral("%1°%2'%3\"").arg(data->lineEditDegress->text())
            .arg(data->lineEditMinutes->text())
            .arg(data->lineEditSeconds->text());
}

int GeoCoordinateEdit::getDegress() const
{
    return data->lineEditDegress->text().toInt();
}

int GeoCoordinateEdit::getMinutes() const
{
    return data->lineEditMinutes->text().toUInt();
}

double GeoCoordinateEdit::getSeconds() const
{
    return data->lineEditSeconds->text().toDouble();
}

double GeoCoordinateEdit::getValue() const
{
    return data->value;
}

QColor GeoCoordinateEdit::getColor() const
{
    return data->color;
}

QColor GeoCoordinateEdit::getBgColor() const
{
    return data->bgColor;
}

void GeoCoordinateEdit::setType(GeoCoordinateEdit::ValueType type)
{
    data->type = type;
    data->curValueTypeDegressValidator.clear();
    data->curValueTypeDegressValidator = Impl::createValidator(data->type);
    data->lineEditDegress->setValidator(data->curValueTypeDegressValidator.data());
    emit typeChanged(data->type);
}

void GeoCoordinateEdit::setValue(double value)
{
    data->value = value;
    valueChanged();
}

void GeoCoordinateEdit::setText(const QString &str)
{
    double ret = degressMinuteSecond2Double(str, data->type);
    if(ret <= -200.0)return;
    data->value = ret;
    valueChanged();
}

void GeoCoordinateEdit::setColor(QColor color)
{
    data->color = color;
    updateColor();
}

void GeoCoordinateEdit::setBgColor(QColor color)
{
    data->bgColor = color;
    updateColor();
}

void GeoCoordinateEdit::clear()
{
   data->value = 0.0;
   valueChanged();
}

void GeoCoordinateEdit::onTextEdited(const QString &)
{
    data->value = degressMinuteSecond2Double(text(), data->type);
}

void GeoCoordinateEdit::initUI()
{
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

    data->lineEditDegress = new QLineEdit;
    data->lineEditMinutes = new QLineEdit;
    data->lineEditSeconds = new QLineEdit;
    data->lineEditDegress->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    data->lineEditMinutes->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    data->lineEditSeconds->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);
    data->lineEditDegress->setMinimumWidth(36);
    data->lineEditMinutes->setMinimumWidth(36);
    data->lineEditSeconds->setMinimumWidth(70);
    data->lineEditDegress->setAlignment(Qt::AlignCenter);
    data->lineEditMinutes->setAlignment(Qt::AlignCenter);
    data->lineEditSeconds->setAlignment(Qt::AlignCenter);

    data->lineEditDegress->setValidator(data->curValueTypeDegressValidator.data());
    data->lineEditMinutes->setValidator(data->minuteValidator.data());
    data->lineEditSeconds->setValidator(data->secondValidator.data());

    QHBoxLayout* lay = new QHBoxLayout;
    lay->setSpacing(0);
    lay->setContentsMargins(3,0,5,0);
    lay->addWidget(data->lineEditDegress);
    lay->addWidget(labelDregress);
    lay->addWidget(data->lineEditMinutes);
    lay->addWidget(labelMinutes);
    lay->addWidget(data->lineEditSeconds);
    lay->addWidget(labelSeconds);

    setLayout(lay);
}

void GeoCoordinateEdit::updateColor()
{
    QString sty=QString("*{color: %1; background-color: %2; border:0px;outline:0px;"
                        "margin:0px;padding:0px;}").arg(data->color.name(QColor::HexArgb))
            .arg(data->bgColor.name(QColor::HexArgb));
    setStyleSheet(sty);
}

void GeoCoordinateEdit::valueChanged()
{
    QString deg, min, sec;
    getDMS(deg, min, sec);
    data->lineEditDegress->setText(deg);
    data->lineEditMinutes->setText(min);
    data->lineEditSeconds->setText(sec);
}

void GeoCoordinateEdit::getDMS(QString &d, QString &m, QString &s)
{
    double value = data->value;
    int deg = floor(value);
    int min = floor(60.0*(value-deg));
    double sec = 60.0*(60.0*(value-deg)-min);
    d = QString::number(deg);
    m = QString::number(min);
    s = QString::number(sec);
}
