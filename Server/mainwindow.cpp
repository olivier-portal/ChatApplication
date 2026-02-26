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
    m_clients.insert(client);

    connect(client, &QTcpSocket::disconnected, this, [this, client](){
        m_clients.remove(client);
        client->deleteLater();
    });

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

void MainWindow::on_btnDisconnectAll_clicked()
{
    const auto clients = m_clients.values();

    for (QTcpSocket *client : clients)
    {
        if (!client) continue;

        //Disconnect
        client->disconnectFromHost();

        //In case it doesn't disconnect rapidly
        if (client->state() != QAbstractSocket::UnconnectedState)
            client->abort();
    }
    ui->lstClients->addItem("Disconnected ALL clients");
    ui->tbChats->clear();
}

void MainWindow::setupServer()
{
    _Server = new ServerManager();
    connect(_Server, &ServerManager::newClientConnected, this, &MainWindow::newClientConnected);
    connect(_Server, &ServerManager::clientDisconnected, this, &MainWindow::clientDisconnected);
}
