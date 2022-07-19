#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("watermelonServer");

    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, 100);
    ui->disconnectButton->setDisabled(true);
    ui->sendButton->setDisabled(true);
    ui->sendFIleButton->setDisabled(true);

    qTcpServer = new QTcpServer(this);

    connect(qTcpServer, &QTcpServer::newConnection, this, [=](){
        QTcpSocket *socket = qTcpServer->nextPendingConnection();

        TcpServer *server = new TcpServer(socket);
        server->start();

        connect(server, &TcpServer::readInformation, this, &MainWindow::readInformation);

        connect(this, &MainWindow::sendInformation, server, &TcpServer::sendInformation);

        connect(this, &MainWindow::sendFileData, server, &TcpServer::sendFile);

        connect(server, &TcpServer::over, this, [=](QString path){
            QDateTime time = QDateTime::currentDateTime();
            ui->dialogBoxTextEdit->append(path+"文件接受完成>>"+time.toString("yyyy-MM-dd hh:mm:ss"));
        });

        connect(server, &TcpServer::textProgressValue, ui->progressBar, &QProgressBar::setValue);

        connect(this, &MainWindow::disconnted, server, &TcpServer::disct);
        connect(socket, &QTcpSocket::disconnected, this, [=](){
            socket->close();
            server->exit();
            socket->deleteLater();
            server->deleteLater();
            ui->connectButton->setDisabled(false);
            ui->portLineEdit->setDisabled(false);
            ui->disconnectButton->setDisabled(true);
            ui->sendButton->setDisabled(true);
            ui->sendFIleButton->setDisabled(true);
        });
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    qTcpServer->listen(QHostAddress::Any, ui->portLineEdit->text().toUShort());
    QDateTime time = QDateTime::currentDateTime();
    ui->connectButton->setDisabled(true);
    ui->portLineEdit->setDisabled(true);
    ui->disconnectButton->setDisabled(false);
    ui->sendButton->setDisabled(false);
    ui->sendFIleButton->setDisabled(false);
    ui->dialogBoxTextEdit->append("服务启动成功！>>"+time.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::on_disconnectButton_clicked()
{
    emit disconnted();
    qTcpServer->close();
    qTcpServer->deleteLater();
    ui->connectButton->setDisabled(false);
    ui->portLineEdit->setDisabled(false);
    ui->disconnectButton->setDisabled(true);
    ui->sendButton->setDisabled(true);
    ui->sendFIleButton->setDisabled(true);
}


void MainWindow::readInformation(QString data)
{
    QDateTime time = QDateTime::currentDateTime();
    ui->dialogBoxTextEdit->append("客户端>>"+time.toString("yyyy-MM-dd hh:mm:ss")+">>>"+data);
}

void MainWindow::on_sendButton_clicked()
{
    emit sendInformation(ui->sendTextEdit->toPlainText().toUtf8());
}


void MainWindow::on_selectButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this, tr("选择发送文件"),
                                                    "./",
                                                    "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)");
    ui->pathLineEdit->setText(path);
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, 100);
}


void MainWindow::on_sendFIleButton_clicked()
{
    QString path = ui->pathLineEdit->text();
    if(path.isEmpty())
    {
        QMessageBox::warning(this, "警告", "未选择文件");
    }else
    {
        emit sendFileData(path);
    }

}


void MainWindow::on_clearSendButton_clicked()
{
    ui->sendTextEdit->clear();
}


void MainWindow::on_saveDialogBoxButton_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QFileDialog::saveFileContent(ui->dialogBoxTextEdit->toPlainText().toUtf8(), "watermelonServer"+time.toString("yyyy-MM-dd hh:mm:ss")+".txt");
}


void MainWindow::on_clearDialogBoxButton_clicked()
{
    ui->dialogBoxTextEdit->clear();
}



