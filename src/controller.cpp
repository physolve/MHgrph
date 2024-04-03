#include "controller.h"
#include <QModbusRtuSerialClient>

Controller::Controller(const SettingsDialog::Settings &settings, QObject* parent) : 
    QObject(parent), m_settings(settings), m_timer(new QTimer), m_points("Flow",{0},{0}),
    modbusDevice(new QModbusRtuSerialClient(this))
{
    // if (modbusDevice) {
    //     modbusDevice->disconnectDevice();
    //     delete modbusDevice;
    //     modbusDevice = nullptr;
    // }
    
    connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error) {
        setLogText(modbusDevice->errorString()); // log under
    });

    if (!modbusDevice) {
        //ui->connectButton->setDisabled(true); //?
        setLogText("Could not create Modbus client."); // log under
    } else {
        connect(modbusDevice, &QModbusClient::stateChanged,
                this, &Controller::onModbusStateChanged);
    }

    connect(m_timer, &QTimer::timeout, this, &Controller::processEvents);

    setLogText("Click connect");

    modbusVariableMap["temperature"] = {41272,2};
    modbusVariableMap["flow"] = {41216,2};
    //modbusVariableMap["anything"] = {0,0};
}

Controller::~Controller(){
    if (modbusDevice)
        modbusDevice->disconnectDevice();
    m_timer->stop();
    delete modbusDevice;
    delete m_timer;
}

void Controller::setSettings(const SettingsDialog::Settings &settings){
    m_settings = settings;
}

void Controller::connectFlow(){
    if (!modbusDevice)
        return;

    setLogText(""); // log
    if (modbusDevice->state() != QModbusDevice::ConnectedState) {
       
        modbusDevice->setConnectionParameter(QModbusDevice::SerialPortNameParameter, m_settings.m_portEdit);
        modbusDevice->setConnectionParameter(QModbusDevice::SerialParityParameter, m_settings.m_parity); // from settings
        modbusDevice->setConnectionParameter(QModbusDevice::SerialBaudRateParameter, m_settings.m_baud); // from settings
        modbusDevice->setConnectionParameter(QModbusDevice::SerialDataBitsParameter, m_settings.m_dataBits); // from settings
        modbusDevice->setConnectionParameter(QModbusDevice::SerialStopBitsParameter, m_settings.m_stopBits); // from settings

        modbusDevice->setTimeout(m_settings.m_responseTime);
        modbusDevice->setNumberOfRetries(m_settings.m_numberOfRetries);
        if (!modbusDevice->connectDevice()) {
            setLogText(tr("Connect failed: ") + modbusDevice->errorString()); // log
        } 
        else {
            //ui->actionConnect->setEnabled(false); // ui
            //ui->actionDisconnect->setEnabled(true); // ui
            setLogText("Connected to " + m_settings.m_portEdit + "\nTemperature address: 41272 (x2)\nFlow address: 41216 (x2)");
        }
    }
    else {
        modbusDevice->disconnectDevice();
        //ui->actionConnect->setEnabled(true); // ui
        //ui->actionDisconnect->setEnabled(false); // ui
    }
}

void Controller::startReading(){
    threshold = 0;
    m_timer->start(1000);
    m_timePassed.start();
}
void Controller::stopReading(){
    m_timer->stop();
    //m_timePassed.restart(); ?
    m_points.clearPoints();
}

void Controller::processEvents(){

    if(modbusVariableMap.contains(m_currentType)){
        requestFlow(modbusVariableMap[m_currentType]);
    }
    else{
        auto customType = ModbusVariable{m_customAdress, m_customSize};
        requestFlow(customType);
    } 
}

void Controller::requestFlow(const ModbusVariable &modbusVariable){
    if (!modbusDevice)
        return;
    //m_readValue.clear(); clear buffer
    setLogText(""); // log

    if (auto *reply = modbusDevice->sendReadRequest(
        readRequest(modbusVariable.m_startAddress, modbusVariable.m_readSize), 
        m_settings.m_serverEdit)) { // ui server address
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished, this, &Controller::onReadReady);
        else
            delete reply; // broadcast replies return immediately
    } else {
        setLogText(tr("Read error: ") + modbusDevice->errorString()); // log
        m_timer->stop();
        //send to qml change switch
    }
}

QModbusDataUnit Controller::readRequest(int adress, int size) const
{
    const auto table = QModbusDataUnit::HoldingRegisters;

    //int startAddress = m_startAddress;
    //Q_ASSERT(startAddress >= 0 && startAddress < 10); // check address 10

    // do not go beyond 10 entries
    quint16 numberOfEntries = quint16(size); // check address
    return QModbusDataUnit(table, adress, numberOfEntries);
}

void Controller::onModbusStateChanged(int state)
{
    bool connected = (state != QModbusDevice::UnconnectedState);
    //ui->actionConnect->setEnabled(!connected); // ui
    //ui->actionDisconnect->setEnabled(connected); // ui

    //if (state == QModbusDevice::UnconnectedState)
        //ui->connectButton->setText(tr("Connect")); // ui
    //else if (state == QModbusDevice::ConnectedState)
        //ui->connectButton->setText(tr("Disconnect")); // ui
}

void Controller::onReadReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
    if (!reply)
        return;
    
    if (reply->error() == QModbusDevice::NoError) {
        const QModbusDataUnit unit = reply->result();

        auto type = checkType(unit.startAddress());

        if(type.isEmpty())
            replyTreat(unit.startAddress(), unit.values());
        else
            replyTreat(type, unit.values());
    }
    else if (reply->error() == QModbusDevice::ProtocolError) {
        setLogText(tr("Read response error: %1 (Modbus exception: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->rawResult().exceptionCode(), -1, 16)); // log
        
        //m_points.y.append(0);
        m_timer->stop();
        //send to qml change switch
    } else {
        setLogText(tr("Read response error: %1 (code: 0x%2)").
                                    arg(reply->errorString()).
                                    arg(reply->error(), -1, 16)); // log
        //m_points.y.append(0);
        m_timer->stop();
        //send to qml change switch
    }
        
    reply->deleteLater();
}

QString Controller::checkType(int startAdress){
    for(const auto &type : modbusVariableMap.keys()){
        if(modbusVariableMap[type].m_startAddress == startAdress)
            return type;
    }
    return QString();
}

void Controller::replyTreat(int adress, const QList<quint16> &values){
    QStringList readValue;
    int c_adress = adress;
    for(const auto &value : values){
        const QString entry = tr("Address: %1, Value: %2").arg(c_adress)
                                    .arg(QString::number(value));
        readValue << entry;
        c_adress++;
    }
    emit infoUpdate(readValue);
}

void Controller::replyTreat(const QString &type, const QList<quint16> &values){
    m_points.x.append(m_timePassed.elapsed()/1000);

    double fiteredVal = 0;
    double magnifier = 1;
    QString msg;  
    QString unit;
    if(type == "temperature"){
        magnifier = 1;
        msg = "Читаю температуру: ";
        unit = " °C";
    }
    else if(type == "flow"){
        magnifier = 1;
        msg = "Текущий поток: ";
        unit = " норм. л./мин";
    }

    if(values.count() == 2){
        float x;
        unsigned long *p;

        p = (unsigned long*)&x;
        *p = (unsigned long)values.at(0)<<16 | values.at(1);

        fiteredVal = x * magnifier;
    }
    else {
        fiteredVal = values.at(0);
    }

    m_points.y.append(fiteredVal);

    setLogText(msg + QString::number(m_points.y.last()) + unit);
    
    emit pointsChanged(m_points.x,m_points.y);
    //m_writeLog.writeLine(QString("%1\t%2").arg(m_points.x.last()).arg(m_points.y.last()));
    
    //setData(m_readValue); 

}

void Controller::setLogText(const QString &text)
{
    if (text != logText)
    {
        logText = text;
        emit logChanged(text);
    }
}

double Controller::getLastChanged(){
    return m_points.y.last();
}