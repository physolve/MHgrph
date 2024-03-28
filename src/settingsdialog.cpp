#include "settingsdialog.h"

#include <QSerialPortInfo>

static const char blankString[] = QT_TRANSLATE_NOOP("SettingsDialog", "N/A");

SettingsDialog::SettingsDialog(QObject* parent) : 
QObject(parent), flowMeterConnected(false)
{
    fillPortsInfo();

    for(const QString &c_name : m_serialPortList.keys()){
        QVariantMap map;
        map["serialPortInfo"] = m_serialPortList[c_name].toStringList().first();
        map["baudRate"] = QSerialPort::Baud19200;
        map["dataBitsBox"] = QSerialPort::Data8;
        map["parityBox"] = QSerialPort::EvenParity;
        map["stopBitsBox"] = QSerialPort::OneStop;
        apply(map,c_name);
    }
}

SettingsDialog::~SettingsDialog()
{
}

SettingsDialog::Settings SettingsDialog::settings(const QString &c_name) const
{
    return m_settingsMap[c_name];
}

void SettingsDialog::fillPortsInfo(){
    m_serialPortList.clear();
    QString description;
    QString manufacturer;
    QString serialNumber;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        description = info.description();
        manufacturer = info.manufacturer();
        serialNumber = info.serialNumber();
        list << info.portName()
             << (!description.isEmpty() ? description : blankString)
             << (!manufacturer.isEmpty() ? manufacturer : blankString)
             << (!serialNumber.isEmpty() ? serialNumber : blankString)
             << info.systemLocation()
             << (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
             << (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);
        qDebug() << info.portName();
        QString c_name = "unknown";
        if(description.startsWith("Silicon")){
            c_name = "flow";
            flowMeterConnected = true;
        }
        m_serialPortList[c_name] = list;
    }
}

QVariantMap SettingsDialog::serialPortListRead() const{
    return m_serialPortList;
}

void SettingsDialog::apply(const QVariantMap &map, const QString &c_name) // names for what?
{
    m_settingsMap[c_name].name = map["serialPortInfo"].toString();

    m_settingsMap[c_name].m_baud = static_cast<QSerialPort::BaudRate>(map["baudRate"].toInt());
    m_settingsMap[c_name].m_dataBits = static_cast<QSerialPort::DataBits>(map["dataBitsBox"].toInt());
    m_settingsMap[c_name].m_parity = static_cast<QSerialPort::Parity>(map["parityBox"].toInt());
    m_settingsMap[c_name].m_stopBits = static_cast<QSerialPort::StopBits>(map["stopBitsBox"].toInt());

    m_settingsMap[c_name].m_portEdit = map["portEdit"].toString();
    m_settingsMap[c_name].m_serverEdit = map["serverEdit"].toInt();
    m_settingsMap[c_name].m_responseTime = map["responseTime"].toInt();
    m_settingsMap[c_name].m_numberOfRetries = map["numberOfRetries"].toInt();
}

bool SettingsDialog::isFlowMeterConnected(){
    return flowMeterConnected;
}
