#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDateTime>
#include <QFileDialog>
#include "tcpserver.h"

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
    QTcpServer *qTcpServer;

private slots:
    void on_connectButton_clicked();
    void readInformation(QString data);
    void on_sendButton_clicked();

    void on_selectButton_clicked();

    void on_sendFIleButton_clicked();

    void on_clearSendButton_clicked();

    void on_saveDialogBoxButton_clicked();

    void on_clearDialogBoxButton_clicked();

    void on_disconnectButton_clicked();

signals:
    void sendInformation(QByteArray data);
    void sendFileData(QString file);
    void disconnted();

};
#endif // MAINWINDOW_H
