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
