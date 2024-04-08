#pragma once

#include "controllers/AdvantechCtrl.h"

class DataAcquisition : public QObject
{
    Q_OBJECT
public:
    explicit DataAcquisition(QObject *parent = 0);
    //QVariantMap profileJson ();
    void processEvents(); // wierdly written
    QVector<double> getMeasures();
    Q_INVOKABLE void advantechDeviceSetting(const QString &description, const QVariantMap& deviceSettings);
    //Q_INVOKABLE void advantechDeviceSetting(const QString &description, const QString &type, const QVariantMap& deviceSettings);
    Q_INVOKABLE void testRead();
private:
    AdvantechAI* m_flowMeter;
};