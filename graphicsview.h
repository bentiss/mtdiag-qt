#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);
    ~GraphicsView();

    void setupView();
    void moveEvent();

    int viewWidth() { return viewRect.width(); }
    int viewHeight() { return viewRect.height(); }

    QGraphicsScene *getScene() { return scene; }

protected:
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);

private:
    QGraphicsScene *scene;
    QRect viewRect;

    bool fullscreen;
    bool maximized;
    bool fitToScreen;

    QRect getAbsoluteGeometry();
    QRect getScreenGeometry();

signals:

public slots:
    void setFitToScreen (bool value) { fitToScreen = value; setupView();}
    void setFullscreen (bool value) { fullscreen = value; }
    void setMaximized (bool value) { maximized = value; }
};

#endif // GRAPHICSVIEW_H
