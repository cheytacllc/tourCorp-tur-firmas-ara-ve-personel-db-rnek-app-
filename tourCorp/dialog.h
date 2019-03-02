#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QStatusBar>
#include <QLayout>
#include <QLabel>
#include "mainwindow.h"
#include "dbconnection.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    MainWindow *w;
    QLabel *status;
    QPushButton *statusBtn;
    QPushButton *pass;
    ServerDialog *sd;
    DbConnection *dbconn;
    QThread *thread;
    QByteArray generateadminPassword();
    QByteArray storeadminPassword();
    ~Dialog();

public slots:
    void pushButton_statusBtn_clicked();
    bool isConnected();
    void connectDb();
    void dbthread();
    void disconnectDb();
private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_lineEdit_2_password_textEdited(const QString &arg1);


private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
