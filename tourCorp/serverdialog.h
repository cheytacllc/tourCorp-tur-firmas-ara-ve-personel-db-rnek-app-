#ifndef SERVERDIALOG_H
#define SERVERDIALOG_H

#include <QCloseEvent>
#include <QDialog>
#include <QSqlDatabase>
#include "settings.h"
#include "dbconnection.h"


namespace Ui {
class ServerDialog;
}

class ServerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ServerDialog(QWidget *parent = nullptr);
    void closeEvent(QCloseEvent *event);
    bool cancelButton=false, acceptButton=false;
    DbConnection *dbconn;
    ~ServerDialog();

signals:
void serverDialogClosing();

private slots:

    void on_lineEdit_password_textEdited(const QString &arg1);

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

public slots:
    void connectDb();
    bool isConnected();
    void disconnectDb();

private:
    Ui::ServerDialog *ui;
};



#endif // SERVERDIALOG_H
