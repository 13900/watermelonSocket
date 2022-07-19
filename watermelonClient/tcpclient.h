#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QFile>
#include <QFileInfo>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = nullptr);
    void connectServer(QString ip, unsigned short port);
    void disconnectServer();
    void sendDate(QString data);
    void sendFIle(QString path);
    int receiveFile(int i);


private:
    QTcpSocket* socket;

signals:
    void connectOK();
    void disconnect();
    void textProgressValue(int num);
    void readInformation(QByteArray data);
    void over(QString path);
};

#endif // TCPCLIENT_H
