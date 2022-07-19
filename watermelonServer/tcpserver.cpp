#include "tcpserver.h"

TcpServer::TcpServer(QTcpSocket* ts, QObject *parent)
    : QThread{parent}
{
    socket = ts;
}

void TcpServer::run()
{
    connect(socket, &QTcpSocket::readyRead, this, [=](){
        static int i = 0;
        if(i == 0)
        {
            socket->read((char*)&i, 4);

        }
        if(i == 1)
        {
            i = receiveFile(i);
        }else if(i == 2)
        {
            emit readInformation(QString(socket->readAll()));
        }
    });
    exec();
}


int TcpServer::receiveFile(int i)
{
    static int count = 0;
    static int fileSzie = 0;
    char* fileName;

    if(count == 0)
    {
        socket->read((char*)&fileSzie, 4);
        socket->read(fileName,);
    }
    QString fn = QString(fileName);
    qDebug()<<fileName;
    QFile file("./"+fn);
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

void TcpServer::sendInformation(QByteArray data)
{
    int i = 2;
    socket->write((char*)&i, 4);
    socket->write(data);
}

void TcpServer::sendFile(QString path)
{
    QFile file(path);
    QFileInfo fileInfo(path);
    file.open(QFile::ReadOnly);
    int fileSize = fileInfo.size();
    QByteArray fn = fileInfo.fileName().toUtf8();
    char* fileName = fn.data();
    int i = 1;
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

void TcpServer::disct()
{
    socket->close();
    socket->deleteLater();
}
