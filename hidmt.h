#ifndef HIDMT_H
#define HIDMT_H

#include <QWidget>
#include "udevdevice.h"

namespace Ui {
class Form_Quirks;
}

class HidMT : public QWidget
{
    Q_OBJECT
public:
    explicit HidMT(UdevDevice *hid, QWidget *parent = 0);
    ~HidMT();

    void expertMode (bool value);

private:
    Ui::Form_Quirks *formQuirks;
    UdevDevice *hid;
    QString quirksPath;
    void readQuirks();

signals:

public slots:
    void updateQuirks ();

};

#endif // HIDMT_H
