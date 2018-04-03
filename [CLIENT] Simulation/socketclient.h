#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QWebSocket>
#include "message.h"

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QUrl init_url, QObject *parent = nullptr);
    ~SocketClient();
    void disconnect();
    void print(QString msg);
    void sendMessage(QString msg);
    void sendBinaryMessage(QByteArray msg);

signals:
    void closed();
    void browserPrint(QString msg);
    void connected();

public slots:
    void onConnected();
    void onTextMessageReceived(QString msg);
    void sendMessage(Message msg);

private:
    QWebSocket socket;
    QUrl url;
    QString messageHistory;
};

#endif // SOCKETCLIENT_H
