#include "DataAcquisition.h"

DataAcquisition::DataAcquisition(QObject *parent) :
    QObject(parent)
{
    //GRAMsIntegrity["pressure"] = ControllerConnection::Offline;
}

void DataAcquisition::advantechDeviceSetting(const QString &description, const QVariantMap& deviceSettings){
    
    AdvAIType a(description);
    a.setSettings(deviceSettings);
    m_flowMeter = new AdvantechAI(a);
    m_flowMeter->ConfigureDeviceTest();
    qDebug() << "Created flowMeter";
}

void DataAcquisition::testRead(){
    m_flowMeter->readData();
    qDebug() << m_flowMeter->getData();
}

void DataAcquisition::processEvents(){
    m_flowMeter->readData();
}

QVector<double> DataAcquisition::getMeasures(){ // const & > does it know which channel in which place?
    return m_flowMeter->getData();
}

