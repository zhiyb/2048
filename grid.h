#ifndef GRID_H
#define GRID_H

#include <QGraphicsWidget>
#include <QtWidgets>
#include "cell.h"
#include "animation.h"

#define FRAME	15

class Grid : public QGraphicsWidget
{
	Q_OBJECT
public:
	explicit Grid(QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
	void init(void);
	void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget * = 0);
	QRectF boundingRect(void) const;
	QPainterPath shape(void) const;

signals:
	void scoreChanged(int);

public slots:
	void save(QString path);
	void load(QString path);
	void reinit(void);

protected:
	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *e);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

private:
	struct vect {
		int x, y;
	};

	void op(const vect& dir);
	bool move(const vect& dir);
	void move(class Cell **from, class Cell **to);
	class Cell **find(class Cell **orig, const vect dir);
	bool generate(void);
	bool failed(void);

	bool succeed;
	int score;
	QPointF mouse;
	Cell *cells[4][4];
	Animation *ani;
};

#endif // GRID_H
