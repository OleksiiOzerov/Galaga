#include "PixmapItem.hpp"

#include <QPainter>

#include <QGraphicsScene>

PixmapItem::PixmapItem(const QString &imagePath, QGraphicsItem *parent) :
    QGraphicsObject(parent)
{
    m_Pixmap = QPixmap(imagePath);
}

PixmapItem::PixmapItem(const QString &imagePath, QGraphicsScene *scene) :
    QGraphicsObject(), m_Pixmap(imagePath)
{
    scene->addItem(this);
}

QSizeF PixmapItem::size() const
{
    return m_Pixmap.size();
}

void PixmapItem::resizePixmap(int w, int h)
{
    m_Pixmap = m_Pixmap.scaled(w, h);
}

QRectF PixmapItem::boundingRect() const
{
    return QRectF(QPointF(0, 0), m_Pixmap.size());
}

void PixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->drawPixmap(0, 0, m_Pixmap);
}
