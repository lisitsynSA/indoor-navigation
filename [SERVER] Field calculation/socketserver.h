#ifndef SOCKETSERVER_H
#define SOCKETSERVER_H

#include <QObject>
#include <QWebSocket>
#include <QWebSocketServer>
#include "message.h"

class SocketServer : public QObject
{
    Q_OBJECT
public:
    explicit SocketServer(int init_port, QObject *parent = nullptr);
    ~SocketServer();
    void disconnect();
    void print(QString msg);

signals:
    void closed();
    void browserPrint(QString msg);
    void sendMessage(Message msg);

public slots:
    void onNewConnection();
    void processTextMessage(QString msg);
    void processBinaryMessage(QByteArray msg);
    void socketDisconnected();

private:
    int port;
    QWebSocketServer *socket;
    QList<QWebSocket *> clients;
    QString messageHistory;
};

#endif // SOCKETSERVER_H
