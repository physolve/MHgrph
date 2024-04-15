#include "DataAcquisition.h"

DataAcquisition::DataAcquisition(QObject *parent) :
    QObject(parent), m_timer(new QTimer), m_flow("flow",{0},{0})
{
    //GRAMsIntegrity["pressure"] = ControllerConnection::Offline;
    connect(m_timer, &QTimer::timeout, this, &DataAcquisition::processEvents);
}

void DataAcquisition::advantechDeviceSetting(const QString &description, const QVariantMap& deviceSettings){
    AdvAIType a(description);
    a.setSettings(deviceSettings);
    m_flowMeter = new AdvantechAI(a);
    m_flowMeter->ConfigureDeviceTest();
    qDebug() << "Created flowMeter";
}

bool DataAcquisition::isFlowConnected(){
    if(!m_flowMeter) return false;
    return true;
}

void DataAcquisition::startReading(){
    //threshold = 0;
    m_timer->start(1000);
    m_timePassed.start();
}

void DataAcquisition::stopReading(){
    m_timer->stop();
    m_timePassed.restart();
    m_flow.clearPoints();
}

void DataAcquisition::processEvents(){
    m_flowMeter->readData();

    auto voltageFlow = m_flowMeter->getData().at(0);
    auto point_flow = filterData_flow(voltageFlow);
    m_flow.addPoint(m_timePassed.elapsed()/1000, point_flow);

    emit pointsFlowChanged(m_flow.x, m_flow.y);
}

const double DataAcquisition::filterData_flow(double voltage){
    double point = 1 * voltage - 0;
    return point;
} 

double DataAcquisition::getLastChanged(){
    return m_flow.y.last();
}

