#include "socketclient.h"

SocketClient::SocketClient(QUrl init_url, QObject *parent) :
    QObject(parent),
    url(init_url)
{
    qDebug() << "Socket Server: " << url;
    connect(&socket, &QWebSocket::connected, this, &SocketClient::onConnected);
    connect(&socket, &QWebSocket::disconnected, this, &SocketClient::closed);
    socket.open(QUrl(url));
}

SocketClient::~SocketClient()
{
    disconnect();
}

void SocketClient::disconnect()
{
    socket.close();
    print("[DBG] Socket Client shut down");
}

void SocketClient::print(QString msg)
{
    messageHistory = msg + "\n" + messageHistory;
    emit browserPrint(messageHistory);
}

void SocketClient::sendMessage(QString msg)
{
    socket.sendTextMessage(msg);
    print("[CLIENT] " + msg);
}

void SocketClient::sendBinaryMessage(QByteArray msg)
{
    socket.sendBinaryMessage(msg);
    print("[CLIENT] Binary Message");
}

void SocketClient::onConnected()
{
    qDebug() << "WebSocket connected";
    print("[DBG] Socket connected");
    connect(&socket, &QWebSocket::textMessageReceived,
            this, &SocketClient::onTextMessageReceived);
    socket.sendTextMessage(QStringLiteral("Connected"));
    print("[CLIENT] Connected");
    emit connected();
}

void SocketClient::onTextMessageReceived(QString msg)
{
    qDebug() << "Message received: " << msg;
    print("[SERVER] " + msg);
}

void SocketClient::sendMessage(Message msg)
{
    socket.sendBinaryMessage(msg.getByteArray());
    print("[CLIENT] " + msg.getInfo());
}
