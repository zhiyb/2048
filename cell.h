#ifndef CELL_H
#define CELL_H

#include <QGraphicsWidget>

class Cell : public QGraphicsWidget
{
	Q_OBJECT
public:
	explicit Cell(int x, int y, int v, QGraphicsItem * parent = 0, Qt::WindowFlags wFlags = 0);
	~Cell();
	void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget * = 0);
	QRectF boundingRect(void) const;
	int x(void) const {return data.x;}
	void setX(const int x) {data.x = x;}
	int y(void) const {return data.y;}
	void setY(const int y) {data.y = y;}
	int value(void) const {return data.v;}
	void setValue(const int v);
	bool merged(void) const {return data.merged;}
	void setMerged(bool merged) {data.merged = merged;}
	void refresh(void);

private:
	struct {
		bool merged;
		int x, y, v, vc;
	} data;
};

#endif // CELL_H
