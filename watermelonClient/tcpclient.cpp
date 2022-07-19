#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent)
    : QObject{parent}
{

}

void TcpClient::connectServer(QString ip, unsigned short port)
{
    socket = new QTcpSocket(this);
    socket->connectToHost(QHostAddress(ip), port);

    connect(socket, &QTcpSocket::connected, this, &TcpClient::connectOK);

    connect(socket, &QTcpSocket::disconnected, this, [=](){
        socket->close();
        socket->deleteLater();
    });

    connect(socket, &QTcpSocket::readyRead, this, [=](){
        static int i = 0;
        if(i == 0)
        {
            socket->read((char*)&i, 4);
        }
        if(i == 1)
        {
            i = receiveFile(i);
        }else
        {
            i = 0;
            emit readInformation(socket->readAll());
        }

    });
}

void TcpClient::sendDate(QString data)
{
    int i = 2;
    socket->write((char*)&i, 4);
    socket->write(data.toUtf8());
}

void TcpClient::sendFIle(QString path)
{
    QFile file(path);
    QFileInfo fileInfo(path);
    file.open(QFile::ReadOnly);
    int fileSize = fileInfo.size();
    int i = 1;
    QByteArray fn = fileInfo.fileName().toUtf8();
    char* fileName = fn.data();
    while(!file.atEnd())
    {
        static int num = 0;
        if(num == 0)
        {
            socket->write((char*)&i, 4);
            socket->write((char*)&fileSize, 4);
            socket->write(fileName);
        }
        QByteArray line = file.readLine();
        socket->write(line);
        num += line.size();
        emit textProgressValue((num * 100)/fileSize);
        if(num == fileSize)
        {
            num = 0;
        }
    }
}

int TcpClient::receiveFile(int i)
{
    static int count = 0;
    static int fileSzie = 0;
    char* fileName;


    if(count == 0)
    {
        socket->read((char*)&fileSzie, 4);
        socket->read(fileName);
    }

    QFile file("./"+QString(fileName));
    file.open(QFile::WriteOnly);

    //读取数据
    QByteArray data = socket->readAll();
    count += data.size();
    file.write(data);
    if(count == fileSzie)
    {
        file.close();
        file.deleteLater();
        count = 0;
        fileSzie = 0;
        i = 0;
        emit over(QString(fileName));
    }
    return i;
}
