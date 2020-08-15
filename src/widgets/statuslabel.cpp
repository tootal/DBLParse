#include "statuslabel.h"
#include "ui_statuslabel.h"

#include "util.h"

StatusLabel::StatusLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatusLabel)
{
    ui->setupUi(this);
    QSize sz(iconSize, iconSize);
    okIcon = Util::svgToPixmap(":/resources/ok.svg", sz);
    noIcon = Util::svgToPixmap(":/resources/no.svg", sz);
    setNo();
}

StatusLabel::~StatusLabel()
{
    delete ui;
}

void StatusLabel::setOk()
{
    ui->iconLabel->setPixmap(okIcon);
}

void StatusLabel::setNo()
{
    ui->iconLabel->setPixmap(noIcon);
}

void StatusLabel::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
