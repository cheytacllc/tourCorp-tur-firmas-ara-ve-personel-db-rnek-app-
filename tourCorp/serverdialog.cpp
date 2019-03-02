#include "serverdialog.h"
#include "ui_serverdialog.h"
#include "dialog.h"
#include <QDebug>
#include <QThread>

ServerDialog::ServerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ServerDialog)
{
    ui->setupUi(this);
    ui->pushButton->hide();
    ui->label_2->setText("Not Connected");
    ui->lineEdit_username->setText(Settings::readSettings("ServerConfig","Username").toString());
    ui->lineEdit_password->setText(QByteArray::fromBase64(Settings::readSettings("ServerConfig","Password").toByteArray()));
    ui->lineEdit_server->setText(Settings::readSettings("ServerConfig","Server").toString());
    ui->lineEdit_db->setText(Settings::readSettings("ServerConfig","Database").toString());
    ui->comboBox->setCurrentIndex(Settings::loadTheme());
    dbconn=new DbConnection("config");
    QThread *thread = QThread::create([this]{ connectDb(); });
    thread->start();
}

void ServerDialog::closeEvent(QCloseEvent *event)
{
    event->ignore();
    if(!cancelButton)
    {
        if(isConnected())
        {
            this->done(1);
            QTimer::singleShot(1000, this, SLOT(disconnectDb()));
            event->accept();
        }
        else
        {
            if(!acceptButton)
            {
                this->done(1);
                QTimer::singleShot(1000, this, SLOT(disconnectDb()));
                event->accept();
            }
            else
                QMessageBox::warning(this,"DB connection error","Please double check the parameters");
        }
    }
    else
    {
        this->done(1);
        QTimer::singleShot(1000, this, SLOT(disconnectDb()));
        event->accept();

    }
}



void ServerDialog::connectDb()
{
    ui->label_2->setText("Connecting...");
    ui->label->setPixmap(QPixmap(":/img/database_reload.png"));
    if(ui->lineEdit_username->text().isEmpty()&&ui->lineEdit_password->text().isEmpty())
          dbconn->connectToServerMSSQL("SQL Server",ui->lineEdit_server->text(),1433,ui->lineEdit_db->text());
    else
        dbconn->connectToServerMSSQL("SQL Server",ui->lineEdit_server->text(),1433,ui->lineEdit_db->text(),ui->lineEdit_username->text(),ui->lineEdit_password->text());
    isConnected();
}

bool ServerDialog::isConnected()
{
    if(dbconn->db.isOpen())
    {
        ui->label->setPixmap(QPixmap(":/img/database_online.png"));
        ui->label_2->setText("Connected");
    }
    else
    {
        ui->label->setPixmap(QPixmap(":/img/database_offline.png"));
        ui->label_2->setText("Not Connected");
    }
    return dbconn->db.isOpen();
}

ServerDialog::~ServerDialog()
{
    delete ui;
}

void ServerDialog::on_lineEdit_password_textEdited(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->pushButton->hide();
    else
        ui->pushButton->show();
}

void ServerDialog::on_pushButton_pressed()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
}

void ServerDialog::on_pushButton_released()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

void ServerDialog::on_comboBox_currentIndexChanged(int index)
{
    Settings::setTheme(index);
}

void ServerDialog::on_pushButton_ok_clicked()
{
    Settings::writeSettings("ServerConfig","Username",ui->lineEdit_username->text());
    Settings::writeSettings("ServerConfig","Password",ui->lineEdit_password->text().toUtf8().toBase64());
    Settings::writeSettings("ServerConfig","Server",ui->lineEdit_server->text());
    Settings::writeSettings("ServerConfig","Database",ui->lineEdit_db->text());
    Settings::setTheme(ui->comboBox->currentIndex());
    acceptButton=true;
    QThread *thread = QThread::create([this]{ connectDb(); });
    thread->start();
    connect(thread,SIGNAL(finished()),this,SLOT(close()));
}

void ServerDialog::on_pushButton_cancel_clicked()
{
    cancelButton=true;
    QTimer::singleShot(1000, this, SLOT(disconnectDb()));
    this->close();
}

void ServerDialog::disconnectDb()
{
    dbconn->db.close();
}
