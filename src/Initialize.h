#pragma once

#include <QVariant>

class Initialize : public QObject
{
    Q_OBJECT
public:
    Initialize(QObject *parent = 0);
    Q_PROPERTY(QVariantMap advantechDeviceMap MEMBER m_advantechDeviceMap NOTIFY advantechDeviceMapChanged)
    Q_INVOKABLE QVariantMap advantechDeviceFill(const QString &description);
signals:
    void advantechDeviceMapChanged();
private:
    bool advantechDeviceCheck();
    QVariantMap m_advantechDeviceMap;
};