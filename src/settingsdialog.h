#pragma once

#include <QtSerialBus/qtserialbusglobal.h>
#include <QSerialPort>
#include <QVariant>

class SettingsDialog : public QObject{
    Q_OBJECT
    Q_PROPERTY(QVariantMap serialPortList READ serialPortListRead CONSTANT)
public:
    struct Settings {
        QString name;// = "serialInfo";
        
        int m_parity;// = QSerialPort::EvenParity;// QSerialPort::EvenParity;
        int m_baud;// = QSerialPort::Baud19200;
        int m_dataBits;// = QSerialPort::Data8;
        int m_stopBits;// = QSerialPort::OneStop;
        int m_responseTime;// = 1000;
        int m_numberOfRetries;// = 3;

        int m_serverEdit;
        QString m_portEdit;
        
    };
    explicit SettingsDialog(QObject* parent = nullptr);
    ~SettingsDialog();

    Settings settings(const QString &c_name) const;

    QVariantMap serialPortListRead() const;

    Q_INVOKABLE void apply(const QVariantMap &map, const QString &c_name);

    bool isFlowMeterConnected();

private:
    void fillPortsInfo();
    void updateSettings();

    QMap<QString,Settings> m_settingsMap;
    QVariantMap m_serialPortList;

    bool flowMeterConnected;
};
