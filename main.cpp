#include <QCoreApplication>
#include "processmonitor.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ProcessMonitor pm;

    return a.exec();
}
