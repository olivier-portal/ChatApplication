#include "ChatItemWidget.h"
#include "ui_ChatItemWidget.h"

ChatItemWidget::ChatItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatItemWidget)
{
    ui->setupUi(this);
}


ChatItemWidget::~ChatItemWidget()
{
    delete ui;
}

void ChatItemWidget::setMessage(QString message, bool isMyMessage)
{
    if (isMyMessage) {
        ui->lblMessage->setAlignment(Qt::AlignRight);
    }
    ui->lblMessage->setText(message);
    ui->lblMessage->setStyleSheet("color: black;");

    ui->lblTime->setText(QDateTime::currentDateTime().toString("HH:mm"));
    ui->lblTime->setStyleSheet("color: black;");

    this->setStyleSheet("background-color: rgb(167,255,237); border-radius: 8px;");
}
