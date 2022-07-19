#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QMessageBox>
#include <QDateTime>
#include <QString>
#include <QFileDialog>
#include "tcpclient.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TcpClient* tcpClient;
    QThread* thread;

private slots:
    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void on_sendButton_clicked();

    void on_clearSendButton_clicked();

    void on_sendFIleButton_clicked();

    void on_selectButton_clicked();

    void on_saveDialogBoxButton_clicked();

    void on_clearDialogBoxButton_clicked();

    void readInformation(QByteArray data);


signals:
    void startConnectServer(QString ip, unsigned short port);
    void sendData(QString data);
    void sendFile(QString path);

};
#endif // MAINWINDOW_H
