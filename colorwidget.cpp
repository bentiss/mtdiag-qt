#include "colorwidget.h"

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent)
{
}

void ColorWidget::setColor(QColor color)
{
    QPalette palette = QPalette();
    setAutoFillBackground(true);
    QBrush brush = QBrush(color);
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    palette.setBrush(QPalette::Active, QPalette::Window, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush);
    setPalette(palette);
}

void ColorWidget::mouseDoubleClickEvent (QMouseEvent *event)
{
    doubleClicked();
}
