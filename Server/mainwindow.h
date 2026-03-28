#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QTcpSocket>
#include <QInputDialog>
#include "ServerManager.h"
#include "qlistwidget.h"

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
    void newClientConnected(QTcpSocket *client);
    void clientDisconnected(QTcpSocket *client);
    void on_btnDisconnectAll_clicked();


    void on_btnBroadcast_clicked();

    void on_lstClients_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::MainWindow *ui;
    ServerManager *_Server;

    QMap<QString,QTcpSocket*> m_clients;

private: //methods
    void setupServer();
};
#endif // MAINWINDOW_H
