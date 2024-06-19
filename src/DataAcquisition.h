#pragma once

#include <QElapsedTimer>
#include <QTimer>
#include "controllers/AdvantechCtrl.h"
#include "scalarcalc.h"

struct Data {
    Data() {}
    Data( const QString& name, QList<quint64> x, QList<double> y)
        : name(name), x(x), y(y) {}
    void clearPoints(){
        x.clear();
        y.clear();
        x.append(0);
        y.append(0);
    }
    void addPoint(const quint64 &val_x, const double &val_y){
        if(x.count() > 120){
            x.removeFirst();
            y.removeFirst();
        }
        x.append(val_x);
        y.append(val_y);      
    }
    QString name;
    QList<quint64> x; // one second data
    QList<double> y; // one second data
};

class DataAcquisition : public QObject
{
    Q_OBJECT
public:
    explicit DataAcquisition(QObject *parent = 0);
    Q_INVOKABLE void flowSetting(const QString &description, const QVariantMap& deviceSettings);
    Q_INVOKABLE void valveSetting(const QString &description, const QVariantMap& deviceSettings);
    ScalarCalc* getScalarCalcObject();
    //Q_INVOKABLE void onTestClicked();
    //QVariantMap profileJson ();
    bool isFlowConnected();
    void startReading();
    void stopReading();
    void processEvents(); // wierdly written
    double getLastChanged();
    void openValve();
    void closeValve();

signals:
    void pointsFlowChanged(const QList<quint64> &x, const QList<double> &y);
private:
    const double filterData_flow(double voltage);
    AdvantechAI* m_flowMeter;
    AdvantechAO* m_flowValve;
    Data m_flow;
    QTimer* m_timer;
    QElapsedTimer m_timePassed;
    ScalarCalc flowToVolume;
};