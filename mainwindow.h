#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTcpSocket>
#include <QAbstractsocket>
#include <QString>
#include <QMediaPlayer>
#include <QUrl>
#include <QMessageBox>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onSockConnect();
    void loop();
private slots:
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);
private:
    Ui::MainWindow *ui;
    QTcpSocket *sock;
    bool status;
    QMediaPlayer *player;
    QMessageBox *msgBox;
    bool mes=false;
};

#endif // MAINWINDOW_H
