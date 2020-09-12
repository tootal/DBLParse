#pragma once

#include <QDialog>

#include "parser.h"

namespace Ui {
class AnnualHotspotDialog;
}

class AnnualHotspotDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AnnualHotspotDialog(const YearWord &yearWords, QWidget *parent = nullptr);
    ~AnnualHotspotDialog();
    
private:
    Ui::AnnualHotspotDialog *ui;
    void setItem(int row, int col, const QString &text);
    void setItem(int row, int col, int text);
};

