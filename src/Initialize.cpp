#include "Initialize.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariantList>

#include "controllers/AdvantechCtrl.h"
#include "../lib/bdaqctrl.h"
using namespace Automation::BDaq;

Initialize::Initialize(QObject *parent) :
    QObject(parent)
{
    auto checkPass = advantechDeviceCheck();
}

bool Initialize::advantechDeviceCheck(){
    auto startCheckInstance = InstantDoCtrl::Create(); // does it work with every Adv controller?
    auto allSupportedDevices = startCheckInstance->getSupportedDevices();
    if (allSupportedDevices->getCount() == 0)
    {
        qDebug() << "No advantech devices connected";
        return false;
    }
    QVariantMap deviceMap;
    for(int i = 0; i < allSupportedDevices->getCount(); i++){
        DeviceTreeNode const &node = allSupportedDevices->getItem(i);
        qDebug("%d, %ls", node.DeviceNumber, node.Description);
        auto advantechDescription = QString::fromWCharArray(node.Description).split(',');
        auto tempName = advantechDescription.value(0);
        auto tempBID = advantechDescription.value(1);

        // it doesn't fit expectations (it should show virtual BID to distinguish from non-virtual and put into BID)
        //qDebug() << "CHECK INDEX FOR BID to SHOW Virtual " << node.ModulesIndex;

        deviceMap.insert(tempBID,tempName);
    }
    m_advantechDeviceMap = deviceMap;
    startCheckInstance->Dispose();
    allSupportedDevices->Dispose();
    return true;
}

QVariantMap Initialize::advantechDeviceFill(const QString &description){
    QVariantMap advantechDeviceSettings;
    AdvAIType a(description);
    auto demoFlow = AdvantechAI(a);
    demoFlow.Initialization();
    a = demoFlow.getInfo();
    advantechDeviceSettings["flow"] = a.getSettings();
    auto demoValve = AdvantechAO(a);
    demoValve.Initialization();
    a = demoValve.getInfo();
    advantechDeviceSettings["valve"] = a.getSettings();
    
    return advantechDeviceSettings;
}