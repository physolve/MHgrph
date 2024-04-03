#pragma once

#include "settingsdialog.h"
#include <QModbusDataUnit>
#include <QElapsedTimer>
#include <QTimer>

class SettingsDialog;
class QModbusClient;
class QModbusReply;

struct Data {
    Data() {}
    Data( const QString& name, QList<quint64> x, QList<double> y)
        : name(name), x(x), y(y) {}
    void clearPoints(){
        x.clear();
        y.clear();
    }
    QString name;
    QList<quint64> x; // one second data
    QList<double> y; // one second data
};

struct ModbusVariable{
    int m_startAddress;
    int m_readSize;
};

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentType MEMBER m_currentType)
    Q_PROPERTY(int customAdress MEMBER m_customAdress)
    Q_PROPERTY(int customSize MEMBER m_customSize)
public:
    Controller(const SettingsDialog::Settings &settings, QObject *parent = nullptr);
    ~Controller();
    void setSettings(const SettingsDialog::Settings &settings);
    void initController();
    void connectFlow();
    void requestFlow(const ModbusVariable &modbusVariable);
    void startReading();
    void stopReading();
    double getLastChanged();

private slots:
    void processEvents();
    void onModbusStateChanged(int state);
    void onReadReady();
signals:
    void infoUpdate(QStringList);
    void logChanged(QString);
    void pointsChanged(const QList<quint64> &x, const QList<double> &y);
    void lastChanged(double y);
private:
    SettingsDialog::Settings m_settings;

    QTimer* m_timer;
    QString logText;

    QString m_currentType;
    int m_customAdress;
    int m_customSize;

    QModbusDataUnit readRequest(int adress, int size) const;
    QModbusDataUnit writeRequest() const;

    QModbusClient *modbusDevice = nullptr;
    QElapsedTimer m_timePassed;
    Data m_points;

    void setLogText(const QString &text);

    uint8_t threshold;

    QMap<QString, ModbusVariable> modbusVariableMap;

    QString checkType(int startAdress);
    void replyTreat(int adress, const QList<quint16> &values);
    void replyTreat(const QString &type, const QList<quint16> &values);

};