#include "statuslabel.h"

#include <QPainter>
#include <QSvgWidget>
#include <QHBoxLayout>
#include <QLabel>

#include "util.h"

StatusLabel::StatusLabel(QWidget *parent)
    : QWidget(parent)
{
    setCursor(Qt::PointingHandCursor);
    okIcon = Util::svgToPixmap(":/resources/ok.svg", QSize(16, 16));
    noIcon = Util::svgToPixmap(":/resources/no.svg", QSize(16, 16));
    QHBoxLayout *layout = new QHBoxLayout(this);
    auto iconLabel = new QLabel;
    iconLabel->setPixmap(okIcon);
    layout->addWidget(iconLabel);
    auto textLabel = new QLabel("Parse");
    layout->addWidget(textLabel);
}

void StatusLabel::mousePressEvent(QMouseEvent *)
{
    emit clicked();
}
