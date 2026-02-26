#include "mainwindow.h"
#include "ChatItemWidget.h"
#include "ui_mainwindow.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);

    ui->lstMessages->setSpacing(2);
    ui->lstMessages->setUniformItemSizes(false);
    ui->lstMessages->setStyleSheet("QListWidget{padding:0px; margin:0px;}");

    ui->lnMessage->setFocus();

    connect(ui->lnMessage, &QLineEdit::returnPressed,
            this, &MainWindow::on_btnSend_clicked);
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

    addChatBubble(msg, false);
}

void MainWindow::on_btnSend_clicked()
{
    const QString message = ui->lnMessage->text().trimmed();
    if (message.isEmpty()) return;

    _client->sendMessage(message);
    ui->lnMessage->clear();

    addChatBubble(message, true);
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

void MainWindow::addChatBubble(const QString& msg, bool isMyMessage)
{
    auto *bar = ui->lstMessages->verticalScrollBar();
    const bool wasAtBottom = (bar->value() >= bar->maximum() - 5);

    auto *item = new QListWidgetItem(ui->lstMessages);

    auto *w = new ChatItemWidget(ui->lstMessages);
    w->setMessage(msg, isMyMessage);

    w->layout()->activate();
    item->setSizeHint(w->sizeHint());

    ui->lstMessages->addItem(item);
    ui->lstMessages->setItemWidget(item, w);

    if (wasAtBottom)
        ui->lstMessages->scrollToBottom();
}
