#include "loadinglabel.h"
#include "ui_loadinglabel.h"

#include <QPainter>
#include <QTimer>

#include "util.h"

LoadingLabel::LoadingLabel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingLabel)
{
    ui->setupUi(this);
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, 
            this, QOverload<>::of(&LoadingLabel::repaint));
    timer->start(17);
}

LoadingLabel::~LoadingLabel()
{
    delete ui;
}

void LoadingLabel::paintEvent(QPaintEvent *)
{
    static int deg = 0;
    deg += 2;
    QPainter painter(this);
    painter.translate(this->width() / 2,
                      this->height() / 2);
    painter.rotate(deg);
    auto pix = Util::svgToPixmap(":/resources/loading.svg", QSize(64, 64));
    painter.drawPixmap(-pix.height() / 2, -pix.width() / 2, pix);
    if (deg >= 360) deg = 0;
}
