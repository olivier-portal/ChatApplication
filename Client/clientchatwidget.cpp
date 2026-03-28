#include "clientchatwidget.h"
#include "ui_clientchatwidget.h"

ClientChatWidget::ClientChatWidget(QString target,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientChatWidget)

{
    ui->setupUi(this);

    ui->lnMessage->setFocus();

    connect(ui->lnMessage, &QLineEdit::returnPressed,
            this, &ClientChatWidget::on_btnSend_clicked);

    msgTarget = target;
}

ClientChatWidget::~ClientChatWidget()
{
    delete ui;
}


void ClientChatWidget::on_btnSend_clicked()
{
    //_client->write(ui->lnMessage->text().trimmed().toUtf8());
    emit messageRequested(msgTarget,ui->lnMessage->text().trimmed());
    ui->lnMessage->setText("");
}

void ClientChatWidget::appendMessage(QString message){
    ui->lstMessages->addItem(message);
}

