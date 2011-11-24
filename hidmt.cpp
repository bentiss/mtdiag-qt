#include "hidmt.h"
#include "ui_quirks.h"
#include <QRadioButton>
#include <QCheckBox>
#include <QDebug>
#include <QFile>

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
    {}
};

HidMT::HidMT(UdevDevice *hid, QWidget *parent) :
    QWidget(parent),
    formQuirks(new Ui::Form_Quirks),
    hid(hid)
{
    formQuirks->setupUi(this);
    quirksPath = hid->getSyspath();
    quirksPath.append("/quirks");
    readQuirks();
}

HidMT::~HidMT()
{
    delete formQuirks;
    // hid should not be freed as it is done in the Qdevice class
}

void HidMT::expertMode(bool value)
{
    if (!value)
        formQuirks->dockWidget_quirks->hide();
    else
        formQuirks->dockWidget_quirks->show();
}

void HidMT::readQuirks()
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
    for (int i = 0; i < sizeof(QUIRKS)/sizeof(QUIRKS[0]); ++i) {
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
        return;
    }

    file.close();

    return;

failed:
    formQuirks->groupBox_Slot->setEnabled(false);
    formQuirks->groupBox_Valid->setEnabled(false);
    formQuirks->groupBox_vendor->setEnabled(false);
}

void HidMT::updateQuirks ()
{
    uint quirks = 0;
    QDockWidget *dw = formQuirks->dockWidget_quirks;
    for (int i = 0; i < sizeof(QUIRKS)/sizeof(QUIRKS[0]); ++i) {
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
