#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialog.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setMaximumHeight(100);
    ui->tabWidget->setMinimumHeight(100);
    ui->toolBar_admin->setParent(ui->tab_admin);
    ui->toolBar_personel->setParent(ui->tab_personel);
    ui->toolBar_arac->setParent(ui->tab_arac);
    ui->toolBar_finans->setParent(ui->tab_finans);
    ui->stackedWidget->setCurrentIndex(0);
    ui->listWidget->clear();
    ui->dateEdit_isegiris->setDate(QDate::currentDate());
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit_izinbas->setDate(QDate::currentDate());
    ui->dateEdit_izinbitis->setDate(QDate::currentDate());
    ui->pushButton_pwd->hide();
    ui->pushButton_aracgoster->hide();
    ui->tableView_admin->setSortingEnabled(true);
    ui->tableView_arac->setSortingEnabled(true);
    ui->tableView_izin->setSortingEnabled(true);
    ui->tableView_maas->setSortingEnabled(true);
    ui->tableView_personel->setSortingEnabled(true);
    ui->tableView_ucret->setSortingEnabled(true);

    //statusbar db conn durumu
    icon=new QLabel();
    status=new QLabel("Veritabanına bağlanılamadı");
    user=new QLabel("User: "+loginUser);
    yetki=new QLabel("Yetki: "+yetkiStr);
    icon->setPixmap(QPixmap(":/img/database_offline.png"));
    ui->statusbar->addWidget(icon);
    ui->statusbar->addWidget(status);
    ui->statusbar->addWidget(user);
    ui->statusbar->addWidget(yetki);

    //multithread db bağlantısı
    dbconn=new DbConnection("main");
    dbthread();

    //lisboxa dinamik olarak araç çubuğundakileri ekleme
    actionList=ui->toolBar_admin->actions()<<ui->toolBar_personel->actions()<<ui->toolBar_arac->actions()<<ui->toolBar_finans->actions();
    for(int i=0;i<actionList.length();i++)
    {
        if(actionList.at(i)->text().isEmpty()||actionList.at(i)->text().contains("Çıkış"))
        {
            actionList.removeAt(i);
        }
        else
        {
            item=new QListWidgetItem(actionList.at(i)->text(),ui->listWidget);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsUserTristate);
            item->setIcon(actionList.at(i)->icon());
            item->setCheckState(Qt::Unchecked);
        }
    }

    buttonListAdmin.append(ui->pushButton_yetkiekle);
    buttonListAdmin.append(ui->pushButton_yetkiguncelle);
    buttonListAdmin.append(ui->pushButton_ekle);
    buttonListAdmin.append(ui->pushButton_sil);
    buttonListAdmin.append(ui->pushButton_guncelle);

    buttonListArac.append(ui->pushButton_eklearac);
    buttonListArac.append(ui->pushButton_icdisarac);
    buttonListArac.append(ui->pushButton_silarac);

    buttonListPersonel.append(ui->pushButton_eklepersonel);
    buttonListPersonel.append(ui->pushButton_guncpersonel);
    buttonListPersonel.append(ui->pushButton_icdispersonel);
    buttonListPersonel.append(ui->pushButton_silpersonel);

    buttonListIzin.append(ui->pushButton_guncizin);
    buttonListIzin.append(ui->pushButton_ekleizin);

    buttonListUcret.append(ui->pushButton_ucretekle);
    buttonListUcret.append(ui->pushButton_ucretgunc);

}

//açık sayfayı seçili yapma
void MainWindow::setActionCheckStates(QToolBar *toolbar, QAction *action)
{
    for(int i=0; i<toolbar->actions().length(); i++)
    {
        toolbar->actions().at(i)->setChecked(false);
    }
    if(action->parentWidget()!=NULL)
    {
        action->setChecked(true);
        tempAction=action;
        tempToolBar=toolbar;
        currentTabIndex=ui->tabWidget->currentIndex();
    }
}

void MainWindow::disconnectDb()
{
    dbconn->db.close();
}

void MainWindow::checkPermissions(int id)
{
    loginYetkiID=id;
    for(int i=0;i<actionList.length();i++)
    {
        if(actionList.at(i)->text().isEmpty()||actionList.at(i)->text().contains("Çıkış"))
        {
            actionList.removeAt(i);
        }
    }

    QSqlQuery permQuery(dbconn->db);
    QString query="SELECT * FROM T_Yetki WHERE ID="+QString::number(id);
    permQuery.exec(query);
    while (permQuery.next())
    {
        yetki->setText(" Yetki: "+permQuery.value(1).toString());
        for(int i=0;i<actionList.length();i++)
        {
            switch (permQuery.value(i+2).toInt())
            {
            case 0:
            {
                if(i==0)
                    hideAdmin();
                actionList.at(i)->setDisabled(true);
                if(actionList.at(i)->text().contains("Çıkış"))
                    actionList.at(i)->setDisabled(false);
                break;
            }
            case 1:
            {
                switch (i) {
                case 0:
                    for(int j=0;j<buttonListAdmin.length();j++)
                    {
                        buttonListAdmin.at(j)->setDisabled(true);
                    }
                    break;
                case 3:
                    for(int j=0;j<buttonListPersonel.length();j++)
                    {
                        buttonListPersonel.at(j)->setDisabled(true);
                    }
                    break;
                case 4:
                    for(int j=0;j<buttonListIzin.length();j++)
                    {
                        buttonListIzin.at(j)->setDisabled(true);
                    }
                    break;
                case 5:
                    for(int j=0;j<buttonListArac.length();j++)
                    {
                        buttonListArac.at(j)->setDisabled(true);
                    }
                    break;
                case 6:
                    for(int j=0;j<buttonListUcret.length();j++)
                    {
                        buttonListUcret.at(j)->setDisabled(true);
                    }
                    break;
                }
                actionList.at(i)->setEnabled(true);
                break;
            }
            case 2:
                actionList.at(i)->setEnabled(true);
                break;
            default:
                break;
            }
        }
    }
}

void MainWindow::on_lineEdit_password_textEdited(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->pushButton_pwd->hide();
    else
        ui->pushButton_pwd->show();
}

void MainWindow::on_lineEdit_password_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->pushButton_pwd->hide();
    else
        ui->pushButton_pwd->show();
}

void MainWindow::hideAdmin()
{
    ui->tabWidget->removeTab(0);
}

int MainWindow::idBul(QString query,QString data)
{
    QSqlQuery qID(dbconn->db);
    qID.prepare(query);
    qID.addBindValue(data);
    qID.exec();
    while (qID.next()) {
        if(!qID.value(0).isNull())
            return qID.value(0).toInt();
    }
}

MainWindow::~MainWindow()
{
    QTimer::singleShot(1000, this, SLOT(disconnectDb()));
    delete ui;
}

//db bağlantısı
void MainWindow::connectDb()
{
    status->setText("Veritabanına bağlanılıyor... ");
    icon->setPixmap(QPixmap(":/img/database_reload.png"));
    if(Settings::readSettings("ServerConfig","Username").toString().isEmpty()&&QByteArray::fromBase64(Settings::readSettings("ServerConfig","Password").toByteArray()).isEmpty())
        dbconn->connectToServerMSSQL("SQL Server",Settings::readSettings("ServerConfig","Server").toString(),1433,Settings::readSettings("ServerConfig","Database").toString());
    else
        dbconn->connectToServerMSSQL("SQL Server",Settings::readSettings("ServerConfig","Server").toString(),1433,Settings::readSettings("ServerConfig","Database").toString(),Settings::readSettings("ServerConfig","Username").toString(),QByteArray::fromBase64(Settings::readSettings("ServerConfig","Password").toByteArray()));
    isConnected();
}

void MainWindow::connectDbBR()
{
    status->setText("Veritabanına bağlanılıyor... ");
    icon->setPixmap(QPixmap(":/img/database_reload.png"));
    dbconn->connectToServerMSSQL("SQL Server","",1433,"master");
    isConnected();
}

//db bağlantı için multithread
void MainWindow::dbthread()
{
    thread = QThread::create([this]{ connectDb(); });
    thread->start();
}

void MainWindow::dbthreadBR()
{
    thread = QThread::create([this]{ connectDbBR(); });
    thread->start();
}

//db bağlantı durumunu statusbarda gösterme
bool MainWindow::isConnected()
{
    if(dbconn->db.isOpen())
    {
        icon->setPixmap(QPixmap(":/img/database_online.png"));
        status->setText("Veritabanı bağlantısı başarılı ");
    }
    else
    {
        icon->setPixmap(QPixmap(":/img/database_offline.png"));
        status->setText("Veritabanı bağlantısı başarısız ");
    }
    return dbconn->db.isOpen();
}

void MainWindow::on_actioncikis_triggered()
{
    this->close();
    QTimer::singleShot(1000, this, SLOT(disconnectDb()));
    thread->quit();
    Dialog *d=new Dialog();
    d->exec();
}

//sayfa seçilme durumlarını koruma
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(currentTabIndex!=index)
    {
        setActionCheckStates(ui->toolBar_personel,new QAction(NULL));
        setActionCheckStates(ui->toolBar_admin,new QAction(NULL));
        setActionCheckStates(ui->toolBar_arac,new QAction(NULL));
        setActionCheckStates(ui->toolBar_finans,new QAction(NULL));
    }
    else
    {
        setActionCheckStates(tempToolBar,tempAction);
    }
}

//menüler
void MainWindow::on_actionKullaniciEkle_triggered()
{
    setActionCheckStates(ui->toolBar_admin, ui->actionKullaniciEkle);
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionPersonelYonetimi_triggered()
{
    setActionCheckStates(ui->toolBar_personel, ui->actionPersonelYonetimi);
    ui->stackedWidget->setCurrentIndex(2);
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT Gorev FROM T_PerGorev",dbconn->db);
    ui->comboBox_gorev->setModel(model);
}

void MainWindow::on_actionAracYonetimi_triggered()
{
    setActionCheckStates(ui->toolBar_arac, ui->actionAracYonetimi);
    ui->stackedWidget->setCurrentIndex(5);
    QSqlQueryModel *arac = new QSqlQueryModel();
    arac->setQuery("SELECT TC_KimlikNo FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID WHERE Gorev='Sürücü'",dbconn->db);
    ui->comboBox_aracsurucutc->setModel(arac);
    QSqlQueryModel *arac1 = new QSqlQueryModel();
    arac1->setQuery("SELECT Marka FROM T_Arac",dbconn->db);
    ui->comboBox_aracmarka->setModel(arac1);
    QSqlQueryModel *arac2 = new QSqlQueryModel();
    arac2->setQuery("SELECT Model FROM T_Arac",dbconn->db);
    ui->comboBox_aracmodel->setModel(arac2);
    QSqlQueryModel *arac3 = new QSqlQueryModel();
    arac3->setQuery("SELECT Plaka FROM T_Arac WHERE Sürücü_TC=NULL",dbconn->db);
    ui->comboBox_aracplaka->setModel(arac3);
}

void MainWindow::on_action_calismaSaatleri_triggered()
{
    setActionCheckStates(ui->toolBar_personel, ui->action_calismaSaatleri);
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_actionIzinler_triggered()
{
    setActionCheckStates(ui->toolBar_personel, ui->actionIzinler);
    ui->stackedWidget->setCurrentIndex(4);
    QSqlQueryModel *tc = new QSqlQueryModel();
    tc->setQuery("SELECT TC_KimlikNo FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID",dbconn->db);
    ui->comboBox_izintc->setModel(tc);
    QSqlQueryModel *ad = new QSqlQueryModel();
    ad->setQuery("SELECT Ad FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID",dbconn->db);
    ui->comboBox_izinad->setModel(ad);
    QSqlQueryModel *soyad = new QSqlQueryModel();
    soyad->setQuery("SELECT Soyad FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID",dbconn->db);
    ui->comboBox_izinsoyad->setModel(soyad);
}

void MainWindow::on_actionBirimUcret_triggered()
{
    setActionCheckStates(ui->toolBar_finans, ui->actionBirimUcret);
    ui->stackedWidget->setCurrentIndex(6);
    QSqlQueryModel *ucret = new QSqlQueryModel();
    ucret->setQuery("SELECT Gorev FROM T_PerGorev",dbconn->db);
    ui->comboBox_gorevucret->setModel(ucret);
}

void MainWindow::on_actionMaaslar_triggered()
{
    setActionCheckStates(ui->toolBar_finans, ui->actionMaaslar);
    ui->stackedWidget->setCurrentIndex(7);
}

//filtre sorguları
void MainWindow::filterPersonel()
{
    ui->tableView_personel->reset();
    QSqlQueryModel *model = new QSqlQueryModel;
    QString w;
    percikis=filter->ckpersonel;
    if(!filter->where.isEmpty()||filter->ckpersonel)
    {
        if(filter->ckpersonel)
        {
            if(filter->where.isEmpty())
            w="SELECT TC_KimlikNo,Ad,Soyad,CONVERT(varchar, GirisTarihi, 103),CONVERT(varchar, CikisTarihi, 103),Gorev FROM T_CikanPersonel";
            else
                w="SELECT TC_KimlikNo,Ad,Soyad,CONVERT(varchar, GirisTarihi, 103),CONVERT(varchar, CikisTarihi, 103),Gorev FROM T_CikanPersonel WHERE "+filter->where;
        }
        else
        {
            w="SELECT TC_KimlikNo,Ad,Soyad,CONVERT(varchar, GirisTarihi, 103),Gorev,Gorev_ID,Maas FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID WHERE "+filter->where;
        }

        model->setQuery(w,dbconn->db);
        model->setHeaderData(0, Qt::Horizontal, tr("TC Kimlik No"));
        model->setHeaderData(1, Qt::Horizontal, tr("Ad"));
        model->setHeaderData(2, Qt::Horizontal, tr("Soyad"));
        model->setHeaderData(3, Qt::Horizontal, tr("Giriş Tarihi"));
        if(filter->ckpersonel)
        {
            model->setHeaderData(4, Qt::Horizontal, tr("Çıkış Tarihi"));
            ui->tableView_personel->showColumn(5);
        }
        else
        {
            model->setHeaderData(4, Qt::Horizontal, tr("Görevi"));
            ui->tableView_personel->hideColumn(5);
        }

        ui->tableView_personel->setModel(model);
        ui->tableView_personel->update();
        //QMessageBox::information(this,"",filter->where);

    }
    else
        on_pushButton_gosterpersonel_clicked();
}

void MainWindow::filterIzin()
{
    ui->tableView_izin->reset();
    QSqlQueryModel *model = new QSqlQueryModel;
    if(!filter->where.isEmpty())
    {
        QString w="SELECT TC_KimlikNo,Ad,Soyad,Gorev,CONVERT(varchar, IzinBas, 103),CONVERT(varchar, IzinBit, 103) FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID JOIN T_Izin ON T_Personel.TC_KimlikNo=T_Izin.Personel_TC WHERE "+filter->where;
        model->setQuery(w,dbconn->db);
        model->setHeaderData(0, Qt::Horizontal, tr("TC Kimlik No"));
        model->setHeaderData(1, Qt::Horizontal, tr("Ad"));
        model->setHeaderData(2, Qt::Horizontal, tr("Soyad"));
        model->setHeaderData(3, Qt::Horizontal, tr("Görev"));
        model->setHeaderData(4, Qt::Horizontal, tr("İzin Başlangıç"));
        model->setHeaderData(5, Qt::Horizontal, tr("İzin Bitiş"));
        ui->tableView_izin->setModel(model);
        ui->tableView_izin->update();
        //QMessageBox::information(this,"",filter->where);
    }
    else
        on_pushButton_gosterizin_clicked();
}

void MainWindow::filterArac()
{
    ui->tableView_arac->reset();
    QSqlQueryModel *model = new QSqlQueryModel;
    QString w;
    if(!filter->where.isEmpty()||filter->cksurucu)
    {
        if(filter->cksurucu)
        {
            if(!filter->where.isEmpty())
                w="SELECT Marka,Model,Yıl,Plaka FROM T_Arac WHERE "+filter->where.replace("TC","Sürücü_TC");
            else
                w="SELECT Marka,Model,Yıl,Plaka FROM T_Arac WHERE Sürücü_TC IS NULL";
        }
        else
            w="SELECT Marka,Model,Yıl,Plaka,Ad,Soyad,TC_KimlikNo FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID JOIN T_Arac ON T_Personel.TC_KimlikNo=T_Arac.Sürücü_TC WHERE "+filter->where.replace("TC","Sürücü_TC");

        model->setQuery(w,dbconn->db);
        if(!filter->cksurucu)
        {
            model->setHeaderData(4, Qt::Horizontal, tr("Sürücü Ad"));
            model->setHeaderData(5, Qt::Horizontal, tr("Sürücü Soyad"));
        }
        ui->tableView_arac->setModel(model);
        ui->tableView_arac->hideColumn(6);
    }
    else
        on_pushButton_gosterarac_clicked();
}

void MainWindow::filterMaas()
{
    QSqlQueryModel *model = new QSqlQueryModel;
    QString w;
    if(!filter->where.isEmpty())
    {
            w="SELECT TC_KimlikNo,Ad,Soyad,Gorev,FORMAT(Dönem,'yyyy/MM') as 'Dönem',T_Maas.Maas FROM T_PerGorev JOIN T_Personel ON T_Personel.Gorev_ID=T_PerGorev.ID JOIN T_Maas ON T_Maas.Personel_TC=T_Personel.TC_KimlikNo WHERE "+filter->where;

        model->setQuery(w,dbconn->db);
        ui->tableView_maas->setModel(model);
    }
    else
        on_pushButton_gostermaas_clicked();
}

//filtre pencereleri
void MainWindow::on_pushButton_filtrepersonel_clicked()
{
    filter=new FilterDialog();
    filter->personel();
    filter->exec();
    if(filter->result()==1)
        filterPersonel();
    connect(filter,SIGNAL(accepted()),this,SLOT(filterPersonel()));
}

void MainWindow::on_pushButton_filtreizin_clicked()
{
    filter=new FilterDialog();
    filter->izinler();
    filter->exec();
    if(filter->result()==1)
        filterIzin();
    connect(filter,SIGNAL(accepted()),this,SLOT(filterIzin()));
}

void MainWindow::on_pushButton_filtrearac_clicked()
{
    filter=new FilterDialog();
    filter->arac();
    filter->exec();
    if(filter->result()==1)
        filterArac();
}

void MainWindow::on_pushButton_filtremaas_clicked()
{
    filter=new FilterDialog();
    filter->maaslar();
    filter->exec();
    if(filter->result()==1)
        filterMaas();
}


void MainWindow::on_comboBox_yetki_currentTextChanged(const QString &arg1)
{
    yetkiID_comboBox=idBul("SELECT ID FROM T_Yetki WHERE YetkiAdı=?",arg1);
}

void MainWindow::on_pushButton_pwd_pressed()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
}

void MainWindow::on_pushButton_pwd_released()
{
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

void MainWindow::on_pushButton_kullgoster_clicked()
{
    ui->tableView_admin->showColumn(0);
    QSqlQueryModel *yetki = new QSqlQueryModel;
    yetki->setQuery("SELECT YetkiAdı FROM T_Yetki",dbconn->db);
    ui->comboBox_yetki->setModel(yetki);

    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT KullanıcıAdı,Sifre,YetkiAdı FROM T_Kullanıcılar JOIN T_Yetki ON T_Kullanıcılar.Yetki_ID=T_Yetki.ID",dbconn->db);
    model->setHeaderData(0, Qt::Horizontal, tr("Kullanıcı Adı"));
    model->setHeaderData(2, Qt::Horizontal, tr("Yetki"));
    ui->tableView_admin->setModel(model);
    ui->tableView_admin->hideColumn(1);
}

void MainWindow::on_pushButton_yetkigoster_clicked()
{
    ui->tableView_admin->showColumn(1);
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT * FROM T_Yetki",dbconn->db);
    model->setHeaderData(1, Qt::Horizontal, tr("Yetki Adı"));
    model->setHeaderData(2, Qt::Horizontal, tr("Kullanıcı Ekle"));
    model->setHeaderData(3, Qt::Horizontal, tr("Veritabanı Yedekle"));
    model->setHeaderData(4, Qt::Horizontal, tr("Veritabanı Geri Yükle"));
    model->setHeaderData(5, Qt::Horizontal, tr("Personel Yönetimi"));
    model->setHeaderData(6, Qt::Horizontal, tr("İzinler"));
    model->setHeaderData(7, Qt::Horizontal, tr("Araç Yönetimi"));
    model->setHeaderData(8, Qt::Horizontal, tr("Birim Ücret"));
    model->setHeaderData(9, Qt::Horizontal, tr("Maaşlar"));
    ui->tableView_admin->setModel(model);
    ui->tableView_admin->hideColumn(0);
    ui->tableView_admin->update();
}

void MainWindow::on_tableView_admin_clicked(const QModelIndex &index)
{
    QModelIndexList indexes = ui->tableView_admin->selectionModel()->selection().indexes();
    if(indexes.length()>4)
    {
        yetkiID_tableView=indexes.at(0).data().toInt();
        ui->lineEdit_yetki->setText(indexes.at(1).data().toString());
        for(int i=0;i<ui->listWidget->count();i++)
        {
            switch (indexes.at(i+2).data().toInt()) {
            case 0:
                ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
                break;
            case 1:
                ui->listWidget->item(i)->setCheckState(Qt::PartiallyChecked);
                break;
            case 2:
                ui->listWidget->item(i)->setCheckState(Qt::Checked);
                break;
            default:
                break;
            }
        }
    }
    else
    {
        kullaniciAdi=indexes.at(0).data().toString();
        ui->lineEdit_username->setText(indexes.at(0).data().toString());
        ui->lineEdit_password->setText(QByteArray::fromBase64(indexes.at(1).data().toByteArray()));
        ui->comboBox_yetki->setCurrentText(indexes.at(2).data().toString());
    }
}

void MainWindow::on_pushButton_yetkiekle_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("INSERT INTO T_Yetki (YetkiAdı,Admin,Yedekle,GeriYükle,Personel,Izinler,Arac,Ucret,Maas) VALUES (?,?,?,?,?,?,?,?,?)");
        query.addBindValue(ui->lineEdit_yetki->text());
        for(int i=0;i<ui->listWidget->count();i++)
        {
            query.addBindValue(ui->listWidget->item(i)->checkState());
        }
        if(query.exec())
            QMessageBox::information(this,"başarılı","eklendi");
        else
            QMessageBox::critical(this,"başarısız",query.lastError().databaseText());
    }
    else
        QMessageBox::critical(this,"başarısız",dbconn->db.lastError().driverText());
}

void MainWindow::on_pushButton_yetkiguncelle_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("UPDATE T_Yetki SET YetkiAdı=?,Admin=?,Yedekle=?,GeriYükle=?,Personel=?,Izinler=?,Arac=?,Ucret=?,Maas=? WHERE ID=?");
        query.addBindValue(ui->lineEdit_yetki->text());
        for(int i=0;i<ui->listWidget->count();i++)
        {
            query.addBindValue(ui->listWidget->item(i)->checkState());
        }
        query.addBindValue(yetkiID_tableView);
        if(query.exec())
        {
            QMessageBox::information(this,"başarılı","güncellendi");
            checkPermissions(loginYetkiID);
        }
        else
            QMessageBox::critical(this,"başarısız","güncellenemedi");
    }
}

void MainWindow::on_pushButton_ekle_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("INSERT INTO T_Kullanıcılar (KullanıcıAdı,Sifre,Yetki_ID) VALUES (?,?,?)");
        query.addBindValue(ui->lineEdit_username->text());
        query.addBindValue(ui->lineEdit_password->text().toUtf8().toBase64());
        query.addBindValue(yetkiID_comboBox);
        if(query.exec())
            QMessageBox::information(this,"başarılı","eklendi");
        else
            QMessageBox::critical(this,"başarısız",query.lastError().databaseText());
    }
    else
        QMessageBox::critical(this,"başarısız",dbconn->db.lastError().driverText());

}

void MainWindow::on_pushButton_sil_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("DELETE FROM T_Kullanıcılar WHERE KullanıcıAdı=?");
        query.addBindValue(ui->lineEdit_username->text());
        if(query.exec())
            QMessageBox::information(this,"başarılı","silindi");
        else
            QMessageBox::critical(this,"başarısız","silinemedi");
    }
}

void MainWindow::on_pushButton_guncelle_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("UPDATE T_Kullanıcılar SET KullanıcıAdı=?,Sifre=?,Yetki_ID=? WHERE KullanıcıAdı=?");
        query.addBindValue(ui->lineEdit_username->text());
        query.addBindValue(ui->lineEdit_password->text().toUtf8().toBase64());
        query.addBindValue(yetkiID_comboBox);
        query.addBindValue(kullaniciAdi);
        if(query.exec())
            QMessageBox::information(this,"başarılı","güncellendi");
        else
            QMessageBox::critical(this,"başarısız","güncellenemedi");
    }
}


void MainWindow::on_pushButton_eklepersonel_clicked()
{
    if(dbconn->db.isOpen())
    {
        int gorevID=0;

        QSqlQuery qGorevEkle(dbconn->db);
        qGorevEkle.prepare("INSERT INTO T_PerGorev (Gorev) VALUES (?)");
        qGorevEkle.addBindValue(ui->comboBox_gorev->currentText());

        if(qGorevEkle.exec())
            gorevID=qGorevEkle.lastInsertId().toInt();
        else
            gorevID=idBul("SELECT ID FROM T_PerGorev WHERE Gorev=?",ui->comboBox_gorev->currentText());

        QSqlQuery query(dbconn->db);
        query.prepare("INSERT INTO T_Personel (TC_KimlikNo,Ad,Soyad,GirisTarihi,Gorev_ID,Maas) VALUES (?,?,?,?,?,?)");
        query.addBindValue(ui->lineEdit_TC->text());
        query.addBindValue(ui->lineEdit_Ad->text());
        query.addBindValue(ui->lineEdit_Soyad->text());
        query.addBindValue(ui->dateEdit_isegiris->date().toString("yyyy/MM/dd"));
        query.addBindValue(gorevID);
        query.addBindValue(ui->doubleSpinBox_maas->value());
        if(query.exec())
        {
            QMessageBox::information(this,"başarılı","eklendi ");

        }
        else
            QMessageBox::critical(this,"başarısız",query.lastError().databaseText());
        on_pushButton_gosterpersonel_clicked();
    }
    else
        QMessageBox::critical(this,"başarısız",dbconn->db.lastError().driverText());
}

void MainWindow::on_pushButton_gosterpersonel_clicked()
{
    ui->tableView_personel->reset();
    percikis=false;
    QSqlQueryModel *model = new QSqlQueryModel;
    model->setQuery("SELECT TC_KimlikNo,Ad,Soyad,CONVERT(varchar, GirisTarihi, 103),Gorev,Gorev_ID,Maas FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID",dbconn->db);
    model->setHeaderData(0, Qt::Horizontal, tr("TC Kimlik No"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ad"));
    model->setHeaderData(2, Qt::Horizontal, tr("Soyad"));
    model->setHeaderData(3, Qt::Horizontal, tr("Giriş Tarihi"));
    model->setHeaderData(4, Qt::Horizontal, tr("Görevi"));
    ui->tableView_personel->setModel(model);
    ui->tableView_personel->hideColumn(5);
    ui->tableView_personel->update();
}

void MainWindow::on_tableView_personel_clicked(const QModelIndex &index)
{
    if(!percikis)
    {
        QModelIndexList indexes = ui->tableView_personel->selectionModel()->selection().indexes();
        ui->lineEdit_TC->setText(indexes.at(0).data().toString());
        ui->lineEdit_Ad->setText(indexes.at(1).data().toString());
        ui->lineEdit_Soyad->setText(indexes.at(2).data().toString());
        ui->dateEdit_isegiris->setDate(QDate::fromString(indexes.at(3).data().toString(),"dd/MM/yyyy"));
        ui->comboBox_gorev->setCurrentText(indexes.at(4).data().toString());
        ui->doubleSpinBox_maas->setValue(indexes.at(6).data().toDouble());
        TC=indexes.at(0).data().toString();
        QSqlQuery query(dbconn->db);
        query.prepare("SELECT COUNT(Plaka) FROM T_Arac WHERE Sürücü_TC=?");
        query.addBindValue(indexes.at(0).data().toString());
        query.exec();
        while (query.next()) {
            if(query.value(0)>0)
                ui->pushButton_aracgoster->show();
            else
                ui->pushButton_aracgoster->hide();
        }
    }
}

void MainWindow::on_pushButton_guncpersonel_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("UPDATE T_Personel SET TC_KimlikNo=?,Ad=?,Soyad=?,GirisTarihi=?,Gorev_ID=?,Maas=? WHERE TC_KimlikNo=?");
        query.addBindValue(ui->lineEdit_TC->text());
        query.addBindValue(ui->lineEdit_Ad->text());
        query.addBindValue(ui->lineEdit_Soyad->text());
        query.addBindValue(ui->dateEdit_isegiris->date().toString("yyyy/MM/dd"));
        query.addBindValue(idBul("SELECT ID FROM T_PerGorev WHERE Gorev=?",ui->comboBox_gorev->currentText()));
        query.addBindValue(ui->doubleSpinBox_maas->value());
        query.addBindValue(TC);
        if(query.exec())
        {
            QMessageBox::information(this,"başarılı","güncellendi");
            QSqlQuery q(dbconn->db);
            q.prepare("exec zam ?,?");
            q.addBindValue(ui->doubleSpinBox_maas->value());
            q.addBindValue(ui->lineEdit_TC->text());
            q.exec();
        }
        else
            QMessageBox::critical(this,"başarısız","güncellenemedi");
        on_pushButton_gosterpersonel_clicked();
    }
}

void MainWindow::on_pushButton_silpersonel_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("DELETE FROM T_Personel WHERE TC_KimlikNo=?");
        query.addBindValue(TC);
        if(query.exec())
            QMessageBox::information(this,"başarılı","silindi");
        else
            QMessageBox::critical(this,"başarısız","silinemedi");
        on_pushButton_gosterpersonel_clicked();
    }
}

void MainWindow::on_pushButton_ekleizin_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("INSERT INTO T_Izin (Personel_TC,IzinBas,IzinBit) VALUES (?,?,?) ");
        query.addBindValue(ui->comboBox_izintc->currentText());
        query.addBindValue(ui->dateEdit_izinbas->date().toString("yyyy/MM/dd"));
        query.addBindValue(ui->dateEdit_izinbitis->date().toString("yyyy/MM/dd"));

        if(query.exec())
        {
            QMessageBox::information(this,"başarılı","eklendi ");
        }
        else
            QMessageBox::critical(this,"başarısız",query.lastError().databaseText());
    }
}

void MainWindow::on_pushButton_gosterizin_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT TC_KimlikNo,Ad,Soyad,Gorev,CONVERT(varchar, IzinBas, 103),CONVERT(varchar, IzinBit, 103) FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID JOIN T_Izin ON T_Personel.TC_KimlikNo=T_Izin.Personel_TC",dbconn->db);
    model->setHeaderData(0, Qt::Horizontal, tr("TC Kimlik No"));
    model->setHeaderData(1, Qt::Horizontal, tr("Ad"));
    model->setHeaderData(2, Qt::Horizontal, tr("Soyad"));
    model->setHeaderData(3, Qt::Horizontal, tr("Görev"));
    model->setHeaderData(4, Qt::Horizontal, tr("İzin Başlangıç"));
    model->setHeaderData(5, Qt::Horizontal, tr("İzin Bitiş"));
    ui->tableView_izin->setModel(model);
}

void MainWindow::on_pushButton_guncizin_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("UPDATE T_Izin SET Personel_TC=?,IzinBas=?,IzinBit=? WHERE Personel_TC=?");
        query.addBindValue(ui->comboBox_izintc->currentText());
        query.addBindValue(ui->dateEdit_izinbas->date().toString("yyyy/MM/dd"));
        query.addBindValue(ui->dateEdit_izinbitis->date().toString("yyyy/MM/dd"));
        query.addBindValue(TCkimlik);

        if(query.exec())
            QMessageBox::information(this,"başarılı","güncellendi");
        else
            QMessageBox::critical(this,"başarısız","güncellenemedi");
        on_pushButton_gosterpersonel_clicked();
    }
}

void MainWindow::on_tableView_izin_clicked(const QModelIndex &index)
{
    QModelIndexList indexes = ui->tableView_izin->selectionModel()->selection().indexes();
    TCkimlik=indexes.at(0).data().toString();
    ui->comboBox_izintc->setCurrentText(indexes.at(0).data().toString());
    ui->comboBox_izinad->setCurrentText(indexes.at(1).data().toString());
    ui->comboBox_izinsoyad->setCurrentText(indexes.at(2).data().toString());
    ui->label_gorevizin->setText(indexes.at(3).data().toString());
    ui->dateEdit_izinbas->setDate(QDate::fromString(indexes.at(4).data().toString(),"dd/MM/yyyy"));
    ui->dateEdit_izinbitis->setDate(QDate::fromString(indexes.at(5).data().toString(),"dd/MM/yyyy"));
}

void MainWindow::on_pushButton_silizin_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("DELETE FROM T_Izin WHERE Personel_TC=?");
        query.addBindValue(TCkimlik);
        if(query.exec())
            QMessageBox::information(this,"başarılı","silindi");
        else
            QMessageBox::critical(this,"başarısız","silinemedi");
        on_pushButton_gosterizin_clicked();
    }
}

void MainWindow::on_pushButton_eklearac_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("INSERT INTO T_Arac (Marka,Model,Yıl,Plaka,Sürücü_TC) VALUES(?,?,?,?,?)");
        query.addBindValue(ui->comboBox_aracmarka->currentText());
        query.addBindValue(ui->comboBox_aracmodel->currentText());
        query.addBindValue(ui->comboBox_aracyil->currentText());
        query.addBindValue(ui->comboBox_aracplaka->currentText());
        query.addBindValue(ui->comboBox_aracsurucutc->currentText());
        if(query.exec())
            QMessageBox::information(this,"başarılı","eklendi");
        else
            QMessageBox::critical(this,"başarısız",query.lastError().text());
    }
    on_pushButton_gosterarac_clicked();
}

void MainWindow::on_pushButton_silarac_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("DELETE FROM T_Arac WHERE Plaka=?");
        query.addBindValue(TCkimlik);
        if(query.exec())
            QMessageBox::information(this,"başarılı","silindi");
        else
            QMessageBox::critical(this,"başarısız","silinemedi");
        on_pushButton_gosterarac_clicked();
    }
}

void MainWindow::on_tableView_arac_clicked(const QModelIndex &index)
{
    QModelIndexList indexes = ui->tableView_arac->selectionModel()->selection().indexes();
    ui->comboBox_aracmarka->setCurrentText(indexes.at(0).data().toString());
    ui->comboBox_aracmodel->setCurrentText(indexes.at(1).data().toString());
    ui->comboBox_aracyil->setCurrentText(indexes.at(2).data().toString());
    ui->comboBox_aracplaka->setCurrentText(indexes.at(3).data().toString());
    if(indexes.length()>4)
        ui->comboBox_aracsurucutc->setCurrentText(indexes.at(6).data().toString());
    else
        ui->comboBox_aracsurucutc->setCurrentText("");
}

void MainWindow::on_pushButton_gosterarac_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT Marka,Model,Yıl,Plaka,Ad,Soyad,TC_KimlikNo FROM T_Personel JOIN T_PerGorev ON T_Personel.Gorev_ID=T_PerGorev.ID JOIN T_Arac ON T_Personel.TC_KimlikNo=T_Arac.Sürücü_TC",dbconn->db);
    model->setHeaderData(4, Qt::Horizontal, tr("Sürücü Ad"));
    model->setHeaderData(5, Qt::Horizontal, tr("Sürücü Soyad"));
    ui->tableView_arac->setModel(model);
    ui->tableView_arac->hideColumn(6);
}

void MainWindow::on_pushButton_guncellearac_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("UPDATE T_Arac SET Marka=?,Model=?,Yıl=?,Plaka=?,Sürücü_TC=? WHERE Plaka=?");
        query.addBindValue(ui->comboBox_aracmarka->currentText());
        query.addBindValue(ui->comboBox_aracmodel->currentText());
        query.addBindValue(ui->comboBox_aracyil->currentText());
        query.addBindValue(ui->comboBox_aracplaka->currentText());
        query.addBindValue(ui->comboBox_aracsurucutc->currentText());
        query.addBindValue(ui->tableView_arac->selectionModel()->selectedIndexes().at(3).data().toString());

        if(query.exec())
            QMessageBox::information(this,"başarılı","güncellendi");
        else
            QMessageBox::critical(this,"başarısız","güncellenemedi");
        on_pushButton_gosterarac_clicked();
    }
}

void MainWindow::on_pushButton_ucretekle_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("INSERT INTO T_BirimUcret (Ucret,Gorev_ID) VALUES(?,?)");
        query.addBindValue(ui->doubleSpinBox_gorevucret->value());
        query.addBindValue(idBul("SELECT ID FROM T_PerGorev WHERE Gorev=?",ui->comboBox_gorevucret->currentText()));

        if(query.exec())
            QMessageBox::information(this,"başarılı","eklendi");
        else
            QMessageBox::critical(this,"başarısız",query.lastError().text());
    }
    on_pushButton_gosterucret_clicked();

}

void MainWindow::on_pushButton_ucretgunc_clicked()
{
    if(dbconn->db.isOpen())
    {
        QSqlQuery query(dbconn->db);
        query.prepare("UPDATE T_BirimUcret SET Ucret=?,Gorev_ID=? WHERE Ucret=? AND Gorev_ID=?");
        query.addBindValue(ui->doubleSpinBox_gorevucret->value());
        query.addBindValue(ui->comboBox_gorevucret->currentText());
        query.addBindValue(ui->tableView_ucret->selectionModel()->selectedIndexes().at(1).data().toString());
        query.addBindValue(ui->tableView_ucret->selectionModel()->selectedIndexes().at(0).data().toString());

        if(query.exec())
            QMessageBox::information(this,"başarılı","güncellendi");
        else
            QMessageBox::critical(this,"başarısız","güncellenemedi");
        on_pushButton_gosterucret_clicked();
    }
}


void MainWindow::on_pushButton_gosterucret_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT Gorev,Ucret FROM T_PerGorev JOIN T_BirimUcret ON T_BirimUcret.Gorev_ID=T_PerGorev.ID",dbconn->db);
    ui->tableView_ucret->setModel(model);
}

void MainWindow::on_comboBox_izintc_currentIndexChanged(int index)
{
    ui->comboBox_izinad->setCurrentIndex(index);
    ui->comboBox_izinsoyad->setCurrentIndex(index);
}

void MainWindow::on_comboBox_izinad_currentIndexChanged(int index)
{
    ui->comboBox_izintc->setCurrentIndex(index);
    ui->comboBox_izinsoyad->setCurrentIndex(index);
}

void MainWindow::on_comboBox_izinsoyad_currentIndexChanged(int index)
{
    ui->comboBox_izintc->setCurrentIndex(index);
    ui->comboBox_izinad->setCurrentIndex(index);
}

void MainWindow::on_pushButton_icdispersonel_clicked()
{
    ImportExport *d=new ImportExport();
    d->modelAl(ui->tableView_personel->model());
    d->exec();
    if(d->result()==1)
    {
        QSqlQuery q(dbconn->db);
        for(int i=0;i<d->modelVer()->rowCount();i++)
        {
            for(int j=0;j<d->modelVer()->columnCount();j++)
            {
                q.prepare("INSERT INTO T_PerGorev (Gorev) VALUES (?)");
                q.addBindValue(d->modelVer()->index(i,4).data().toString());
                q.exec();
            }
        }
        for(int i=0;i<d->modelVer()->rowCount();i++)
        {
            for(int j=0;j<d->modelVer()->columnCount();j++)
            {
                q.prepare("INSERT INTO T_Personel (TC_KimlikNo,Ad,Soyad,GirisTarihi,Gorev_ID) VALUES (?,?,?,CONVERT(date, ?, 103),?)");
                q.addBindValue(d->modelVer()->index(i,0).data().toString());
                q.addBindValue(d->modelVer()->index(i,1).data().toString());
                q.addBindValue(d->modelVer()->index(i,2).data().toString());
                q.addBindValue(d->modelVer()->index(i,3).data().toString());
                q.addBindValue(idBul("SELECT ID FROM T_PerGorev WHERE Gorev=?",d->modelVer()->index(i,4).data().toString()));
                q.exec();
            }
        }
    }
    on_pushButton_gosterpersonel_clicked();
}

void MainWindow::on_pushButton_icdisarac_clicked()
{
    ImportExport *d=new ImportExport();
    d->modelAl(ui->tableView_arac->model());
    d->exec();
    if(d->result()==1)
    {
        QSqlQuery q(dbconn->db);
        for(int i=0;i<d->modelVer()->rowCount();i++)
        {
            for(int j=0;j<d->modelVer()->columnCount();j++)
            {
                q.prepare("INSERT INTO T_Arac (Marka,Model,Yıl,Plaka,Sürücü_TC) VALUES(?,?,?,?,?)");
                q.addBindValue(d->modelVer()->index(i,0).data().toString());
                q.addBindValue(d->modelVer()->index(i,1).data().toString());
                q.addBindValue(d->modelVer()->index(i,2).data().toString());
                q.addBindValue(d->modelVer()->index(i,3).data().toString());
                q.addBindValue(d->modelVer()->index(i,6).data().toString());
                q.exec();
            }
        }
    }
    on_pushButton_gosterarac_clicked();
}

void MainWindow::on_tableView_ucret_clicked(const QModelIndex &index)
{
    QModelIndexList indexes = ui->tableView_ucret->selectionModel()->selection().indexes();
    ui->comboBox_gorevucret->setCurrentText(indexes.at(0).data().toString());
    ui->doubleSpinBox_gorevucret->setValue(indexes.at(1).data().toDouble());
}

void MainWindow::on_pushButton_aracgoster_clicked()
{
    filter=new FilterDialog();
    filter->arac();
    filter->surucuTC(TC);
    filter->on_buttonBox_accepted();
    filterArac();
    on_actionAracYonetimi_triggered();
}

void MainWindow::on_actionVeritabani_Yedekle_triggered()
{
    QSqlQuery q(dbconn->db);
    dbconn->db.close();
    isConnected();
    thread->quit();
    dbthreadBR();
    x:
    if(dbconn->db.isOpen())
    {
    QString filename=QFileDialog::getSaveFileName(this,tr("Veritabanı Yedek Al"), Settings::readSettings("ServerConfig","Database").toString(), tr("SQL Server Backup (*.bak)"));
    filename.replace(".bak",QDateTime::currentDateTime().toString("yyyyMMddhhmm")+".bak");
    q.prepare("BACKUP DATABASE ? TO DISK=?;");
    q.addBindValue(Settings::readSettings("ServerConfig","Database").toString());
    q.addBindValue(filename);  
    if(q.exec())
        QMessageBox::information(this,"başarılı","yedekleme başarılı");
    else
        QMessageBox::critical(this,"hata",q.lastError().driverText());
    dbconn->db.close();
    thread->quit();
    dbthread();
    }
    else {
        goto x;
    }


}

void MainWindow::on_actionVeritabani_Geri_Yukle_triggered()
{
    QSqlQuery q(dbconn->db);
    dbconn->db.close();
    isConnected();
    thread->quit();
    dbthreadBR();
    x:
    if(dbconn->db.isOpen())
    {
    QString filename=QFileDialog::getOpenFileName(this,tr("Veritabanı Yedek Al"), "", tr("SQL Server Backup (*.bak)"));
    q.prepare("RESTORE DATABASE ? FROM DISK=? WITH  FILE = 1,  NOUNLOAD,  STATS = 5;");
    q.addBindValue(Settings::readSettings("ServerConfig","Database").toString());
    q.addBindValue(filename);
    if(q.exec())
        QMessageBox::information(this,"başarılı","geri yükleme başarılı");
    else
        QMessageBox::critical(this,"hata",q.lastError().driverText());
    dbconn->db.close();
    thread->quit();
    dbthread();
    }
    else {
        goto x;
    }

}

void MainWindow::on_pushButton_gostermaas_clicked()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT TC_KimlikNo,Ad,Soyad,Gorev,FORMAT(Dönem,'yyyy/MM') as 'Dönem',T_Maas.Maas FROM T_PerGorev JOIN T_Personel ON T_Personel.Gorev_ID=T_PerGorev.ID JOIN T_Maas ON T_Maas.Personel_TC=T_Personel.TC_KimlikNo",dbconn->db);
    ui->tableView_maas->setModel(model);
}
