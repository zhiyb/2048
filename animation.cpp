#include <QPropertyAnimation>
#include "animation.h"

Animation::Animation(const int dur, QObject *parent) :
	QObject(parent), duration(dur)
{
	group = new QParallelAnimationGroup(this);
	connect(group, SIGNAL(finished()), this, SLOT(stop()));
}

void Animation::start(void)
{
	group->start();
}

void Animation::stop(void)
{
	if (group->state() == group->Stopped && refresh.count() != 0) {
		group->clear();
		for (int i = 0; i < refresh.count(); i++) {
			Cell *w = refresh.at(i);
			w->refresh();
			QPropertyAnimation *anim = new QPropertyAnimation(w, "scale");
			anim->setDuration(duration);
			anim->setEasingCurve(QEasingCurve::Linear);
			anim->setStartValue(w->scale());
			anim->setKeyValueAt(0.5, 1.2);
			anim->setEndValue(w->scale());
			group->addAnimation(anim);
			anim = new QPropertyAnimation(w, "pos");
			anim->setDuration(duration);
			anim->setEasingCurve(QEasingCurve::Linear);
			anim->setStartValue(w->pos());
			qreal d = w->boundingRect().width();
			anim->setKeyValueAt(0.5, w->pos() - QPoint(d, d) * 0.1);
			anim->setEndValue(w->pos());
			group->addAnimation(anim);
		}
		group->start();
	} else {
		if (group->state() != group->Stopped) {
			group->setCurrentTime(group->totalDuration());
			group->stop();
		}
		group->clear();
		for (int i = 0; i < refresh.count(); i++)
			refresh.at(i)->refresh();
	}
	for (int i = 0; i < news.count(); i++)
		news.at(i)->show();
	qDeleteAll(deletes);
	clear();
}

void Animation::add(QGraphicsWidget *w, QPointF to)
{
	QPropertyAnimation *anim = new QPropertyAnimation(w, "pos");
	anim->setDuration(duration);
	anim->setEasingCurve(QEasingCurve::OutQuad);
	anim->setStartValue(w->pos());
	anim->setEndValue(to);
	group->addAnimation(anim);
}

void Animation::addNew(QGraphicsWidget *w)
{
	if (group->state() != group->Running)
		return;
	if (deletes.indexOf(w) != -1)
		return;
	w->hide();
	news.append(w);
}

void Animation::addDelete(QGraphicsWidget *w)
{
	if (deletes.indexOf(w) == -1)
		deletes.append(w);
}

void Animation::addRefresh(Cell *w)
{
	if (refresh.indexOf(w) == -1)
		refresh.append(w);
}

void Animation::remove(QGraphicsWidget *w)
{
	for (int i = 0; i < group->animationCount(); i++)
		if (((QPropertyAnimation *)group->animationAt(i))->targetObject() == w)
			group->removeAnimation(group->animationAt(i));
}

void Animation::clear(void)
{
	news.clear();
	deletes.clear();
	refresh.clear();
}
