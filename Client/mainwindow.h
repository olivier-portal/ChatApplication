#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "clientmanager.h"
#include <clientchatwidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionConnect_triggered();
    void dataReceived(QByteArray data);

    void on_btnStartChat_clicked();

    void on_btnEndChat_clicked();

    void on_actionDisconnect_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    ClientManager *_client;

    QMap<QString, ClientChatWidget* > m_chats;
};
#endif // MAINWINDOW_H
