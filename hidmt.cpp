/**	mtdiag, a multitouch diagnostic tool
 **
 **	The copyright owners for the contents of this file are:
 **		Ecole Nationale de l'Aviation Civile, France (2011-2012)
 **
 **	this files contains the GUI for hid-multitouch devices
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

#include "hidmt.h"
#include "ui_quirks.h"
#include "ui_protocol.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QDebug>
#include <QSplitter>
#include <iostream>
#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <errno.h>
#include "hid.h"
#include <unistd.h>

struct quirks {
    QString name;
    uint value;
};

static struct quirks QUIRKS[] = {
    {"NOT_SEEN_MEANS_UP"           , 1 << 0},
    {"SLOT_IS_CONTACTID"           , 1 << 1},
    {"CYPRESS"                     , 1 << 2},
    {"SLOT_IS_CONTACTNUMBER"       , 1 << 3},
    {"ALWAYS_VALID"                , 1 << 4},
    {"VALID_IS_INRANGE"            , 1 << 5},
    {"VALID_IS_CONFIDENCE"         , 1 << 6},
    {"EGALAX_XYZ_FIXUP"            , 1 << 7},
    {"SLOT_IS_CONTACTID_MINUS_ONE" , 1 << 8},
    {"NO_AREA"                     , 1 << 9},
    {"WIN_8_CERTIFIED"             , 1 << 10},
    {0,0}
};

HidMT::HidMT(Udev *udevMgr, UdevDevice *hid, QWidget *parent) :
    QWidget(parent),
    formQuirks(new Ui::Form_Quirks),
    formProtocol(new Ui::Form_Protocol),
    udevMgr(udevMgr),
    hid(hid),
    hiddev(0),
    sn(0),
    usages(0),
    prev_usages(0),
    usagesCount(0),
    mt_values(QList<struct MT::touch> ()),
    prev_mt_values(QList<struct MT::touch> ()),
    contactCount(0),
    fdHiddev(-1),
    eventIndex(0)
{
    QGridLayout *gridLayout = new QGridLayout(this);
    QSplitter *splitter = new QSplitter(this);
    splitter->setOrientation(Qt::Vertical);
    gridLayout->addWidget(splitter, 0, 0, 1, 1);
    formQuirks->setupUi(this);
    splitter->addWidget(formQuirks->dockWidget_quirks);
    quirksPath = hid->getSyspath();
    quirksPath.append("/quirks");

    readQuirks();

#if 0
    if (wasManuallyAdded()) {
        formProtocol->setupUi(this);
        splitter->addWidget(formProtocol->groupBox);
        formProtocol->progressBar->hide();

        hiddev = udevMgr->getHiddev(hid);

        if (hiddev) {
            fdHiddev = open (hiddev->getDevnode(), O_RDONLY);
            if (fdHiddev >= 0) {
                ::close (fdHiddev);
                fdHiddev = -1;
            } else {
                formProtocol->scrollAreaWidgetContents->setEnabled(false);
            }
        } else
            formProtocol->scrollAreaWidgetContents->setEnabled(false);
    }
#endif
}

HidMT::~HidMT()
{
    delete formQuirks;
    delete formProtocol;
    // hid should not be freed as it is done in the Qdevice class
    if (hiddev) {
        delete hiddev;
        if (sn)
            delete sn;
        if (fdHiddev >= 0)
            ::close (fdHiddev);
    }
    if (usages)
        free(usages);
    if (prev_usages)
        free(prev_usages);
}

void HidMT::expertMode(bool value)
{
    if (!value)
        formQuirks->dockWidget_quirks->hide();
    else
        formQuirks->dockWidget_quirks->show();
}

uint HidMT::readQuirks()
{
    QFile file (quirksPath);
    bool opened = file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in (&file);
    QString line;
    bool ok;
    uint quirks;
    QDockWidget *dw;

    if (!opened)
        goto failed;

    line = in.readLine();

    file.close();

    line = line.remove("\n");

    quirks = line.toUInt(&ok);

    if (!ok)
        goto failed;

    dw = formQuirks->dockWidget_quirks;
    for (uint i = 0; i < sizeof(QUIRKS)/sizeof(QUIRKS[0]); ++i) {
        struct quirks *quirk = &QUIRKS[i];
        if (quirks & quirk->value) {
            QRadioButton *radio;
            QCheckBox *checkbox;

            radio = dw->findChild<QRadioButton *>(QString("radioButton_%1").arg(quirk->name));
            if (radio)
                radio->setChecked(true);

            checkbox = dw->findChild<QCheckBox *>(QString("checkBox_%1").arg(quirk->name));
            if (checkbox)
                checkbox->setChecked(true);
        }
    }

    opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!opened) {
        formQuirks->dockWidget_quirks->setWindowTitle("Quirks (read only)");
        return quirks;
    }

    file.close();

    return quirks;

failed:
    formQuirks->groupBox_Slot->setEnabled(false);
    formQuirks->groupBox_Valid->setEnabled(false);
    formQuirks->groupBox_vendor->setEnabled(false);
    return 0;
}

void HidMT::updateQuirks ()
{
    uint quirks = 0;
    QDockWidget *dw = formQuirks->dockWidget_quirks;
    for (uint i = 0; i < sizeof(QUIRKS)/sizeof(QUIRKS[0]); ++i) {
        struct quirks *quirk = &QUIRKS[i];
        QRadioButton *radio;
        QCheckBox *checkbox;

        radio = dw->findChild<QRadioButton *>(QString("radioButton_%1").arg(quirk->name));
        if (radio && radio->isChecked())
            quirks += quirk->value;

        checkbox = dw->findChild<QCheckBox *>(QString("checkBox_%1").arg(quirk->name));
        if (checkbox && checkbox->isChecked())
            quirks += quirk->value;
    }

    QFile file (quirksPath);
    bool opened = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (opened) {
        QTextStream out(&file);
        out << quirks;
        file.close();
        qDebug() << "quirks updated to:" << quirks;
    } else {
        qDebug() << "quirks need update:\n$ echo" << quirks  << ">" << quirksPath;
    }
    readQuirks();
}

#define STORE_USAGE(field)  if (touch.field != 0) {\
    mt_values.append(touch);\
    prev_mt_values.append(prev_touch);\
    memset(&touch, 0, sizeof(touch));\
    memset(&prev_touch, 0, sizeof(prev_touch));\
    }\
    touch.field = &usages[i].value;\
    prev_touch.field = &prev_usages[i].value;

void HidMT::autoDetect(bool state)
{
    formProtocol->progressBar->setVisible(state);
    formProtocol->progressBar->setValue(0);
    if (!state) {
        if (fdHiddev >= 0) {
            delete sn;
            sn = 0;
            ::close (fdHiddev);
        }
        return;
    }

    fdHiddev = open (hiddev->getDevnode(), O_RDONLY);
    qDebug() << hiddev->getDevnode();
    if (fdHiddev >= 0) {
        sn = new QSocketNotifier(fdHiddev, QSocketNotifier::Read, this);
        QObject::connect(sn, SIGNAL(activated(int)), this, SLOT(events(int)));
    }

    if (mt_values.count() > 0) {
        // initialization already done
        return;
    }

    struct hiddev_devinfo devinfo;
    struct hiddev_report_info info;
    if (ioctl (fdHiddev, HIDIOCGDEVINFO, &devinfo) < 0)
        qDebug() << strerror(errno);
    QList< struct hiddev_event> list_usages;
    list_usages = QList< struct hiddev_event> ();

    memset(&info, 0, sizeof(struct hiddev_report_info));
    info.report_type = HID_REPORT_TYPE_INPUT;
    info.report_id = HID_REPORT_ID_FIRST;
    bool mt = false;
    while (!mt && ioctl (fdHiddev, HIDIOCGREPORTINFO, &info) == 0) {
        list_usages.clear();
        for (int field = 0 ; field < info.num_fields ; ++field) {
            struct hiddev_field_info field_info;
            memset(&field_info, 0, sizeof(struct hiddev_field_info));
            field_info.report_type = info.report_type;
            field_info.report_id = info.report_id;
            field_info.field_index = field;
            ioctl (fdHiddev, HIDIOCGFIELDINFO, &field_info);
            for (int usage_index = 0; usage_index < field_info.maxusage; ++usage_index) {
                struct hiddev_usage_ref usage;
                memset(&usage, 0, sizeof(struct hiddev_usage_ref));
                usage.report_type = info.report_type;
                usage.report_id = info.report_id;
                usage.field_index = field;
                usage.usage_index = usage_index;
                struct hiddev_event event = {0, 0};
                if (ioctl (fdHiddev, HIDIOCGUCODE, &usage) < 0) {
                    list_usages.append(event);
                    qDebug() << strerror(errno);
                    continue;
                }
                event.hid = usage.usage_code;
                list_usages.append(event);
                mt = mt || usage.usage_code == HID_DG_CONTACTID;
            }
        }
        info.report_id |= HID_REPORT_ID_NEXT;
    }

    usagesCount = list_usages.count();
    usages = (struct hiddev_event*)malloc (usagesCount * sizeof (struct hiddev_event));
    for (int i = 0; i < usagesCount; ++i) {
        usages[i].hid = list_usages[i].hid;
        usages[i].value = 0;
    }
    prev_usages = (struct hiddev_event*)malloc (usagesCount * sizeof (struct hiddev_event));
    memcpy(prev_usages, usages, usagesCount * sizeof (struct hiddev_event));

    struct MT::touch touch = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    struct MT::touch prev_touch = {0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (int i = 0; i < usagesCount; ++i) {
        switch (usages[i].hid) {
        case HID_DG_INRANGE:
            STORE_USAGE(inRange)
            break;
        case HID_DG_TIPSWITCH:
            STORE_USAGE(tipSwitch)
            break;
        case HID_DG_CONFIDENCE:
            STORE_USAGE(confidence)
            break;
        case HID_DG_CONTACTID:
            STORE_USAGE(contactID)
            break;
        case HID_DG_TIPPRESSURE:
            STORE_USAGE(pressure)
            break;
        case HID_GD_X:
            STORE_USAGE(x)
            break;
        case HID_GD_Y:
            STORE_USAGE(y)
            break;
        case HID_DG_WIDTH:
            STORE_USAGE(width)
            break;
        case HID_DG_HEIGHT:
            STORE_USAGE(height)
        }
    }
    mt_values.append(touch);
    prev_mt_values.append(prev_touch);

    qDebug() << usagesCount << mt_values.count();

}

bool HidMT::wasManuallyAdded()
{
    return true;
}

#define NUM_EVENTS 16

void HidMT::events(int fd)
{
    if (fd != fdHiddev)
        return;

    struct hiddev_event ev[NUM_EVENTS];
    int i, len = sizeof(ev);

    while (len == sizeof(ev))
    {
        len = read(fdHiddev, &ev, sizeof(ev));
        if (len <= 0)
        {
            if (errno == ENODEV) /* May happen after resume */
            {
                ::close(fdHiddev);
                fdHiddev = -1;
            } else if (errno != EAGAIN)
                std::cerr << "hiddev Read error: " << strerror(errno) << std::endl;
            break;
        }

        /* The kernel promises that we always only read a complete
         * event, so len != sizeof ev is an error. */
        if (len % sizeof(ev[0])) {
            std::cerr << "hiddev read error: " << strerror(errno) << std::endl;
            break;
        }

        for (i = 0; i < (int)(len/sizeof(ev[0])); i++) {
            processOneEvent(&ev[i]);
        }
    }
}

void HidMT::processOneEvent(struct hiddev_event *ev)
{
    while (usages[eventIndex].hid != ev->hid && eventIndex < usagesCount) {
        qDebug() << "missing " << QString::number(usages[eventIndex].hid, 16);
        usages[eventIndex++].value = 0;
    }
    if (eventIndex >= usagesCount) {
        processOneFrame();
        eventIndex = 0;
        contactCount = 0;
    }
    contactCount = ev->hid == HID_DG_CONTACTCOUNT ? ev->value : contactCount;
    usages[eventIndex++].value = ev->value;
    if (eventIndex >= usagesCount) {
        processOneFrame();
        eventIndex = 0;
        contactCount = 0;
    }

    qDebug() << eventIndex << usagesCount;

}

void HidMT::processOneFrame()
{
    int finger_count = 0;
    QPixmap check_icon = QPixmap(QString::fromUtf8(":/icons/check.png"));
    QPixmap fault_icon = QPixmap(QString::fromUtf8(":/icons/fault.png"));
    QPixmap question_icon = QPixmap(QString::fromUtf8(":/icons/question.png"));
    int progressBarValue = formProtocol->progressBar->value();

    if (!mt_values[0].tipSwitch) {
        formProtocol->icon_tipswitch->setPixmap(fault_icon);
        formProtocol->progressBar->setValue(100);
        formProtocol->pushButton->setChecked(false);
        return;
    }

    for (int slot = 0; slot < mt_values.count(); ++slot) {
        struct MT::touch *touch = &mt_values[slot];
        if (*touch->tipSwitch)
            finger_count++;
    }

    //if (formProtocol->icon_tipswitch->text() != "ok") {
        formProtocol->icon_tipswitch->setPixmap(finger_count <= contactCount ?
                                                check_icon : fault_icon);
//        formProtocol->icon_tipswitch->setText("ok");
//        progressBarValue += 25;
//        formProtocol->progressBar->setValue(progressBarValue);
//    }

    for (int slot = 0; slot < mt_values.count(); ++slot) {
        struct MT::touch *touch = &mt_values[slot];
        struct MT::touch *prev_touch;
        int old_slot = -1;
        for (int i = 0; i < prev_mt_values.count(); ++i) {
            struct MT::touch *prev = &prev_mt_values[i];
            if (*prev->contactID == *touch->contactID) {
                old_slot = i;
                prev_touch = prev;
                break;
            }
        }

        if (!prev_touch)
            continue;

        if (!*touch->tipSwitch && *prev_touch->tipSwitch) {
            if (touch->inRange &&
                    *touch->inRange && *prev_touch->inRange) {
                formQuirks->radioButton_VALID_IS_INRANGE->setChecked(true);
                formProtocol->icon_valid->setPixmap(check_icon);
            } else if (touch->confidence &&
                       *touch->confidence && *prev_touch->confidence) {
                formQuirks->radioButton_VALID_IS_CONFIDENCE->setChecked(true);
                formProtocol->icon_valid->setPixmap(check_icon);
            }  else {
                formProtocol->icon_valid->setPixmap(question_icon);
            }
        }
    }

//    qDebug() << "\ngot" << contactCount << "touches:";
//    for (int slot = 0; slot < mt_values.count(); ++slot) {
//        struct MT::touch *touch = &mt_values[slot];
//        qDebug() << "slot #" << slot;
//        if (touch->tipSwitch)
//            qDebug() << " TipSwitch:" << *touch->tipSwitch;
//        if (touch->x)
//            qDebug() << " X:" << *touch->x;
//        if (touch->y)
//            qDebug() << " Y:" << *touch->y;
//        if (touch->inRange)
//            qDebug() << " InRange:" << *touch->inRange;
//        if (touch->confidence)
//            qDebug() << " Confidence:" << *touch->confidence;
//        if (touch->width)
//            qDebug() << " Width:" << *touch->width;
//        if (touch->height)
//            qDebug() << " Height:" << *touch->height;
//        if (touch->pressure)
//            qDebug() << " Pressure:" << *touch->pressure;
//        if (touch->contactID)
//            qDebug() << " ContactID:" << *touch->contactID;
//    }

    memcpy(prev_usages, usages, usagesCount * sizeof (struct hiddev_event));
}
