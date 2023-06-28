/**
 * File: paint_utils.h
 * Author: NiceBlueChai
 * Date: 2023/06/20
 * Description: Paint util functions
 *
 * Copyright (C) 2023 NiceBlueChai  bluechai@qq
 */
#pragma once

#include <QList>
#include <QPair>
#include "global/nbc.h"
class QPainter;
class QString;
class QRectF;
class QSize;

NBC_BEGIN_NAMESPACE
namespace utils {
void drawIcon(const QString& icon,
              QPainter* painter,
              const QRectF& bounds,
              const QList<QPair<QString, QString>>& attribute = QList<QPair<QString, QString>>());
QSize getSvgDefaultSize(const QString& iconPath);
} // namespace utils
NBC_END_NAMESPACE

