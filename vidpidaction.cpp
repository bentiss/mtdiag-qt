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

#include "vidpidaction.h"
#include <QFile>
#include <QTextStream>

static const QString new_idPath =
        "/sys/module/hid_multitouch/drivers/hid:hid-multitouch/new_id";

VidPidAction::VidPidAction(QString name, QObject *parent) :
    QAction(parent),
    name(name)
{
    QRegExp rx("(....):(....).*");
    int pos = rx.indexIn(name);
    if (pos > -1) {
        vid = rx.cap(1);
        pid = rx.cap(2);
    } else {
        vid = "";
        pid = "";
    }
    setText(name);

    QFile file (new_idPath);
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (opened) {
        file.close();
        connect (this, SIGNAL(triggered()), this, SLOT(clicked()));
    } else {
        setEnabled(false);
    }

}

VidPidAction::~VidPidAction()
{

}

void VidPidAction::clicked()
{
    QFile file (new_idPath);
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (opened) {
        QTextStream out(&file);
        out << QString("3 %1 %2 1").arg(vid, pid);
        file.close();
        setEnabled(false);
    }
}
