#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionConnect_triggered()
{
    _client = new ClientManager();

    connect(_client, &ClientManager::connected, [this](){
        ui->centralwidget->setEnabled(true);
    });

    connect(_client, &ClientManager::disconnected, [this](){
        ui->centralwidget->setEnabled(false);
    });

    connect(_client, &ClientManager::dataReceived, this, &MainWindow::dataReceived);

    _client->connectToServer();
}

void MainWindow::dataReceived(QByteArray data)
{
    const QString msg = QString::fromUtf8(data).trimmed();

    if (msg.startsWith("ID:"))
    {
        bool ok = false;
        int id = msg.mid(3).toInt(&ok);
        if (ok)
        {
            this->setWindowTitle(QString("Client (%1)").arg(id));
            return;
        }
    }

    else if(msg.startsWith("FROM:"))
    {
        QStringList splitMsg = msg.mid(5).split(":");


        if (m_chats.contains(splitMsg[0])){
            m_chats[splitMsg[0]]->appendMessage(splitMsg[1]);
        }

    }
}


void MainWindow::on_actionDisconnect_triggered()
{
    if (!_client)
        return;
    _client->disconnectFromServer();

    ui->centralwidget->setEnabled(false);
}

void MainWindow::on_actionExit_triggered()
{
    if (!_client){
    _client->disconnectFromServer();
    _client->deleteLater();
    _client = nullptr;
    }

    close();
}


void MainWindow::on_btnStartChat_clicked()
{
    QListWidgetItem* client = ui->lstClients->currentItem();
    if(client !=nullptr){

        ClientChatWidget* chatWidget = new ClientChatWidget(QString(client->text()));

        ui->tbChats->addTab(chatWidget, client->text());

        m_chats.insert(client->text(), chatWidget);



        connect(chatWidget, &ClientChatWidget::messageRequested, this, [this](QString target, QString message){
            _client->sendMessage(target + ":" + message);
        });
    }
}


void MainWindow::on_btnEndChat_clicked()
{
    int tbIndex = ui->tbChats->currentIndex();

    if (tbIndex == -1) return;

    QString tbText = ui->tbChats->tabText(tbIndex);
    QWidget* currentTab = ui->tbChats->currentWidget();

    m_chats.remove(tbText);

    ui->tbChats->removeTab(tbIndex);

    delete currentTab;


}

