#include "ChatItemWidget.h"
#include "ui_ChatItemWidget.h"
#include <qdatetime.h>

ChatItemWidget::ChatItemWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatItemWidget)
{
    ui->setupUi(this);

    // Transparent widget
    setAttribute(Qt::WA_StyledBackground, true);
    setStyleSheet("background: transparent;");

    //Message wrap
    ui->lblMessage->setWordWrap(true);

    //Time without bubble
    ui->lblTime->setStyleSheet("color: white; background: transparent; font-size: 10px; margin-top: 2px");
}


ChatItemWidget::~ChatItemWidget()
{
    delete ui;
}

void ChatItemWidget::setMessage(const QString &message, bool isMyMessage)
{
    ui->lblMessage->setText(message);
    ui->lblTime->setText(QDateTime::currentDateTime().toString("HH:mm"));

    //Bubble size
    ui->bubbleFrame->setMaximumWidth(380);

    //Bubble color
    const QString bubbleColor = isMyMessage ? "#4a90e2" : "#a7ffed";
    const QString textColor = isMyMessage ? "white" : "black";

    ui->lblMessage->setStyleSheet(QString("color: %1;").arg(textColor));

    ui->bubbleFrame->setStyleSheet(QString(
        "QFrame#bubbleFrame {"
        " background-color: %1;"
        " border-radius: 10px;"
        " padding: 4px 8px;"
        "}"
        ).arg(bubbleColor));

    //Align text and time
    ui->rootLayout->setAlignment(ui->container, isMyMessage ? Qt::AlignRight : Qt::AlignLeft);
    ui->lblTime->setAlignment(isMyMessage ? Qt::AlignRight : Qt::AlignLeft);

    //fix bubble size and gap
    ui->container->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    ui->bubbleFrame->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
    ui->lblMessage->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

    ui->bubbleFrame->setMaximumWidth(380);
    ui->lblMessage->setMaximumWidth(360);

    ui->rootLayout->setContentsMargins(0, 0, 0, 0);
    ui->rootLayout->setSpacing(0);
}
