#pragma once

#include <QObject>
#include <QDateTime>

class Logger: public QObject {
    Q_OBJECT

public:
    enum Channel {
        kFirstChannel,
        kSecondChannel,
        kCommentChannel,

        kChannelsNum
    };
    Q_ENUM(Channel)

    struct LogItem
    {
        QDateTime  timestamp;
        Channel   channel;
        QByteArray message;
    };

public:
    Logger(QObject* parent = nullptr);

    void print(Channel channel, const QByteArray& msg);
    void comment(const char* str); // <- Перенести
    void clear();

    void setEnableChannel(Channel channel);
    void setDisableChannel(Channel channel);

    bool empty() const;
    int count() const;
    const LogItem& back() const;
    const LogItem& at(int i) const;

signals:
    void appended();
    void cleared();

private:
    QList<LogItem> m_log;
};
