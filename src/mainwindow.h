#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>

#include <QElapsedTimer>

#include "Initialize.h"
#include "DataAcquisition.h"
#include "writelogfile.h"


class MainWindow : public QApplication
{
    Q_OBJECT
    Q_PROPERTY (QString logText READ getLogText NOTIFY logChanged)
public:
    explicit MainWindow(int &argc, char **argv);
    ~MainWindow();

    QString getLogText() const;
    //void initController();
    //Q_INVOKABLE void onConnectButtonClicked();
    Q_INVOKABLE bool onReadButtonClicked(bool s);
    Q_INVOKABLE bool onValveButtonClicked(bool s);
signals:
    void logChanged(QString);
    
private slots:
    void setLogText(const QString &text);
    void processEvents();
private:
    QQmlApplicationEngine m_engine;

    Initialize initSource;
    DataAcquisition dataSource;
    QString logText;

    QTimer* m_logTimer;
    QElapsedTimer m_programTime;
    WriteLogFile m_writeLog;
};

