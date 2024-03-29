#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>

#include <QElapsedTimer>

#include "controller.h"
#include "writelogfile.h"


class MainWindow : public QApplication
{
    Q_OBJECT
    Q_PROPERTY (QString logText READ getLogText NOTIFY logChanged)
public:
    explicit MainWindow(int &argc, char **argv);
    ~MainWindow();

    void initController();
    QString getLogText() const;

    Q_INVOKABLE void onConnectButtonClicked();
    Q_INVOKABLE void onReadButtonClicked(bool s);
signals:
    void logChanged(QString);
    
private slots:
    void setLogText(const QString &text);
    void processEvents();
private:
    QQmlApplicationEngine m_engine;

    SettingsDialog *m_settings;
    Controller *m_flow;
    
    QString logText;

    QTimer* m_logTimer;
    QElapsedTimer m_programTime;
    WriteLogFile m_writeLog;
};

