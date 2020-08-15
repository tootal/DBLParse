#include "iconlabel.h"

#include <QPainter>

IconLabel::IconLabel(QWidget *parent, QIcon icon, QSize size)
    : QWidget(parent), m_size(size), m_icon(icon)
{
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

QSize IconLabel::sizeHint() const
{
    return m_size;
}

void IconLabel::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QRect rect = contentsRect();
    int width = rect.width();
    int height = rect.height();
    if(width < height) {
        rect.setHeight(width);
        rect.translate(0, (height - width) / 2);
    } else if (width > height) {
        rect.setWidth(height);
        rect.translate((width - height) / 2, 0);
    }
    m_icon.paint(&p, rect);
}

void IconLabel::setIcon(QIcon icon)
{
    m_icon = icon;
    update();
}
