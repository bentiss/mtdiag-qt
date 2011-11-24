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
