#ifndef PIXMAPITEM_HPP
#define PIXMAPITEM_HPP

#include <QGraphicsObject>

class PixmapItem : public QGraphicsObject
{
public:
    explicit PixmapItem(const QString &imagePath, QGraphicsItem *parent = 0);
    explicit PixmapItem(const QString &imagePath, QGraphicsScene *scene);

    PixmapItem(const PixmapItem& pix) : QGraphicsObject() { m_Pixmap = pix.m_Pixmap;}

    QSizeF size() const;
    void resizePixmap(int w, int h);
    QRectF boundingRect() const Q_DECL_OVERRIDE;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) Q_DECL_OVERRIDE;


private:
    QPixmap m_Pixmap;
};

#endif // PIXMAPITEM_HPP
