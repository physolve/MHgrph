#pragma once
#include <QDebug>
#include <QTime>
#include <QUrl>
struct DataStore{
    QString name;
    QString timeStart;
    QString duration;
    double currentScalar;
};

class ScalarCalc : public QObject
{
    Q_OBJECT
    Q_PROPERTY (QVariantList dataCollection READ getDataCollection NOTIFY dataCollectionChanged)
    Q_PROPERTY (QString clockTime READ getClockTime NOTIFY clockTimeChanged)
    Q_PROPERTY (QString scalarStr READ getScalarStr NOTIFY scalarStrChanged)
    Q_PROPERTY(int runCnt READ getRunCnt NOTIFY runCntChanged)
public:
    explicit ScalarCalc(QObject *parent  = nullptr);
    ~ScalarCalc();
    Q_INVOKABLE void onFlowToScalarClicked(const QString &name, bool s);
    Q_INVOKABLE void saveResults(const QUrl &fileName);
    void startCalc(const QString &name);
    void stopCalc();
    void calcScalar(double y0, double y1, qreal x0, qreal x1);
    double getCurrentScalar() const;
    QString getClockTime() const;
    QString getScalarStr() const;
    int getRunCnt() const;
    QVariantList getDataCollection() const;
    bool isExposure;
signals:
    void dataCollectionChanged();
    void clockTimeChanged();
    void scalarStrChanged();
    void runCntChanged();
private:
    QList<DataStore> dataCollection;
    QString clockTime;
    QString m_currentCollection;
    QTime m_timeStart;
    //int m_duration;
    double m_currentScalar;
    int runCnt;
    QString scalarStr;
};