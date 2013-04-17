#include "selection.h"

Selection::Selection(QGraphicsScene *scene):
    QGraphicsRectItem(0, scene)
{
    init();
}

Selection::Selection(const QRectF &rect, QGraphicsScene *scene):
    QGraphicsRectItem(rect, 0, scene)
{
    init();
}

Selection::Selection(qreal x, qreal y, qreal width, qreal height, QGraphicsScene *scene):
    QGraphicsRectItem(x, y, width, height, 0, scene)
{
    init();
}

void Selection::init()
{
    // Always drawn on top of the scene.
    setZValue(INT_MAX);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::SquareCap);
    pen.setCosmetic(true);
    setPen(pen);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor::fromRgb(80, 200, 10, 60));
    setBrush(brush);
}