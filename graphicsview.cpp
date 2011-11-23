#include "graphicsview.h"
#include <QDebug>
#include <QResizeEvent>
#include <QMoveEvent>
#include <QDesktopWidget>
#include <QApplication>
#include <QMoveEvent>
#include <QMainWindow>

GraphicsView::GraphicsView(QWidget *parent) :
    QGraphicsView(parent),
    scene(new QGraphicsScene ()),
    viewRect(QRect ()),
    fullscreen(false),
    maximized(false),
    fitToScreen(false)
{
    setScene(scene);
}

GraphicsView::~GraphicsView()
{
    delete scene;
}

QRect GraphicsView::getAbsoluteGeometry()
{
    return QRect(mapToGlobal(QPoint(0,0)), geometry().size());
}

QRect GraphicsView::getScreenGeometry()
{
    QDesktopWidget *desktop = QApplication::desktop ();
    QRect screen_geom = desktop->screenGeometry(this);
    QRect ret = getAbsoluteGeometry();

    ret.translate(-screen_geom.topLeft());

    return ret;
}

void GraphicsView::moveEvent()
{
    setupView();
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    setupView();
}

void GraphicsView::moveEvent(QMoveEvent *event)
{
    setupView();
}

void GraphicsView::setupView()
{
    if (fitToScreen) {
        QDesktopWidget *desktop = QApplication::desktop ();
        viewRect = desktop->screenGeometry(this);
        setSceneRect(getScreenGeometry());
    } else {
        viewRect = rect();
        setSceneRect(viewRect);
    }
}
