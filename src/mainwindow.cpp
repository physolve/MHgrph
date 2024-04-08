
#include "mainwindow.h"
#include "CustomPlotItem.h"
//#include "writeregistermodel.h"
#include <QQmlContext>
#include <QtQuickControls2/QQuickStyle>
#include <QStandardItemModel>
#include <QStatusBar>
#include <QUrl>


MainWindow::MainWindow(int &argc, char **argv)
    : QApplication(argc, argv), initSource(), dataSource(), m_logTimer(new QTimer)
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
    m_engine.rootContext()->setContextProperty("initSource", &initSource);
    m_engine.rootContext()->setContextProperty("dataSource", &dataSource);

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
    //delete m_settings;
}

void MainWindow::initController(){
    
    //start connection to Advantech here
}

void MainWindow::onConnectButtonClicked()
{
    // initialize Advantech using known parameters
}


void MainWindow::onReadButtonClicked(bool s)
{
    // delete
}

void MainWindow::processEvents(){
    quint64 c_time = m_programTime.elapsed()/1000;
    double c_flow = 0;

    dataSource.processEvents();

    auto values = dataSource.getMeasures();

    c_flow = values[0];
    
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

