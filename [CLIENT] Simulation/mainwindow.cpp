#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setLayout(ui->gridLayout);

    client = NULL;
    startInterfaceEnabled(true);

    connect(ui->clientButton, SIGNAL(pressed()), this, SLOT(startClient()));
    connect(ui->disconnectButton, SIGNAL(pressed()), this, SLOT(restartSession()));
    connect(ui->sendButton, SIGNAL(pressed()), this, SLOT(sendMessage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startClient()
{
    startInterfaceEnabled(false);

    qDebug() << "Start Socket Client";
    if (client)
        delete client;
    client = new SocketClient("ws://" + ui->urlEdit->text(), this);
    connect(client, SIGNAL(closed()), this, SLOT(restartSession()));
    connect(client, SIGNAL(browserPrint(QString)), ui->msgBrowser, SLOT(setPlainText(QString)));
    connect(&calc, SIGNAL(sendMessage(Message)), client, SLOT(sendMessage(Message)));
    connect(client, SIGNAL(connected()), &calc, SLOT(start()));
    connect(client, SIGNAL(closed()), &calc, SLOT(stop()));
}

void MainWindow::restartSession()
{
    if (client)
        client->disconnect();
    startInterfaceEnabled(true);
}

void MainWindow::sendMessage()
{
    if (client)
        client->sendMessage(ui->msgEdit->text());
    ui->msgEdit->clear();
}

void MainWindow::startInterfaceEnabled(bool choice)
{
    ui->clientButton->setEnabled(choice);
    ui->urlEdit->setEnabled(choice);
    ui->disconnectButton->setEnabled(!choice);
    ui->sendButton->setEnabled(!choice);
}
