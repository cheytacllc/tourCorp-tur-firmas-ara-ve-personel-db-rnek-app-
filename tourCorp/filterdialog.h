#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(QWidget *parent = nullptr);
    void personel();
    void izinler();
    void arac();
    void maaslar();
    void surucuTC(QString TC);
    QString tarih1, tarih2, tarih3, tarih4, where;
    QString tc, ad, soyad, gorev, marka, model, yil, plaka, maas;
    bool cksurucu=false, ckpersonel=false;
    bool chkSurucu();
    bool chkPersonel();
    ~FilterDialog();

public slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_lineEdit_TC_textChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void on_pushButton_clicked();

private slots:


    void on_checkBox_percikis_toggled(bool checked);

    void on_pushButton_2_clicked();

private:
    Ui::FilterDialog *ui;
};

#endif // FILTERDIALOG_H
