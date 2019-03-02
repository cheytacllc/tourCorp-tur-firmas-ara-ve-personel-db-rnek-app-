#include "importexport.h"
#include "ui_importexport.h"
#include <QDebug>

ImportExport::ImportExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportExport)
{
    ui->setupUi(this);
    ui->pushButton_export->setDisabled(true);
    ui->pushButton_PDF->setDisabled(true);
}

void ImportExport::modelAl(QAbstractItemModel *tablo)
{

    ui->tableView->setModel(tablo);
    ui->tableWidget->hide();
    ui->buttonBox->hide();
    if(ui->tableView->rowAt(1)==NULL){
        ui->pushButton_PDF->setEnabled(true);
        ui->pushButton_export->setEnabled(true);
    }

}

QAbstractItemModel* ImportExport::modelVer()
{
    return ui->tableView->model();
}

bool ImportExport::checkTable(QStringList hdr)
{
    for(int i=0;i<hdr.length();i++)
    {
        if(ui->tableView->model()->headerData(i,Qt::Horizontal)!=hdr.at(i))
            return false;
    }
    return true;
}


ImportExport::~ImportExport()
{
    delete ui;
}

void ImportExport::on_pushButton_export_clicked()
{
    QXlsx::Document xlsxW;

    for(int i=0;i<=ui->tableView->model()->rowCount();i++)
    {
        for(int j=0;j<ui->tableView->model()->columnCount();j++)
        {
            if(i==0)
                xlsxW.write(i+1,j+1,ui->tableView->model()->headerData(j,Qt::Horizontal));
            else
                xlsxW.write(i+1,j+1,ui->tableView->model()->index(i-1,j).data().toString()); // write "Hello Qt!" to cell(A,1). it's shared string.
        }
    }


    QString filename=QFileDialog::getSaveFileName(this,tr("Excel çalışma sayfasını kaydet"), "", tr("Microsoft Excel Çalışma Sayfası (*.xlsx)"));
    xlsxW.saveAs(filename); // save the document as 'Test.xlsx'
}


void ImportExport::on_pushButton_import_clicked()
{
    //ui->tableView->setModel(NULL);
    QString filename=QFileDialog::getOpenFileName(this,tr("Excel çalışma sayfasını aç"), "", tr("Microsoft Excel Çalışma Sayfası (*.xlsx)"));

    QXlsx::Document xlsxTmp( filename );
    if ( !xlsxTmp.isLoadPackage() )
    {
        //return false; // failed to load
    }


    // load new xlsx using new document
    xlsxDoc = new QXlsx::Document( filename );
    xlsxDoc->isLoadPackage();

    sheet=xlsxDoc->currentSheet();
    QXlsx::Worksheet* wsheet = xlsxDoc->currentWorksheet(); //(QXlsx::Worksheet*) xlsxDoc->sheet("Sheet 1");
    qDebug()<<xlsxDoc->sheetNames();
    if ( NULL != wsheet )
    {
        //return false;

        // get full cells of sheet
        int maxRow = -1;
        int maxCol = -1;
        QVector<QXlsx::CellLocation> clList = wsheet->getFullCells( &maxRow, &maxCol );
        //QTableWidget *table=new QTableWidget(maxRow,maxCol);
        //table->setParent(ui->gridLayout);
        ui->tableWidget->setColumnCount(maxCol); //->setColumnCount(maxCol);
        ui->tableWidget->setRowCount(maxRow);
        // set max count of row,col
        // NOTE: This part should be modified later.
        //  The maximum value of the sheet should be set to an appropriate value.



        for ( int ic = 0; ic < clList.size(); ++ic )
        {
            // cell location
            QXlsx::CellLocation cl = clList.at(ic);

            ////////////////////////////////////////////////////////////////////
            // First cell of tableWidget is 0.
            // But first cell of Qxlsx document is 1.
            int row = cl.row - 1;
            int col = cl.col - 1;

            ////////////////////////////////////////////////////////////////////
            // cell pointer
            QSharedPointer<QXlsx::Cell> ptrCell = cl.cell;

            ////////////////////////////////////////////////////////////////////
            // create new item of table widget
            QTableWidgetItem* newItem = new QTableWidgetItem();

            ///////////////////////////////////////////////////////////////////
            // value of cell
            QVariant var = cl.cell.data()->value();
            QString str = var.toString();

            ////////////////////////////////////////////////////////////////////
            // set text
            //model->setData(model->index(row,col),str);
            newItem->setText( str );

            ////////////////////////////////////////////////////////////////////
            // set item
            if(row!=0)
                ui->tableWidget->setItem( row-1, col, newItem );
            else
                ui->tableWidget->setHorizontalHeaderItem(col,newItem);

        }

    }
    ui->tableView->setModel(NULL);
    ui->tableView->setModel(ui->tableWidget->model());
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->buttonBox->show();
}

void ImportExport::on_pushButton_PDF_clicked()
{
    printer.setPaperSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Portrait);
    printer.setFullPage(true);

    printPreview = new QPrintPreviewDialog(&printer);
    connect(printPreview, SIGNAL(paintRequested(QPrinter*)), SLOT(printAll(QPrinter*)));

    printPreview->setWindowTitle("Baskı Önizleme");
    //Qt::WindowFlags flags(Qt::WindowTitleHint);
    //printPreview->setWindowFlags(flags);
    printPreview->setWindowState(Qt::WindowMaximized);
    printPreview->exec();

}

void ImportExport::printAll(QPrinter *printer)
{
    QTextStream out(&strStream);

    const int rowCount = ui->tableView->model()->rowCount();
    const int columnCount = ui->tableView->model()->columnCount();

    out <<  "<html>\n"
            "<head>\n"
            "<meta Content=\"Text/html; charset=Windows-1251\">\n"
         <<  QString("<title>%1</title>\n").arg("strTitle")
          <<  "</head>\n"
              "<body bgcolor=#ffffff link=#5000A0>\n"
              "<table border=1 cellspacing=0 cellpadding=2>\n";

    // headers

    out << QString("<h1>%1</h1>&nbsp;").arg("Rapor");
    out << QString("<h3>%1</h3>&nbsp;").arg(QDateTime::currentDateTime().toString());
    out << "<thead><tr bgcolor=#f0f0f0>";
    for (int column = 0; column < columnCount; column++)
        if (!ui->tableView->isColumnHidden(column))
            out << QString("<th>%1</th>").arg(ui->tableView->model()->headerData(column, Qt::Horizontal).toString());
    out << "</tr></thead>\n";

    // data table
    for (int row = 0; row < rowCount; row++) {
        out << "<tr>";
        for (int column = 0; column < columnCount; column++) {
            if (!ui->tableView->isColumnHidden(column)) {
                QString data = ui->tableView->model()->data(ui->tableView->model()->index(row, column)).toString().simplified();
                out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
            }
        }
        out << "</tr>\n";
    }
    out <<  "</table>\n"
            "</body>\n"
            "</html>\n";

    document = new QTextDocument();
    document->setHtml(strStream);

    document->print(printer);
    //    QPrinter printer;

    //    QPrintPreviewDialog ppd;

    //    QPrintDialog *dialog = new QPrintDialog(&printer, NULL);
    //    if (dialog->exec() == QDialog::Accepted) {
    //        document->print(&printer);
    //    }

    delete document;
}

void ImportExport::on_buttonBox_clicked()
{
    done(1);
}
