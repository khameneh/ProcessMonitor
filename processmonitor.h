#ifndef PROCESSMONITOR_H
#define PROCESSMONITOR_H

#include <QObject>
#include <QProcess>
#include <QSettings>
#include <QFile>
#include <QTimer>
#include <QDebug>
#include <QDateTime>

#define CONFIG_FILE "config"
#define WATCHDOG_TIMEOUT 1111

class ProcessMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ProcessMonitor(QObject *parent = nullptr);

    // loads the process configuration
    void loadConfig();

    // checks to see if a process currently exists in processes_
    bool processExists(QString cmd);

    // bring the foreground process window to the top (Only in Linux)
    void manageForegroundProcess();

public slots:
    // starts processes that not running while reading the config and managing the foreground process
    void watchDog();

private:
    // list of processes
    QList<QProcess*> processes_;

    // list of process commands
    QStringList process_commands_;

    // watchdog timer
    QTimer watchdog_timer_;

    // the foreground process name
    QString foreground_process_name_;

};

#endif // PROCESSMONITOR_H
