/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the callback when vid/pid is called
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

#ifndef VIDPIDACTION_H
#define VIDPIDACTION_H

#include <QObject>
#include <QAction>

class VidPidAction : public QAction
{
    Q_OBJECT
public:
    explicit VidPidAction(QString name, QObject *parent = 0);
    ~VidPidAction();

private:
    QString name;
    QString vid;
    QString pid;

signals:

public slots:
    void clicked();

};

#endif // VIDPIDACTION_H
