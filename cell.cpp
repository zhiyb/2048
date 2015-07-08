#include <QPainter>
#include <QGraphicsScene>
#include <QFont>
#include "cell.h"
#include "tile.h"
#include "grid.h"

Cell::Cell(int x, int y, int v, QGraphicsItem *parent, Qt::WindowFlags wFlags) :
	QGraphicsWidget(parent, wFlags)
{
	setX(x);
	setY(y);
	setValue(v);
	setMerged(false);
	setCacheMode(DeviceCoordinateCache);
	refresh();
}

QRectF Cell::boundingRect(void) const
{
	QRectF rCon(FRAME, FRAME, scene()->width() - FRAME * 2, scene()->height() - FRAME * 2);
	QRectF rCell(0, 0, (rCon.width() - FRAME * 3) / 4, (rCon.width() - FRAME * 3) / 4);
	return rCell;
}

void Cell::setValue(const int v)
{
	data.v = v;
	setZValue(abs(v));
}

void Cell::refresh(void)
{
	data.vc = data.v;
	update(boundingRect());
}

void Cell::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	QRectF rCell = boundingRect();
	painter->setPen(Tile::background(data.vc));
	painter->setBrush(Tile::background(data.vc));
	painter->drawRoundRect(rCell, 3, 3);
	if (data.vc == 0)
		return;
	painter->setFont(Tile::font(data.vc));
	painter->setPen(Tile::foreground(data.vc));
	painter->drawText(rCell, Qt::AlignCenter, QString::number(data.vc));
#if 0
	QImage image(QString(":/image/%1.jpg").arg(abs(data.vc)));
	if (data.vc < 0)
		image.invertPixels();
	if (!image.isNull())
		painter->drawImage(rCell, image, image.rect());
#endif
}
