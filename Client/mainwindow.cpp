#include "mainwindow.h"
#include "ChatItemWidget.h"
#include "ui_mainwindow.h"

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
    const QString msg = data.trimmed();

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

    auto chatWidget = new ChatItemWidget(this);
    chatWidget->setMessage(data);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0, 65));
    ui->lstMessages->addItem(listItemWidget);
    ui->lstMessages->setItemWidget(listItemWidget, chatWidget);
}


void MainWindow::on_btnSend_clicked()
{
    auto message = ui->lnMessage->text().trimmed();
    _client->sendMessage(message);
    // ui->lstMessages->addItem(message);
    ui->lnMessage->setText("");
    auto chatWidget = new ChatItemWidget(this);
    chatWidget->setMessage(message, true);
    auto listItemWidget = new QListWidgetItem();
    listItemWidget->setSizeHint(QSize(0, 65));
    ui->lstMessages->addItem(listItemWidget);
    ui->lstMessages->setItemWidget(listItemWidget, chatWidget);
}

