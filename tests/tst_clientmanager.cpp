#include <QtTest>
#include <QTcpServer>
#include <QTcpSocket>

#include "clientmanager.h"   // ton ClientManager du projet Client

class tst_clientmanager : public QObject
{
    Q_OBJECT
private slots:
    void smoke();
};

void tst_clientmanager::smoke()
{
    QVERIFY2(true, "QtTest is running");
}

QTEST_APPLESS_MAIN(tst_clientmanager)
//#include "tst_clientmanager.moc"
