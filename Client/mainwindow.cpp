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
    if (_client) return;

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

    qDebug()<<data;
    const QStringList messages = QString::fromUtf8(data).trimmed().split("\n");
    // const QString msg = QString::fromUtf8(data).trimmed();

    for (const QString& message : messages){
    if (message.startsWith("ID:"))
        {
            bool ok = false;
            int id = message.mid(3).toInt(&ok);
            if (ok)
            {
                this->setWindowTitle(QString("Client (%1)").arg(id));
                continue;
            }
        }

        else if(message.startsWith("FROM:"))
        {
            QStringList splitMsg = message.mid(5).split(":");


            if (!m_chats.contains(splitMsg[0])){

                ClientChatWidget* chatWidget = new ClientChatWidget(QString(splitMsg[0]));
                ui->tbChats->addTab(chatWidget, splitMsg[0]);
                m_chats.insert(splitMsg[0], chatWidget);

                connect(chatWidget, &ClientChatWidget::messageRequested, this, [this](QString target, QString message){
                    _client->sendMessage(target + ":" + message);
                });


            }
            m_chats[splitMsg[0]]->appendMessage(splitMsg[1], false);




        }

        else if(message.startsWith("CLIENTS:"))
        {

            const QStringList splitClients = message.mid(8).split("|");
            ui->lstClients->clear();

            for (const QString& clientReceived : splitClients){
                ui->lstClients->addItem(clientReceived);

            }

        }
    }
}


void MainWindow::on_actionDisconnect_triggered()
{
    if (!_client)
        return;
    _client->disconnectFromServer();

    _client->deleteLater();
    _client = nullptr;

    ui->centralwidget->setEnabled(false);
}

void MainWindow::on_actionExit_triggered()
{
    if (_client){
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

