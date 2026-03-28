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
    m_clients.insert(QString::number(id), client);

    connect(client, &QTcpSocket::disconnected, this, [this, client, id](){
        m_clients.remove(QString::number(id));
        client->deleteLater();
    });


    ui->lstClients->addItem(QString("New Client added: %1").arg(id));
    auto chatWidget = new ClientChatWidget(client);
    ui->tbChats->addTab(chatWidget, QString("client(%1)").arg(id));

    connect(chatWidget, &ClientChatWidget::messageReceived, this, &MainWindow::routeMessage);



    QList<QTcpSocket*> clientList = m_clients.values();

    QList clientIds = m_clients.keys();

    QString clientListToBroadcast = "CLIENTS:"+clientIds.join("|");


    client->write(QString("ID:%1").arg(id).toUtf8()+"\n");


    for (QTcpSocket* client : clientList){
        client->write(clientListToBroadcast.toUtf8()+"\n");
    }
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

void MainWindow::on_btnBroadcast_clicked()
{
    bool ok; // create a boolean to see if the ok button was clicked
    QString message = QInputDialog::getText(this,"Broadcast","Enter message to send to ALL clients", QLineEdit::Normal, "", &ok); // open the dialog containing the text box and ok button

    if(ok == true && !message.isEmpty()){           // if the user clicks on broadcast while having something in the text box

        QList<QTcpSocket*> clientList = m_clients.values();

        for (QTcpSocket* client: clientList){        // loop through the clients
            client->write("FROM:0:"+message.toUtf8()+"\n");        // write in the socket of each client
        }
    }


}



void MainWindow::routeMessage(QString senderId, QString message)
{
    QList splitMsg = message.split(":");

    m_clients[splitMsg[0]]->write(("FROM:"+senderId+":"+splitMsg[1]).toUtf8()+"\n");
}

