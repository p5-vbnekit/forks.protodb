#pragma once

#include "Logger.h"

#include <QObject>

class LogFormatter;

class LogPrinter: public QObject
{
    Q_OBJECT

public:
    LogPrinter(QObject* parent = nullptr);
   ~LogPrinter();

    void setLogFile(const QString& path);
    QString logFile() const;

    void setAppendFile(bool append);
    bool appendFile() const;

    bool setEnabled(bool enabled = true);
    bool enabled() const;

    bool setDisabled(bool disabled = true);
    bool disabled();

public slots:
    void print(const Logger::Event& event);

private:
    bool m_append_flag;
    QFile m_log_file;
    LogFormatter* m_fmt;
};