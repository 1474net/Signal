#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sock = new QTcpSocket(this);
    player = new QMediaPlayer;
    player->setMedia(QUrl("qrc:/new/sound/03281.mp3"));
    player->setVolume(100);
    msgBox = new QMessageBox;


    connect(sock, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(sock, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(sock, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(sock, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    onSockConnect();
//    loop();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSockConnect()
{
    sock->connectToHost("1474net.tk",9090);
}

void MainWindow::onSokConnected()
{
    sock->write("Reade");
    QString str = ui->label->text();
    ui->label->setText("Статус: <font color=Green>Online");
}

void MainWindow::onSokReadyRead()
{
    QString str;
    while (sock->bytesAvailable())
        str = sock->readAll();
    ui->label_2->setText(str);
    if (str=="Signal" && !mes)
    {
       mes = true;
       player->play();
       msgBox->setText("Внимание");
       msgBox->setIcon(QMessageBox::Critical);
       msgBox->setWindowTitle("Сообщение");
       msgBox->setInformativeText("На сервер пришло уведомление                   Проверти \"Скаут-Студия\"");
       msgBox->setStandardButtons(QMessageBox::Ok);
       msgBox->setDefaultButton(QMessageBox::Ok);
       int ret = msgBox->exec();
       ui->label_2->setText("Получин сигнал");

       if (ret == QMessageBox::Ok){
           player->stop();
       }
       mes = false;
    }

}

void MainWindow::onSokDisconnected()
{

}

void MainWindow::onSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        ui->label->setText("Error The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        ui->label->setText("Error The connection was refused by the peer.");
        break;
    default:
        ui->label->setText("Error The following error occurred: "+sock->errorString());
    }
}

void MainWindow::loop(){
//    while(true){
//        status =  sock->state();
//        if (status){
//            continue;
//        }
//        else{
//            onSockConnect();
//            ui->label->setText("connect offline");
//        }
//    }

}
