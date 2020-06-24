#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE

class Paintarea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void changeBrush();
    void toCenter();

private:
    typedef void (MainWindow::*Member)();

    void createActions();
    void createMenus();
    void loadPlugins();
    void populateMenus(QObject *plugin);
    void addToMenu(QObject *plugin, const QStringList& texts, QMenu *menu,
                   Member member, QActionGroup *actionGroup = nullptr);
private:
    Paintarea *paintArea{nullptr};
    QScrollArea *scrollArea{nullptr};

    QMenu *brushMenu{nullptr};
    QActionGroup *brushActionGroup{nullptr};

};
#endif // MAINWINDOW_H
