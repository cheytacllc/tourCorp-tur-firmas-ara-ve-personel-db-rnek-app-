#include <QThread>
#include <QRandomGenerator>
#include <QClipboard>
#include "dialog.h"
#include "ui_dialog.h"
#include "main.cpp"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->pushButton->hide();

    //statusbar ekleme
    QStatusBar* statusBar = new QStatusBar;
    ui->verticalLayout_4 ->addWidget(statusBar);
    //statusBar->addWidget(status=new QLabel);
    statusBar->addWidget(statusBtn=new QPushButton);
    statusBtn->setFlat(true);
    statusBtn->setIcon(QIcon(":/img/database_offline.png"));
    statusBtn->setText("Veritabanına bağlanılamadı ");
    Settings::setTheme(Settings::loadTheme());
    //ui->pushButton_login->setDisabled(true);
    dbconn=new DbConnection("login");
    dbthread();
    w=new MainWindow();

    //statusbar bağlama
    connect(statusBtn,SIGNAL(clicked()),this,SLOT(pushButton_statusBtn_clicked()));
}

Dialog::~Dialog()
{
    delete ui;
}

QByteArray Dialog::generateadminPassword()
{
    QByteArray key;
    QRandomGenerator *b= new QRandomGenerator;
    QRandomGenerator *s=new QRandomGenerator;
    for(int i=0;i<s->global()->bounded(10,20);i++){
        int a=b->global()->bounded(33,126);
        QChar b(a);
        key+=a;
    }
    delete b;
    delete s;
    return key.toBase64();
}

QByteArray Dialog::storeadminPassword()
{
    QByteArray key;
    QSettings s("HKEY_CURRENT_USER\\Software\\Microsoft",QSettings::NativeFormat);  // ayarlar registryde saklanmasi icin -> QSettings s("m231",QApplication::applicationName());
    s.beginGroup("Settings");
    if(s.value("adminpassword").toByteArray()=="")
        s.setValue("adminpassword",this->generateadminPassword());
    else
        key=s.value("adminpassword").toByteArray();
    s.endGroup();
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(key);
    return QByteArray::fromBase64(key);
}


void Dialog::connectDb()
{
    statusBtn->setText("Veritabanına bağlanılıyor...");
    statusBtn->setIcon(QIcon(":/img/database_reload.png"));
    if(Settings::readSettings("ServerConfig","Username").toString().isEmpty()&&QByteArray::fromBase64(Settings::readSettings("ServerConfig","Password").toByteArray()).isEmpty())
        dbconn->connectToServerMSSQL("SQL Server",Settings::readSettings("ServerConfig","Server").toString(),1433,Settings::readSettings("ServerConfig","Database").toString());
    else
        dbconn->connectToServerMSSQL("SQL Server",Settings::readSettings("ServerConfig","Server").toString(),1433,Settings::readSettings("ServerConfig","Database").toString(),Settings::readSettings("ServerConfig","Username").toString(),QByteArray::fromBase64(Settings::readSettings("ServerConfig","Password").toByteArray()));
    isConnected();
}

void Dialog::dbthread()
{
    thread = QThread::create([this]{ connectDb(); });
    thread->start();
}

void Dialog::disconnectDb()
{
    dbconn->db.close();
}

bool Dialog::isConnected()
{
    if(dbconn->db.isOpen())
    {
        statusBtn->setIcon(QIcon(":/img/database_online.png"));
        statusBtn->setText("Veritabanı bağlantısı başarılı ");
        //ui->pushButton_login->setEnabled(true);
    }
    else
    {
        statusBtn->setIcon(QIcon(":/img/database_offline.png"));
        statusBtn->setText("Veritabanı bağlantısı başarısız ");
    }

    return dbconn->db.isOpen();
}

void Dialog::on_pushButton_login_clicked()
{
    if(ui->lineEdit_username->text()=="admin"&&ui->lineEdit_2_password->text()!=storeadminPassword())
    {
        QMessageBox q(QMessageBox::Warning,"Yönetici Girişi Algılandı","Yönetici şifrenizi öğrenebilmek için "+storeadminPassword().toBase64()+" kodunu geliştiriciye bildirmelisiniz");
        q.setTextInteractionFlags(Qt::TextSelectableByMouse);
        q.exec();
    }
    else if (ui->lineEdit_username->text()=="admin"&&ui->lineEdit_2_password->text()==storeadminPassword()) {
        this->close();
        QTimer::singleShot(1000, this, SLOT(disconnectDb()));
        thread->quit();
        w->show();
    }
    else
    {
        if(dbconn->db.isOpen())
        {
            QSqlQuery checkLogin(dbconn->db);
            checkLogin.exec("SELECT KullanıcıAdı, Sifre, Yetki_ID FROM T_Kullanıcılar");
            while (checkLogin.next())
            {
                if(checkLogin.value("KullanıcıAdı").toString()==ui->lineEdit_username->text()&&checkLogin.value("Sifre")==ui->lineEdit_2_password->text().toUtf8().toBase64())
                {
                    w->checkPermissions(checkLogin.value("Yetki_ID").toInt());
                    w->user->setText(" Kullanıcı adı: "+ui->lineEdit_username->text());
                    this->close();
                    QTimer::singleShot(1000, this, SLOT(disconnectDb()));
                    //dbconn->db.close();
                    thread->quit();
                    w->show();

                }
                //else
                    //QMessageBox::warning(this,"kullanıcı bulunamadı","kullanıcı adı ya da şifrenizi yanlış girdiniz");
            }
        }
    }
}

void Dialog::pushButton_statusBtn_clicked()
{
    sd=new ServerDialog();
    connect(sd,SIGNAL(accepted()),this,SLOT(dbthread()));
    sd->exec();
}

void Dialog::on_pushButton_pressed()
{
    ui->lineEdit_2_password->setEchoMode(QLineEdit::Normal);
}


void Dialog::on_pushButton_released()
{
    ui->lineEdit_2_password->setEchoMode(QLineEdit::Password);
}

void Dialog::on_lineEdit_2_password_textEdited(const QString &arg1)
{
    if(!arg1.isEmpty())
        ui->pushButton->show();
    else
        ui->pushButton->hide();
}

