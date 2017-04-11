/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the color widget
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

#include "colorwidget.h"
#include <QMouseEvent>

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

void ColorWidget::mouseDoubleClickEvent (QMouseEvent *event __attribute__((unused)))
{
    doubleClicked();
}

void ColorWidget::mouseReleaseEvent (QMouseEvent *event)
{
    if (event->button() == 2)
        doubleClicked();
}
