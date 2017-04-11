/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains a subclass of the QGraphicsView
 **
 **	Contributors:
 **		Benjamin Tissoires <benjamin.tissoires@enac.fr>
 **
 **
 **	This program is provided to you as free software;
 **	you can redistribute it	and/or modify it under the terms of the
 **	GNU General Public License as published by the Free Software
 **	Foundation; either version 2 of the License, or (at your option)
 **	any later version.
 **/

#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = 0);
    ~GraphicsView();

    void setupView(QEvent *event);
    void moveEvent();

    int viewWidth() { return viewRect.width(); }
    int viewHeight() { return viewRect.height(); }

    QGraphicsScene *getScene() { return scene; }

    void moveEvent(QMoveEvent *event);

protected:
    void resizeEvent(QResizeEvent *event);

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
    void setFitToScreen (bool value) { fitToScreen = value; setupView(NULL);}
    void setFullscreen (bool value) { fullscreen = value; }
    void setMaximized (bool value) { maximized = value; }
};

#endif // GRAPHICSVIEW_H
