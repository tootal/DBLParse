#include "annualhotspotdialog.h"
#include "ui_annualhotspotdialog.h"

#include "parser.h"

AnnualHotspotDialog::AnnualHotspotDialog(const YearWord &yearWords, QWidget *parent)
    : QDialog(parent),
      ui(new Ui::AnnualHotspotDialog)
{
    ui->setupUi(this);
    ui->tableWidget->setRowCount(yearWords.size());
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({
        tr("Year"),
        tr("Hotspots")
    });
    int row = 0;
    for (int year : yearWords.keys()) {
        setItem(row, 0, year);
        auto wordsCount = yearWords[year];
        QStringList texts;
        for (int i = wordsCount.size() > 10
                    ? wordsCount.size() - 10
                    : 0;
             i < wordsCount.size();
             i++) {
            texts.append(QString("%1 (%2)")
                         .arg(QString(wordsCount[i].word))
                         .arg(wordsCount[i].count));
        }
        setItem(row, 1, texts.join("; "));
        row++;
    }
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

AnnualHotspotDialog::~AnnualHotspotDialog()
{
    delete ui;
}

void AnnualHotspotDialog::setItem(int row, int col, const QString &text)
{
    ui->tableWidget->setItem(row, col, new QTableWidgetItem(text));
}

void AnnualHotspotDialog::setItem(int row, int col, int text)
{
    setItem(row, col, QString::number(text));
}
