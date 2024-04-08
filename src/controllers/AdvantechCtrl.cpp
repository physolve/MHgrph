#include "AdvantechCtrl.h"

#include <QDebug>
#include <QBitArray>

AdvantechCtrl::AdvantechCtrl(const QString &name, QObject *parent) : 
	QObject(parent), m_name(name)
{
}
AdvantechCtrl::~AdvantechCtrl(){
	qDebug() << "Controller deleted";
}
void AdvantechCtrl::Initialization(){
    qDebug() << "Controller initialized";
}
void AdvantechCtrl::readData(){
	qDebug() << "I read data!";
}
/* Advantech Analog Input (pressure and temperature readings) */

AdvantechAI::AdvantechAI(const AdvAIType &info, QObject *parent) :
	AdvantechCtrl(info.m_deviceName,parent), m_info(info), m_instantAiCtrl(NULL), m_vector(8,0.0)
{
}

AdvantechAI::~AdvantechAI(){
    qDebug() << QString("Oh no, %1 was deleted").arg(m_info.m_deviceName);
}

void AdvantechAI::Initialization()
{
    std::wstring description = m_info.m_deviceName.toStdWString();
    DeviceInformation selected(description.c_str());

    InstantAiCtrl *instantAiCtrl = InstantAiCtrl::Create();
	ErrorCode errorCode = instantAiCtrl->setSelectedDevice(selected);

	if (errorCode != 0){
		QString str;
        QString des = QString::fromStdWString(description);
		return;
	}

	int channelCount = (instantAiCtrl->getChannelCount() < 16) ? 
		instantAiCtrl->getChannelCount() : 16;

	int logicChannelCount = instantAiCtrl->getChannelCount();

	m_info.m_channelStart = logicChannelCount;

	m_info.m_channelCount = channelCount;

	Array<ValueRange>* ValueRanges = instantAiCtrl->getFeatures()->getValueRanges();
	wchar_t		 vrgDescription[128];
	MathInterval ranges;
	for (int i = 0; i < ValueRanges->getCount(); i++)
	{
		errorCode = AdxGetValueRangeInformation(ValueRanges->getItem(i), 
			sizeof(vrgDescription), vrgDescription, &ranges, NULL);
		CheckError(errorCode);
		QString str = QString::fromWCharArray(vrgDescription);
		m_info.m_valueRanges.append(str);
	}

	instantAiCtrl->Dispose();
}

const AdvAIType& AdvantechAI::getInfo(){
	return m_info;
}

void AdvantechAI::ConfigureDeviceTest(){ // after accept

	if (m_instantAiCtrl==NULL)
	{
      m_instantAiCtrl = InstantAiCtrl::Create();
	}

    std::wstring description = m_info.m_deviceName.toStdWString();
    DeviceInformation selected(description.c_str());

    ErrorCode errorCode = m_instantAiCtrl->setSelectedDevice(selected);
	CheckError(errorCode);
	// add DIR ? profile
    std::wstring profile = m_info.m_profilePath.toStdWString();
    errorCode = m_instantAiCtrl->LoadProfile(profile.c_str());
    CheckError(errorCode);

	//Get channel max number. set value range for every channel.
	Array<AiChannel> *channels = m_instantAiCtrl->getChannels();

	Array<ValueRange>* valueRanges = m_instantAiCtrl->getFeatures()->getValueRanges();
	m_valueRange = valueRanges->getItem(m_info.m_valueRangeCh);
	
	for (int i = 0; i < channels->getCount(); i++)
	{
		channels->getItem(i).setValueRange(m_valueRange);
	}

	qDebug() << "INFO COUNT " << channels->getCount();
	resizeDataVector(m_info.m_channelCount); // ?
}

void AdvantechAI::resizeDataVector(uint8_t size){
	this->m_vector.resize(size);
}

void AdvantechAI::CheckError(ErrorCode errorCode)
{
    if (BioFailed(errorCode))
	{
		QString message = tr("Sorry, there are some errors occurred, Error Code: 0x") +
			QString::number(errorCode, 16).right(8).toUpper();
		qDebug() << QString("Warning Information %1").arg(message);
	}
}

void AdvantechAI::readData(){
	ErrorCode errorCode = Success;
	//qDebug() << "controller Data count = " << m_vector.count();
	errorCode = m_instantAiCtrl->Read(m_info.m_channelStart, m_info.m_channelCount, m_vector.data());
	CheckError(errorCode);
	if (errorCode != Success)
	{
		return;
	}
}

const QVector<double> AdvantechAI::getData(){ // const & ?
	//vector=scaledData;
	return m_vector;
}
