#ifndef IMPORTEXPORT_H
#define IMPORTEXPORT_H

#include <QDialog>
#include <QWidget>
#include <QAbstractItemModel>
#include <QFileDialog>
#include <QModelIndex>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QtPrintSupport>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "xlsxdocument.h"
#include "xlsxchartsheet.h"
#include "xlsxcellrange.h"
#include "xlsxchart.h"
#include "xlsxrichstring.h"
#include "xlsxworkbook.h"

namespace Ui {
class ImportExport;
}

class ImportExport : public QDialog
{
    Q_OBJECT

public:
    explicit ImportExport(QWidget *parent = nullptr);
    void modelAl(QAbstractItemModel *tablo);
    void printerPaint(QPrinter *printer);
    QModelIndex *index;
    QXlsx::AbstractSheet *sheet;
    QXlsx::Document *xlsxDoc;
    QString strStream;
    QTextDocument *document;
    int maxRow;
    int maxCol;
    QPrintPreviewDialog *printPreview;
    QPrinter printer;
    QAbstractItemModel *modelVer();
    QStringList headers;
    bool checkTable(QStringList hdr);

    ~ImportExport();

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_import_clicked();

    void on_pushButton_PDF_clicked();

    void printAll(QPrinter *printer);

    void on_buttonBox_clicked();

private:
    Ui::ImportExport *ui;
};

#endif // IMPORTEXPORT_H
