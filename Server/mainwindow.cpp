#include "clientchatwidget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newClientConnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->lstClients->addItem(QString("New Client added: %1").arg(id));
    auto chatWidget = new ClientChatWidget(client);
    ui->tbChats->addTab(chatWidget, QString("client(%1)").arg(id));

    //send Id to client
    client->write(QString("ID:%1").arg(id).toUtf8());
}

void MainWindow::clientDisconnected(QTcpSocket *client)
{
    auto id = client->property("id").toInt();
    ui->lstClients->addItem(QString("Client disconnected: %1").arg(id));
}

void MainWindow::setupServer()
{
    _Server = new ServerManager();
    connect(_Server, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);
    connect(_Server, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);
}
