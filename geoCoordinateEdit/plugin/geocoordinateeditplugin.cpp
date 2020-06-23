#include "geocoordinateedit.h"
#include "geocoordinateeditplugin.h"

#include <QtPlugin>

GeoCoordinateEditPlugin::GeoCoordinateEditPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void GeoCoordinateEditPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool GeoCoordinateEditPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *GeoCoordinateEditPlugin::createWidget(QWidget *parent)
{
    return new GeoCoordinateEdit(parent);
}

QString GeoCoordinateEditPlugin::name() const
{
    return QLatin1String("GeoCoordinateEdit");
}

QString GeoCoordinateEditPlugin::group() const
{
    return QLatin1String("NBC");
}

QIcon GeoCoordinateEditPlugin::icon() const
{
    return QIcon();
}

QString GeoCoordinateEditPlugin::toolTip() const
{
    return QLatin1String("经纬度输入框");
}

QString GeoCoordinateEditPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool GeoCoordinateEditPlugin::isContainer() const
{
    return false;
}

QString GeoCoordinateEditPlugin::domXml() const
{
    return QLatin1String("<widget class=\"GeoCoordinateEdit\" name=\"geoCoordinateEdit\">\n</widget>\n");
}

QString GeoCoordinateEditPlugin::includeFile() const
{
    return QLatin1String("geocoordinateedit.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(geoCoordinateEditplugin, GeoCoordinateEditPlugin)
#endif // QT_VERSION < 0x050000
