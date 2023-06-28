#include "paint-utils.h"

#include <QSvgRenderer>
#include <QPainter>
#include <QRect>
#include <QPixmap>
#include <QIcon>

NBC_BEGIN_NAMESPACE
namespace utils {

static QByteArray writeSVGDom(const QString& iconPath, const QList<QPair<QString, QString>>& attributes) {
#if 0
    QFile f(iconPath);
    f.open(QFile::ReadOnly);

    QDomDocument dom;
    dom.setContent(f.readAll());
    f.close();

    auto pathNodes = dom.elementsByTagName("path");
    for (int i = 0; i < pathNodes.size(); ++i) {
        auto element = pathNodes.at(i).toElement();

        for (auto& p : attributes) {
            element.setAttribute(p.first, p.second);
        }
    }
    return dom.toByteArray();
#else
    Q_UNUSED(iconPath)
    Q_UNUSED(attributes)
    return QByteArray();
#endif
}

void drawIcon(const QString& str,
              QPainter* painter,
              const QRectF& bounds,
              const QList<QPair<QString, QString>>& attribute) {
    if (str.endsWith(".SVG", Qt::CaseInsensitive)) {
        if (attribute.isEmpty()) {
            QSvgRenderer render(str);
            auto rectf = bounds;
            if (!bounds.size().isValid()) {
                rectf.setSize(render.defaultSize());
            }
            render.render(painter, rectf);
        } else {
            QSvgRenderer render(writeSVGDom(str, attribute));
            auto rectf = bounds;
            if (!bounds.size().isValid()) {
                rectf.setSize(render.defaultSize());
            }
            render.render(painter, rectf);
        }
    } else {
        QIcon icon(str);
        auto rect = bounds.toRect();
        auto image = icon.pixmap(rect.width(), rect.height());
        painter->drawPixmap(rect, image);
    }
}

QSize getSvgDefaultSize(const QString& iconPath) {
    if (!iconPath.endsWith(".SVG", Qt::CaseInsensitive))
        return QSize();
    QSvgRenderer render(iconPath);
    return render.defaultSize();
}
} // namespace utils
NBC_END_NAMESPACE
