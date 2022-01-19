/**
 * \file gui-util.h
 * ©2021 NiceBlueChai <bluechai@qq.com>
 *
 * \brief 界面工具函数
 */
#ifndef GUIUTILS_H
#define GUIUTILS_H
#include <QPoint>

class QWidget;

namespace nbc {
namespace utils {

/**
 * @brief getWindowTopLeftPosCenterOfScreen
 * @param width   窗口的宽
 * @param height  窗口的高
 * @param hasTitleBar 是否将（系统默认）标题栏的高度算进去 如果是 往上移动标题栏个高度
 * @return 返回窗口左上角点的坐标
 */
QPoint getWindowTopLeftPosCenterOfScreen(int width, int height, bool hasTitleBar = true);
inline QPoint getWindowTopLeftPosCenterOfScreen(const QWidget& w);
/**
 * @brief moveToScreenCenter 将窗口移动到屏幕中央 通过 getWindowTopLeftPosCenterOfScreen 函数计算窗口左上角坐标并移动过去
 * @param w 目标窗口
 */
void moveToScreenCenter(QWidget& w);
}
}

#endif // GUIUTILS_H
