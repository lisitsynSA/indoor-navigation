#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "socketclient.h"
#include "calc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void startClient();
    void restartSession();
    void sendMessage();

private:
    Ui::MainWindow *ui;
    SocketClient *client;
    Calc calc;

    void startInterfaceEnabled(bool choice);
};

#endif // MAINWINDOW_H
