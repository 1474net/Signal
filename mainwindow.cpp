#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this -> setTrayIconActions();
    this -> showTrayIcon();

    QIcon winIcon(":/pict/signal.png");
    this->setWindowIcon(winIcon);
    this -> setFixedSize(400,300);

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
//    loop();:
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
       msgBox->setInformativeText("На сервер пришло уведомление                   "
                                  "Проверти \"Скаут-Студия\"");
       msgBox->setStandardButtons(QMessageBox::Ok);
       msgBox->setDefaultButton(QMessageBox::Ok);
       msgBox->setWindowFlag(Qt::WindowStaysOnTopHint);
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
//***************************
//Натройки траея программы
//**************************
void MainWindow::showTrayIcon()
{
    // метод, в котором будет создан объект класса QSystemTrayIcon,
    // заданы его свойства, добавлено простое контекстное меню и обработчик клика по значку в трее:
    // Создаём экземпляр класса и задаём его свойства...
    trayIcon = new QSystemTrayIcon(this);
    QIcon trayImage(":/pict/signal.png");
    trayIcon -> setIcon(trayImage);
    trayIcon -> setContextMenu(trayIconMenu);

    // Подключаем обработчик клика по иконке...
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));

    // Выводим значок...
    trayIcon -> show();
}
void MainWindow::trayActionExecute()
{
//    Добавьте методы обработки кликов по иконке в системном трее:
        if (isHidden()){
            this->showNormal();
            this->activateWindow();
        }
        else{
            this->hide();
        }
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            this -> trayActionExecute();
            break;
        default:
            break;
    }
}
void MainWindow::setTrayIconActions()
{
//    Создайте и добавьте контекстное меню для нашего значка:
    // Setting actions...
    minimizeAction = new QAction("Свернуть", this);
    restoreAction = new QAction("Восстановить", this);
    quitAction = new QAction("Выход", this);

    // Connecting actions to slots...
    connect (minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));
    connect (restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    connect (quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    // Setting system tray's icon menu...
    trayIconMenu = new QMenu(this);
    trayIconMenu -> addAction (minimizeAction);
    trayIconMenu -> addAction (restoreAction);
    trayIconMenu -> addAction (quitAction);
}

void MainWindow::changeEvent(QEvent *event)
{
//    перехватчик события сворачивания формы на панель задач и вместо этого свернём её в трей:
    QMainWindow::changeEvent(event);
    if (event -> type() == QEvent::WindowStateChange)
    {
        if (isMinimized())
        {
            this -> hide();
        }
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
