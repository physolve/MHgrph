
#include "mainwindow.h"
#include "CustomPlotItem.h"
//#include "writeregistermodel.h"
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>


MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), m_settings(new SettingsDialog), m_logTimer(new QTimer)
{
    QQuickStyle::setStyle("Material");
    QString applicationName = "MHgrph";
    
    m_engine.addImportPath(":/");

    const QUrl url(QString("qrc:/%1/qml/main.qml").arg(applicationName));
    QObject::connect(
                &m_engine, &QQmlApplicationEngine::objectCreated, this,
                [url](QObject *obj, const QUrl &objUrl) {
                    if(!obj && url == objUrl) QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);
    qmlRegisterType<CustomPlotItem>("CustomPlot", 1, 0, "CustomPlotItem");

    m_engine.rootContext()->setContextProperty("backend", this);
    m_engine.rootContext()->setContextProperty("settingsDialog", m_settings);
    setLogText("Click connect");
    
    initController();
    
    m_engine.load(url);

    connect(m_logTimer, &QTimer::timeout, this, &MainWindow::processEvents);

    m_programTime.start();
}

MainWindow::~MainWindow()
{
    m_logTimer->stop();
    delete m_logTimer;
    delete m_settings;
}

void MainWindow::initController(){
    if(m_settings->isFlowMeterConnected()){
        m_flow = new Controller(m_settings->settings("flow"));
        connect(m_flow, &Controller::logChanged, this, &MainWindow::setLogText); //link to qml notify
        m_engine.rootContext()->setContextProperty("flowBack", m_flow);
    }
    else{
        m_engine.rootContext()->setContextProperty("flowBack", "undefined");
    }
}

void MainWindow::onConnectButtonClicked()
{
    if(!m_settings->isFlowMeterConnected())
        return;
    m_flow->setSettings(m_settings->settings("flow"));
    m_flow->connectFlow(); // test it!
}


void MainWindow::onReadButtonClicked(bool s)
{
    if(s){
        m_logTimer->start(1000);
        m_flow->startReading();
    }
    else{
        m_logTimer->stop();
        m_flow->stopReading();
    }
}

void MainWindow::processEvents(){
    quint64 c_time = m_programTime.elapsed()/1000;
    double c_flow = 0;
    if(m_settings->isFlowMeterConnected()){
        c_flow = m_flow->getLastChanged();
    }
    QString line = QString("%1\t%2").arg(c_time).arg(c_flow);
    m_writeLog.writeLine(line);
}

void MainWindow::setLogText(const QString &text){
    if (text != logText)
    {
        logText = text;
        emit logChanged(text);
    }
}

QString MainWindow::getLogText() const
{
    return logText;
}

