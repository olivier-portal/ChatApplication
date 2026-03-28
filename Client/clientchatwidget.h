#ifndef CLIENTCHATWIDGET_H
#define CLIENTCHATWIDGET_H

#include <QWidget>
#include "ChatItemWidget.h"


namespace Ui {
class ClientChatWidget;
}

class ClientChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientChatWidget(QString target, QWidget *parent = nullptr);
    ~ClientChatWidget();

    /**
     * @brief append messages to the widget's message list
     * @param message message to be appended
     * @param isMyMsg if the message is sent by the client = true if the message is received = false
     */
    void appendMessage(QString message, bool isMyMsg);

private slots:
    void on_btnSend_clicked();


signals:
        void messageRequested(QString target,QString message);

private:
    Ui::ClientChatWidget *ui;
    QString msgTarget; // target to send the messages to
};

#endif // CLIENTCHATWIDGET_H
