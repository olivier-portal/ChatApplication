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
    QString message = ui->lnMessage->text().trimmed();
    emit messageRequested(msgTarget,message);
    appendMessage(message, true);
    ui->lnMessage->setText("");
}

void ClientChatWidget::appendMessage(QString message, bool isMyMsg){

    auto item  = new QListWidgetItem(ui->lstMessages);

    ChatItemWidget* bubble = new ChatItemWidget(this);

    bubble->setMessage(message, isMyMsg);

    item->setSizeHint(bubble->sizeHint());

    ui->lstMessages->setItemWidget(item, bubble);

    // ui->lstMessages->addItem(message);
}

