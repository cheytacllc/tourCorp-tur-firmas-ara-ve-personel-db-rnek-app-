#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    ui->dateEdit_tarih->setDate(QDate::currentDate());
    ui->dateEdit_tarih_2->setDate(QDate::currentDate());
    ui->dateEdit_tarih_3->setDate(QDate::currentDate());
    ui->dateEdit_tarih_4->setDate(QDate::currentDate());
}

void FilterDialog::personel()
{
    ui->dateEdit_tarih_2->hide();
    ui->dateEdit_tarih_3->hide();
    ui->dateEdit_tarih_4->hide();
    ui->groupBox_2->hide();
    ui->label_aracmarka->hide();
    ui->label_aracmodel->hide();
    ui->label_aracplaka->hide();
    ui->label_aracyil->hide();
    ui->lineEdit_aracmarka->hide();
    ui->lineEdit_aracmodel->hide();
    ui->lineEdit_aracplaka->hide();
    ui->lineEdit_aracyil->hide();
    ui->pushButton->hide();
    ui->checkBox_surucu->hide();
    //ui->doubleSpinBox->hide();
    //ui->label_maas->hide();
    tarih1=" GirisTarihi";
    tarih2=" GirisTarihi";
    maas="Maas";
}

void FilterDialog::izinler()
{
    ui->dateEdit_tarih_2->hide();
    ui->dateEdit_tarih_4->hide();
    ui->groupBox->setTitle("İzin Başlangıcı");
    ui->groupBox_2->setTitle("İzin Bitişi");
    ui->label_aracmarka->hide();
    ui->label_aracmodel->hide();
    ui->label_aracplaka->hide();
    ui->label_aracyil->hide();
    ui->lineEdit_aracmarka->hide();
    ui->lineEdit_aracmodel->hide();
    ui->lineEdit_aracplaka->hide();
    ui->lineEdit_aracyil->hide();
    ui->pushButton->hide();
    ui->checkBox_surucu->hide();
    ui->checkBox_percikis->hide();
    ui->doubleSpinBox->hide();
    ui->label_maas->hide();
    ui->pushButton_2->hide();
    tarih1=" IzinBas";
    tarih2=" IzinBas";
    tarih3=" IzinBit";
    tarih4=" IzinBit";

}

void FilterDialog::arac()
{
    ui->label_Gorev->hide();
    ui->lineEdit_gorev->hide();
    ui->dateEdit_tarih->hide();
    ui->dateEdit_tarih_2->hide();
    ui->dateEdit_tarih_3->hide();
    ui->dateEdit_tarih_4->hide();
    ui->groupBox->hide();
    ui->groupBox_2->hide();
    ui->checkBox_percikis->hide();
    ui->doubleSpinBox->hide();
    ui->label_maas->hide();
    ui->pushButton_2->hide();
}

void FilterDialog::maaslar()
{
    ui->dateEdit_tarih_2->hide();
    ui->label_aracmarka->hide();
    ui->label_aracmodel->hide();
    ui->label_aracplaka->hide();
    ui->label_aracyil->hide();
    ui->lineEdit_aracmarka->hide();
    ui->lineEdit_aracmodel->hide();
    ui->lineEdit_aracplaka->hide();
    ui->lineEdit_aracyil->hide();
    ui->dateEdit_tarih_3->hide();
    ui->dateEdit_tarih_4->hide();
    ui->checkBox_surucu->hide();
    ui->groupBox_2->hide();
    ui->pushButton->hide();
    ui->checkBox_percikis->hide();
    tarih1=" Dönem";
    tarih2=" Dönem";
    maas="T_Maas.Maas";
}

void FilterDialog::surucuTC(QString TC)
{
    ui->lineEdit_TC->setText(TC);
}

bool FilterDialog::chkSurucu()
{
    return ui->checkBox_surucu->isChecked();
}

bool FilterDialog::chkPersonel()
{
    return ui->checkBox_percikis->isChecked();
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox->currentText().contains("tarihleri arası"))
        ui->dateEdit_tarih_2->show();
    else
        ui->dateEdit_tarih_2->hide();
}


void FilterDialog::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    if(ui->comboBox_2->currentText().contains("tarihleri arası"))
        ui->dateEdit_tarih_4->show();
    else
        ui->dateEdit_tarih_4->hide();
}

//tarih filtresi
void FilterDialog::on_comboBox_currentIndexChanged(int index)
{

}

void FilterDialog::on_lineEdit_TC_textChanged(const QString &arg1)
{

}

void FilterDialog::on_buttonBox_accepted()
{
    if(!ui->lineEdit_TC->text().isEmpty())
        tc="TC='"+ui->lineEdit_TC->text()+"' AND ";
    else
        tc="";

    if(!ui->lineEdit_Ad->text().isEmpty())
        ad="Ad='"+ui->lineEdit_Ad->text()+"' AND ";
    else
        ad="";

    if(!ui->lineEdit_Soyad->text().isEmpty())
        soyad="Soyad='"+ui->lineEdit_Soyad->text()+"' AND ";
    else
        soyad="";

    if(!ui->lineEdit_gorev->text().isEmpty())
        gorev="Gorev='"+ui->lineEdit_gorev->text()+"' AND ";
    else
        gorev="";

    if(!ui->lineEdit_aracmarka->text().isEmpty())
        marka="Marka='"+ui->lineEdit_aracmarka->text()+"' AND ";
    else
        marka="";

    if(!ui->lineEdit_aracmodel->text().isEmpty())
        model="Model='"+ui->lineEdit_aracmodel->text()+"' AND ";
    else
        model="";

    if(!ui->lineEdit_aracplaka->text().isEmpty())
        plaka="Plaka='"+ui->lineEdit_aracplaka->text()+"' AND ";
    else
        plaka="";

    if(!ui->lineEdit_aracyil->text().isEmpty())
        yil="Yıl"+ui->pushButton->text()+"'"+ui->lineEdit_aracyil->text()+"' AND ";
    else
        yil="";
    if(ui->doubleSpinBox->value()!=0)
        maas+=ui->pushButton_2->text()+"'"+QString::number(ui->doubleSpinBox->value())+"' AND ";
    else
        maas="";

    switch (ui->comboBox->currentIndex()) {
    case 0:
    {
       tarih1="";
       tarih2="";
        break;
    }
    case 1:
    {
        tarih1+="='"+ui->dateEdit_tarih->date().toString("yyyy/MM/dd")+"' AND ";
        tarih2="";
        break;
    }
    case 2:
    {
        tarih1+="<'"+ui->dateEdit_tarih->date().toString("yyyy/MM/dd")+"' AND ";
        tarih2="";
        break;
    }
    case 3:
    {
        tarih1+=">'"+ui->dateEdit_tarih->date().toString("yyyy/MM/dd")+"' AND ";
        tarih2="";
        break;
    }
    case 4:
    {
        //"DATEDIFF(day,"+ui->dateEdit_tarih->date().toString("yyyy/MM/dd")+","+ui->dateEdit_tarih_2->date().toString("yyyy/MM/dd")+")>0";
        tarih1+=">'"+ui->dateEdit_tarih->date().toString("yyyy/MM/dd")+"' AND ";
        tarih2+="<'"+ui->dateEdit_tarih_2->date().toString("yyyy/MM/dd")+"' AND ";
        //tarih2.remove(tarih2.length()-5,5);
        break;
    }
    default:
        break;
    }

    switch (ui->comboBox_2->currentIndex()) {
    case 0:
    {
        tarih3="";
        tarih4="";
        break;
    }
    case 1:
    {
        tarih3+="='"+ui->dateEdit_tarih_3->date().toString("yyyy/MM/dd")+"' AND ";
        tarih4="";
        break;
    }
    case 2:
    {
        tarih3+="<'"+ui->dateEdit_tarih_3->date().toString("yyyy/MM/dd")+"' AND ";
        tarih4="";
        break;
    }
    case 3:
    {
        tarih3+=">'"+ui->dateEdit_tarih_3->date().toString("yyyy/MM/dd")+"' AND ";
        tarih4="";
        break;
    }
    case 4:
    {
        //"DATEDIFF(day,"+ui->dateEdit_tarih->date().toString("yyyy/MM/dd")+","+ui->dateEdit_tarih_2->date().toString("yyyy/MM/dd")+")>0";
        tarih3+=">'"+ui->dateEdit_tarih_3->date().toString("yyyy/MM/dd")+"' AND ";
        tarih4+="<'"+ui->dateEdit_tarih_4->date().toString("yyyy/MM/dd")+"' AND ";
        //tarih2.remove(tarih2.length()-5,5);
        break;
    }
    default:
       break;
    }
    where=tc+ad+soyad+gorev+plaka+model+marka+yil+maas+tarih1+tarih2+tarih3+tarih4;
    where.remove(where.length()-5,5);
    cksurucu=ui->checkBox_surucu->isChecked();
    ckpersonel=ui->checkBox_percikis->isChecked();
}

void FilterDialog::on_pushButton_clicked()
{
    if(ui->pushButton->text()=="=")
        ui->pushButton->setText("<");
    else if(ui->pushButton->text()==">")
        ui->pushButton->setText("=");
    else
        ui->pushButton->setText(">");
}

void FilterDialog::on_checkBox_percikis_toggled(bool checked)
{
    if(checked)
    {
        ui->groupBox_2->show();
        //ui->dateEdit_tarih_2->show();
        ui->dateEdit_tarih_3->show();
        //ui->dateEdit_tarih_4->show();
        ui->groupBox->setTitle("Giriş Tarihi");
        ui->groupBox_2->setTitle("Çıkış tarihi");
        tarih1=" GirisTarihi";
        tarih2=" GirisTarihi";
        tarih3=" CikisTarihi";
        tarih4=" CikisTarihi";
    }
    else {
        ui->groupBox_2->hide();
        ui->dateEdit_tarih_2->hide();
        ui->dateEdit_tarih_3->hide();
        ui->dateEdit_tarih_4->hide();
    }
}

void FilterDialog::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text()=="=")
        ui->pushButton_2->setText("<");
    else if(ui->pushButton_2->text()==">")
        ui->pushButton_2->setText("=");
    else
        ui->pushButton_2->setText(">");
}
