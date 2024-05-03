#include "DataAcquisition.h"

DataAcquisition::DataAcquisition(QObject *parent) :
    QObject(parent), m_timer(new QTimer), m_flow("flow",{0},{0})
{
    //GRAMsIntegrity["pressure"] = ControllerConnection::Offline;
    connect(m_timer, &QTimer::timeout, this, &DataAcquisition::processEvents);
}

void DataAcquisition::flowSetting(const QString &description, const QVariantMap& deviceSettings){
    if(description.isEmpty())
        return;
    AdvAIType a(description);
    a.setSettings(deviceSettings);
    m_flowMeter = new AdvantechAI(a);
    m_flowMeter->ConfigureDeviceTest();
    qDebug() << "Created flowMeter";
}
void DataAcquisition::valveSetting(const QString &description, const QVariantMap& deviceSettings){
    if(description.isEmpty())
        return; //
    AdvAIType a(description);
    a.setSettings(deviceSettings);
    m_flowValve = new AdvantechAO(a);
    m_flowValve->ConfigureDeviceTest();
    qDebug() << "Created Valve";
}

bool DataAcquisition::isFlowConnected(){
    if(!m_flowMeter) return false;
    return true;
}

void DataAcquisition::startReading(){
    //threshold = 0;
    if(m_flowMeter==nullptr||m_flowValve==nullptr)
        return;
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
    double point = 10 * voltage - 0; // range [0 - 5] V = [0 - 50] SLM H_2 -> 1V = 10 SLM H2
    return point;
} 

double DataAcquisition::getLastChanged(){
    return m_flow.y.last();
}

void DataAcquisition::openValve(){
    m_flowValve->setData(true);
}
void DataAcquisition::closeValve(){
    m_flowValve->setData(false);
}
