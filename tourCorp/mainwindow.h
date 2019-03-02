#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serverdialog.h"
#include "settings.h"
#include <QPushButton>
#include <QCommandLinkButton>
#include <QStyleFactory>
#include <QToolBar>
#include <QTabBar>
#include <QTime>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QAction>
#include <QComboBox>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlResult>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QItemSelectionModel>
#include <QThread>
#include <QSqlField>
#include "dbconnection.h"
#include "filterdialog.h"
#include "importexport.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
//    QToolBar *toolbar_personel;
//    QToolBar *toolbar_muhasebe;
    explicit MainWindow(QWidget *parent = nullptr);
    void setActionCheckStates(QToolBar *toolbar, QAction *action);
    int currentTabIndex=-1, yetkiID_tableView=0, yetkiID_comboBox=0;
    QString kullaniciAdi, loginUser, yetkiStr, TC, TCkimlik;
    QAction *tempAction;
    QToolBar *tempToolBar;
    QCheckBox *ck;
    QLabel *icon, *status, *user, *yetki;
    bool percikis=false;
    QListWidgetItem *item;
    QList <QAction*> actionList;
    QList <QPushButton*> buttonListAdmin;
    QList <QPushButton*> buttonListPersonel;
    QList <QPushButton*> buttonListArac;
    QList <QPushButton*> buttonListIzin;
    QList <QPushButton*> buttonListUcret;
    DbConnection *dbconn;
    FilterDialog *filter;
    QThread *thread;
    void checkPermissions(int id);
    void hideAdmin();
    int idBul(QString query, QString data);
    int loginYetkiID, gorev_ID, markaID, modelID, yilID;
    ~MainWindow();



public slots:
    void on_actioncikis_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_actionKullaniciEkle_triggered();
    void on_actionPersonelYonetimi_triggered();
    void on_actionAracYonetimi_triggered();
    bool isConnected();
    void connectDb();
    void connectDbBR();
    void dbthread();
    void filterPersonel();
    void filterIzin();
    void filterArac();
    void filterMaas();
    void on_action_calismaSaatleri_triggered();

    void on_actionIzinler_triggered();

    void on_actionBirimUcret_triggered();

    void on_pushButton_yetkiekle_clicked();

    void on_actionMaaslar_triggered();

    void on_pushButton_filtrepersonel_clicked();
    void disconnectDb();

    void dbthreadBR();
private slots:
    void on_pushButton_filtreizin_clicked();

    void on_pushButton_filtrearac_clicked();

    void on_pushButton_filtremaas_clicked();

    void on_pushButton_kullgoster_clicked();

    void on_tableView_admin_clicked(const QModelIndex &index);

    void on_pushButton_yetkigoster_clicked();

    void on_pushButton_ekle_clicked();

    void on_pushButton_sil_clicked();

    void on_pushButton_yetkiguncelle_clicked();

    void on_pushButton_guncelle_clicked();

    void on_comboBox_yetki_currentTextChanged(const QString &arg1);

    void on_lineEdit_password_textEdited(const QString &arg1);

    void on_lineEdit_password_textChanged(const QString &arg1);

    void on_pushButton_pwd_pressed();

    void on_pushButton_pwd_released();

    void on_pushButton_eklepersonel_clicked();

    void on_pushButton_gosterpersonel_clicked();

    void on_tableView_personel_clicked(const QModelIndex &index);

    void on_pushButton_guncpersonel_clicked();

    void on_pushButton_silpersonel_clicked();

    void on_pushButton_ekleizin_clicked();

    void on_pushButton_gosterizin_clicked();

    void on_pushButton_guncizin_clicked();

    void on_tableView_izin_clicked(const QModelIndex &index);

    void on_pushButton_silizin_clicked();

    void on_pushButton_eklearac_clicked();

    void on_pushButton_silarac_clicked();

    void on_tableView_arac_clicked(const QModelIndex &index);

    void on_pushButton_gosterarac_clicked();

    void on_pushButton_ucretekle_clicked();

    void on_pushButton_ucretgunc_clicked();

    void on_pushButton_guncellearac_clicked();

    void on_pushButton_gosterucret_clicked();

    void on_comboBox_izintc_currentIndexChanged(int index);

    void on_comboBox_izinad_currentIndexChanged(int index);

    void on_comboBox_izinsoyad_currentIndexChanged(int index);

    void on_pushButton_icdispersonel_clicked();

    void on_pushButton_icdisarac_clicked();

    void on_tableView_ucret_clicked(const QModelIndex &index);

    void on_pushButton_aracgoster_clicked();

    void on_actionVeritabani_Yedekle_triggered();

    void on_actionVeritabani_Geri_Yukle_triggered();

    void on_pushButton_gostermaas_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
