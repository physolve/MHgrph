#include "scalarcalc.h"
#include <QVariant>
#include <QFile>

ScalarCalc::ScalarCalc(QObject *parent) : QObject(parent), isExposure(false), clockTime("00:00:00"), scalarStr("0.000"), runCnt(1)
{
    
}

ScalarCalc::~ScalarCalc(){

}

void ScalarCalc::onFlowToScalarClicked(const QString &name, bool s)
{
    if(s){
        startCalc(name);
    }
    else{
        stopCalc();
    }
}

void ScalarCalc::startCalc(const QString &name){
    this->m_currentCollection = name;
    this->m_timeStart = QTime::currentTime();
    this->m_currentScalar = 0;
    this->isExposure = true;
}

void ScalarCalc::stopCalc(){
    this->isExposure = false;
    auto duration = m_timeStart.secsTo(QTime::currentTime());
    this->dataCollection << DataStore({ m_currentCollection,
        m_timeStart.toString("hh:mm:ss"),
        QTime(0,0,0,0).addSecs(duration).toString("hh:mm:ss"),
        m_currentScalar});
    emit dataCollectionChanged();
    this->runCnt = dataCollection.count() + 1;
    emit runCntChanged();
}

void ScalarCalc::calcScalar(double y0, double y1, qreal x0, qreal x1){
    double scalar = (y0+y1)/2*(x1-x0)/60;
    this->m_currentScalar += scalar;
    scalarStr = QString::number(this->m_currentScalar, 'g', 5);
    emit scalarStrChanged();
    auto duration = m_timeStart.secsTo(QTime::currentTime());
    clockTime = QTime(0,0,0,0).addSecs(duration).toString("hh:mm:ss");
    emit clockTimeChanged();
}

double ScalarCalc::getCurrentScalar() const{
    return m_currentScalar;
}

QString ScalarCalc::getClockTime() const{
    return clockTime;
}

QString ScalarCalc::getScalarStr() const{
    return scalarStr;
}

int ScalarCalc::getRunCnt() const{
    return runCnt;
}

QVariantList ScalarCalc::getDataCollection() const
{
    QVariantList collection;
    for (const DataStore &i : dataCollection){
        QVariantList data;
        data.append(QVariant::fromValue(i.name));
        data.append(QVariant::fromValue(i.timeStart));
        data.append(QVariant::fromValue(i.duration));
        data.append(QVariant::fromValue(i.currentScalar));
        collection.append(QVariant::fromValue(data));
    }
    return collection;
}

void ScalarCalc::saveResults(const QUrl &fileName)
{
    QFile saveFile(fileName.toLocalFile());
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }
    QString header = QString::fromUtf8("Имя запуска\tНачало    \tДлительность\tЗначение");
    QTextStream in(&saveFile);
    in << header << "\n";
    for (const DataStore &i : dataCollection){
        in << i.name << '\t'<< i.timeStart << '\t'<< i.duration << '\t'<< QString::number(i.currentScalar, 'g', 5) << '\n';
    }
    saveFile.close();
}