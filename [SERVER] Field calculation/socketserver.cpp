#include "socketserver.h"

SocketServer::SocketServer(int init_port, QObject *parent) :
    QObject(parent),
    port(init_port)
{
    socket = new QWebSocketServer(QStringLiteral("Indoor Navigation Server"),\
                                  QWebSocketServer::NonSecureMode, this);

    if (socket->listen(QHostAddress::Any, port))
    qDebug() << "Socket Server listening on port " << port;
    connect(socket, &QWebSocketServer::newConnection, this, &SocketServer::onNewConnection);
    connect(socket, &QWebSocketServer::closed, this, &SocketServer::closed);
}

SocketServer::~SocketServer()
{
    disconnect();
    qDeleteAll(clients.begin(), clients.end());
}

void SocketServer::disconnect()
{
    socket->close();
    print("[DBG] Socket Server shut down");
}

void SocketServer::print(QString msg)
{
    messageHistory = msg + "\n" + messageHistory;
    emit browserPrint(messageHistory);
}

void SocketServer::onNewConnection()
{
    QWebSocket *pSocket = socket->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &SocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SocketServer::socketDisconnected);

    clients << pSocket;
    print("[DBG] New connection " + QString::number(clients.size()));
}

void SocketServer::processTextMessage(QString msg)
{
    qDebug() << "Message received: " << msg;
    print("[CLIENT] " + msg);
}

void SocketServer::processBinaryMessage(QByteArray msg)
{
    Message translation(msg);
    print("[CLIENT] " + translation.getInfo());
    emit sendMessage(translation);
}

void SocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    qDebug() << "socketDisconnected: " << pClient;
    print("[DBG] Disconnected");
    if (pClient) {
        clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

