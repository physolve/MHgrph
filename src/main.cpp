#include <iostream>
#include "mainwindow.h"
int main(int argc, char *argv[]) {
    QCoreApplication::setApplicationName("MHgrph");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCoreApplication::setOrganizationName(QStringLiteral("Tomsk Polytechnic University"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("tpu.ru"));
    
    MainWindow app(argc, argv);
    return app.exec();
}