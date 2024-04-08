#pragma once

#include <QtCore/QObject>
#include "../../lib/bdaqctrl.h"
#include "ControllerInfo.h"
using namespace Automation::BDaq;


class AdvantechCtrl : public QObject
{
    Q_OBJECT
public:
    AdvantechCtrl(const QString &name = "unknown", QObject *parent = nullptr);
    virtual ~AdvantechCtrl();
    virtual void Initialization();
    virtual void readData(); 
protected:
    QString m_name;

};

class AdvantechAI : public AdvantechCtrl
{
    Q_OBJECT
public:
    AdvantechAI(const AdvAIType &info, QObject *parent = nullptr); 
    virtual ~AdvantechAI();
    void Initialization() ; //override
    void initialInfo();
    void ConfigureDeviceTest();
	void CheckError(ErrorCode errorCode);
    const AdvAIType& getInfo(); // move to base class
    void resizeDataVector(uint8_t size);
    void readData() override;
    const QVector<double> getData();

//Q_SIGNALS:

public slots:
    //void settingAccepted();
    //void generateData(int type, int rowCount, int colCount);

private:
    AdvAIType m_info;
    
    ValueRange m_valueRange;
    InstantAiCtrl* m_instantAiCtrl; // change to smart pointer or initialize inside class 
    QVector<double> m_vector;
	//double scaledData[16];
};