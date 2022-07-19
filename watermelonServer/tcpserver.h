#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>

class TcpServer : public QThread
{
    Q_OBJECT
public:
    explicit TcpServer(QTcpSocket* ts, QObject *parent = nullptr);
    int receiveFile(int i);
    void sendInformation(QByteArray data);
    void sendFile(QString file);
    void disct();

protected:
    void run() override;

private:
    QTcpSocket *socket;
signals:
    void over(QString path);
    void readInformation(QString data);
    void textProgressValue(int i);
};

#endif // TCPSERVER_H
