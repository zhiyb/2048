#ifndef ANIMATION_H
#define ANIMATION_H

#include <QObject>
#include <QParallelAnimationGroup>
#include <QGraphicsWidget>
#include <QList>
#include "cell.h"

class Animation : public QObject
{
	Q_OBJECT
public:
	explicit Animation(const int dur, QObject *parent = 0);
	/*void changeInterval(const int msec) {interval = msec;}
	int find(QWidget *w) {return widgets.find(w);}*/
	void add(QGraphicsWidget *w, QPointF to);
	void addNew(QGraphicsWidget *w);
	void addDelete(QGraphicsWidget *w);
	void addRefresh(Cell *w);
	void remove(QGraphicsWidget *w);

signals:

public slots:
	void start(void);
	void stop(void);

private:
	void clear(void);

	int duration;
	QParallelAnimationGroup *group;
	QList<QGraphicsWidget *> news, deletes;
	QList<Cell *> refresh;
};

#endif // ANIMATION_H
