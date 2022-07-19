#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("watermelonClient");
    ui->disconnectButton->setDisabled(true);
    ui->dialogBoxTextEdit->setReadOnly(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0, 100);


}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{
    thread = new QThread(this);
    tcpClient = new TcpClient(this);
    tcpClient->moveToThread(thread);
    thread->start();

    connect(this, &MainWindow::startConnectServer, tcpClient, &TcpClient::connectServer);
    connect(tcpClient, &TcpClient::connectOK, this, [=](){
        QDateTime time = QDateTime::currentDateTime();
        ui->dialogBoxTextEdit->append(time.toString("yyyy-MM-dd hh:mm:ss")+">>>"+"连接服务器成功");
        ui->connectButton->setDisabled(true);
        ui->disconnectButton->setDisabled(false);
        ui->ipLineEdit->setDisabled(true);
        ui->portLineEdit->setDisabled(true);
    });

    connect(tcpClient, &TcpClient::disconnect, this, [=](){
        thread->quit();
        thread->wait();
        tcpClient->deleteLater();
        thread->deleteLater();
        QDateTime time = QDateTime::currentDateTime();
        ui->dialogBoxTextEdit->append(time.toString("yyyy-MM-dd hh:mm:ss")+">>>"+"服务器断开连接");
        ui->disconnectButton->setDisabled(true);
        ui->ipLineEdit->setDisabled(false);
        ui->portLineEdit->setDisabled(false);
    });

    connect(this, &MainWindow::sendData, tcpClient, &TcpClient::sendDate);

    connect(this, &MainWindow::sendFile, tcpClient, &TcpClient::sendFIle);

    connect(tcpClient, &TcpClient::textProgressValue, ui->progressBar, &QProgressBar::setValue);

    connect(tcpClient, &TcpClient::readInformation, this, &MainWindow::readInformation);

    connect(tcpClient, &TcpClient::over, this, [=](QString path){
        QDateTime time = QDateTime::currentDateTime();
        ui->dialogBoxTextEdit->append(path+"文件接受完成>>"+time.toString("yyyy-MM-dd hh:mm:ss"));
    });

    emit startConnectServer(ui->ipLineEdit->text(), ui->portLineEdit->text().toUShort());
}


void MainWindow::on_disconnectButton_clicked()
{
    thread->quit();
    thread->wait();
    tcpClient->deleteLater();
    thread->deleteLater();
    QDateTime time = QDateTime::currentDateTime();
    QMessageBox::information(this, "提示", "已断开连接");
    ui->dialogBoxTextEdit->append(time.toString("yyyy-MM-dd hh:mm:ss")+">>>"+"已断开连接");
    ui->disconnectButton->setDisabled(true);
    ui->connectButton->setDisabled(false);
    ui->ipLineEdit->setDisabled(false);
    ui->portLineEdit->setDisabled(false);

}

void MainWindow::readInformation(QByteArray data)
{
    QDateTime time = QDateTime::currentDateTime();
    ui->dialogBoxTextEdit->append("服务端："+time.toString("yyyy-MM-dd hh:mm:ss")+">>>"+data);
}
void MainWindow::on_sendButton_clicked()
{
   emit sendData(ui->sendTextEdit->toPlainText());
   QDateTime time = QDateTime::currentDateTime();
   ui->dialogBoxTextEdit->append(time.toString("yyyy-MM-dd hh:mm:ss")+">>>"+ui->sendTextEdit->toPlainText());
}

void MainWindow::on_clearSendButton_clicked()
{
    ui->sendTextEdit->clear();
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
        emit sendFile(ui->pathLineEdit->text());
    }

}


void MainWindow::on_saveDialogBoxButton_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QFileDialog::saveFileContent(ui->dialogBoxTextEdit->toPlainText().toUtf8(), "watermelonClient"+time.toString("yyyy-MM-dd hh:mm:ss")+".txt");
}


void MainWindow::on_clearDialogBoxButton_clicked()
{
    ui->dialogBoxTextEdit->clear();
}

