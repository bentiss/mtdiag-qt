#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>

class ColorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColorWidget(QWidget *parent = 0);
    void setColor(QColor color);

private:

protected:
    void mouseDoubleClickEvent (QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void doubleClicked();
public slots:

};

#endif // COLORWIDGET_H
