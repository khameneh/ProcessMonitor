#include "processmonitor.h"

ProcessMonitor::ProcessMonitor(QObject *parent)
    : QObject{parent}
{
    // load process configuration
    loadConfig();

    // connect the timeout signal to the watchdog function
    QObject::connect(&watchdog_timer_, SIGNAL(timeout()), this, SLOT(watchDog()) );

    // start the watchdog timer
    watchdog_timer_.start(WATCHDOG_TIMEOUT);
}

void ProcessMonitor::loadConfig()
{
    // open the configuration file
    QFile configFile(CONFIG_FILE);
    configFile.open(QFile::ReadOnly);

    // clear process commands
    process_commands_.clear();

    while (!configFile.atEnd())
    {
        // update process commands
        QString command = configFile.readLine();
        command.truncate(command.length()-1);

        // determine the foreground process
        if (command.endsWith("*"))
        {
            command.truncate(command.length()-1);
            foreground_process_name_ = command;
            qInfo() << QDateTime::currentDateTime().toString() << "foreground process: " << command;
        }

        process_commands_.append(command);

        // add new processes
        if(!processExists(command))
        {
            QProcess *process = new QProcess(this);
            process->setProgram(command);

            qInfo() << QDateTime::currentDateTime().toString() << "adding " << command;
            processes_.append(process);

            // kill if the process is running
            QProcess killprocess;
            killprocess.start("killall -9 " + command);
            killprocess.waitForFinished();
        }
    }

    // close the configuration file
    configFile.close();
}

bool ProcessMonitor::processExists(QString cmd)
{
    for ( int i = 0; i < processes_.count(); i++)
    {
        if (processes_[i]->program() == cmd)
            return true;
    }

    return false;
}

void ProcessMonitor::manageForegroundProcess()
{
    QProcess wmctrl(this);
    QProcess xprop(this);

    // determine window list
    wmctrl.start("wmctrl -l");
    wmctrl.waitForFinished();
    QByteArrayList windowList = wmctrl.readAllStandardOutput().split('\n');

    foreach( QByteArray window, windowList)
    {
        QByteArrayList windowProperties = window.split(' ');
        QString windowID(windowProperties[0]);
        QString processName;

        // determine the process name for the corresponding window
        xprop.start("xprop -id " + windowID);
        xprop.waitForFinished();
        QByteArrayList xpropList = xprop.readAllStandardOutput().split('\n');
        foreach(QByteArray line, xpropList)
        {
            if (line.contains("WM_CLASS"))
            {
                processName = line.split(' ')[3].toLower();
                processName = processName.mid(1, processName.length()-2);
            }
        }
        // bring the process to the foreground using the wmctrl command
        if (processName == foreground_process_name_)
        {
            windowProperties.removeFirst(); windowProperties.removeFirst(); windowProperties.removeFirst(); windowProperties.removeFirst();
            wmctrl.setProgram("wmctrl -a " + windowProperties.join(' '));
            wmctrl.start("wmctrl -a " + windowProperties.join(' '));
            wmctrl.waitForFinished();
        }
    }
}

void ProcessMonitor::watchDog()
{
    // load configuration
    loadConfig();

    // start the processes if needed
    for ( int i = 0; i < processes_.count(); i++)
    {
        // print out the state of each process
        qInfo() << QDateTime::currentDateTime().toString() << processes_[i]->program() << ": " << processes_[i]->state();

        // Only start the process if it is in the process_commands list, is not running or starting
        if (process_commands_.contains(processes_[i]->program()) && processes_[i]->state() != QProcess::Starting && processes_[i]->state() != QProcess::Running )
        {
            // redirect the standard output to null
            processes_[i]->setStandardOutputFile("/dev/null", QFile::WriteOnly);

            // start the process
            qInfo() << QDateTime::currentDateTime().toString() << "starting" << processes_[i]->program();
            processes_[i]->start();
            processes_[i]->waitForStarted();
        }
    }

    // check foreground process
    manageForegroundProcess();
}
