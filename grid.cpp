#include <QPainter>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QtWidgets>
#include <QPropertyAnimation>
#include <QFile>
#include <QDataStream>
#include <QTextStream>
#include <ctime>
#include "grid.h"
#include "tile.h"

Grid::Grid(QGraphicsItem *parent, Qt::WindowFlags wFlags) :
	QGraphicsWidget(parent, wFlags)
{
	qsrand(time(NULL));
	ani = new Animation(150, this);
	setZValue(-1);
	setFlag(ItemIsFocusable, true);
	setFocusPolicy(Qt::StrongFocus);
	setAcceptedMouseButtons(Qt::AllButtons);
	setCacheMode(DeviceCoordinateCache);
	connect(qApp, SIGNAL(aboutToQuit()), ani, SLOT(stop()));
}

void Grid::init(void)
{
	score = 0;
	succeed = false;
	emit scoreChanged(score);
	QRectF rCell = boundingRect().adjusted(FRAME, FRAME, -FRAME, -FRAME);
	rCell.setWidth((rCell.width() - FRAME * 3) / 4);
	rCell.setHeight((rCell.height() - FRAME * 3) / 4);
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			cells[x][y] = new Cell(x, y, 0, this);
			cells[x][y]->setPos(rCell.left() + x * (rCell.width() + FRAME), \
					    rCell.top() + y * (rCell.height() + FRAME));
		}
	generate();
}

void Grid::reinit(void)
{
	ani->stop();
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			delete cells[x][y];
	init();
}

QRectF Grid::boundingRect(void) const
{
	return QRectF(0, 0, scene()->width(), scene()->height());
}

QPainterPath Grid::shape(void) const
{
	QPainterPath path;
	path.addRoundRect(boundingRect(), 3, 3);
	return path;
}

void Grid::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(Tile::foreground(0));
	painter->setBrush(Tile::foreground(0));
	painter->drawRoundRect(boundingRect(), 3, 3);

	QRectF rCell = boundingRect().adjusted(FRAME, FRAME, -FRAME, -FRAME);
	rCell.setWidth((rCell.width() - FRAME * 3) / 4);
	rCell.setHeight((rCell.height() - FRAME * 3) / 4);
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			painter->setPen(Tile::background(0));
			painter->setBrush(Tile::background(0));
			painter->drawRoundRect(rCell.left() + x * (rCell.width() + FRAME), \
					       rCell.top() + y * (rCell.height() + FRAME), \
					       rCell.width(), rCell.height(), 3, 3);
		}
}

bool Grid::move(const vect& dir)
{
	ani->stop();
	bool succeedBak = succeed;
	int scoreBak = score;
	bool moved = false;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			int xt = dir.x == 1 ? 3 - x : x;
			int yt = dir.y == 1 ? 3 - y : y;
			if (cells[xt][yt]->value() == 0)
				continue;
			Cell **cell = find(&cells[xt][yt], dir);
			if (&cells[xt][yt] != cell)
				moved = true;
			move(&cells[xt][yt], cell);
		}
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			cells[x][y]->setMerged(false);
	ani->start();
	if (score != scoreBak)
		emit scoreChanged(score);
	if (succeedBak == false && succeed == true) {
		if (QMessageBox::information((QWidget *)scene()->views().first(), \
					     "Succeed", "You got 2048 tile!\nContinue?", \
					     QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) {
			reinit();
			return false;
		}
	}
	return moved;
}

void Grid::move(Cell **from, Cell **to)
{
	if (*from == *to)
		return;
	QRectF rCell = boundingRect().adjusted(FRAME, FRAME, -FRAME, -FRAME);
	rCell.setWidth((rCell.width() - FRAME * 3) / 4);
	rCell.setHeight((rCell.height() - FRAME * 3) / 4);
	if ((*to)->value() != 0) {
		(*from)->setValue((*from)->value() + (*to)->value());
		(*from)->setMerged(true);
		ani->addRefresh(*from);
		score += (*to)->value();
		if ((*from)->value() == 2048)
			succeed = true;
	}
	int x = (*to)->x(), y = (*to)->y();
	ani->addDelete(*to);
	*to = *from;
	*from = new Cell((*from)->x(), (*from)->y(), 0, this);
	(*from)->setPos(rCell.left() + (*to)->x() * (rCell.width() + FRAME), \
		      rCell.top() + (*to)->y() * (rCell.height() + FRAME));
	(*to)->setX(x);
	(*to)->setY(y);

	ani->add(*to, QPointF(rCell.left() + x * (rCell.width() + FRAME), \
			      rCell.top() + y * (rCell.height() + FRAME)));
}

class Cell **Grid::find(Cell **orig, const vect dir)
{
	int x = (*orig)->x(), y = (*orig)->y();
	do {
		x += dir.x;
		y += dir.y;
	} while (x >= 0 && x <= 3 && y >= 0 && y <= 3 && cells[x][y]->value() == 0);
	if (x >= 0 && x <= 3 && y >= 0 && y <= 3 && \
			abs(cells[x][y]->value()) == abs((*orig)->value()) && \
			!cells[x][y]->merged())
		return &cells[x][y];
	x -= dir.x;
	y -= dir.y;
	return &cells[x][y];
}

bool Grid::generate(void)
{
	QList<Cell **> list;
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (cells[x][y]->value() != 0)
				continue;
			list.append(&cells[x][y]);
		}
	if (list.count() == 0)
		return false;
	Cell **cell = list.at(qrand() % list.count());
	(*cell)->setValue((qrand() % 100) < 90 ? 2 : 4);
	(*cell)->refresh();
	ani->addNew(*cell);
	return true;
}

bool Grid::failed(void)
{
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			if (cells[x][y]->value() == 0)
				return false;
			if (x != 0 && abs(cells[x][y]->value()) == abs(cells[x - 1][y]->value()))
				return false;
			if (x != 3 && abs(cells[x][y]->value()) == abs(cells[x + 1][y]->value()))
				return false;
			if (y != 0 && abs(cells[x][y]->value()) == abs(cells[x][y - 1]->value()))
				return false;
			if (y != 3 && abs(cells[x][y]->value()) == abs(cells[x][y + 1]->value()))
				return false;
		}
	return true;
}

void Grid::save(QString path)
{
	QFile f(path);
	if (!f.open(f.WriteOnly)) {
		QMessageBox::critical((QWidget *)scene()->views().first(), \
				      "Error", "Cannot open file for save!");
		return;
	}
	QTextStream fs(&f);
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++)
			fs << cells[x][y]->value() << " ";
	fs << score << " " << succeed;
	f.close();
}

void Grid::load(QString path)
{
	QFile f(path);
	if (!f.open(f.ReadOnly)) {
		QMessageBox::critical((QWidget *)scene()->views().first(), \
				      "Error", "Cannot open file for load!");
		return;
	}
	QTextStream fs(&f);
	for (int x = 0; x < 4; x++)
		for (int y = 0; y < 4; y++) {
			int value;
			fs >> value;
			cells[x][y]->setValue(value);
			cells[x][y]->refresh();
		}
	int s;
	fs >> score >> s;
	succeed = s;
	f.close();
	emit scoreChanged(score);
	if (failed()) {
		QMessageBox::critical((QWidget *)scene()->views().first(), \
				      "Game over", "The board is full, game over!");
		reinit();
	}
}

void Grid::op(const vect& dir)
{
	if (move(dir)) {
		generate();
		save("2048.dat");
		if (failed()) {
			QMessageBox::critical((QWidget *)scene()->views().first(), \
					      "Game over", "The board is full, game over!");
			reinit();
		}
	}
}

void Grid::keyPressEvent(QKeyEvent *event)
{
	vect dir = {0, 0};
	switch (event->key()) {
	case Qt::Key_W:
	case Qt::Key_K:
	case Qt::Key_Up:
		dir.y = -1;
		break;
	case Qt::Key_S:
	case Qt::Key_J:
	case Qt::Key_Down:
		dir.y = 1;
		break;
	case Qt::Key_A:
	case Qt::Key_H:
	case Qt::Key_Left:
		dir.x = -1;
		break;
	case Qt::Key_D:
	case Qt::Key_L:
	case Qt::Key_Right:
		dir.x = 1;
		break;
	case Qt::Key_P:
		dir.x = rand() % 3 - 1;
		if (dir.x == 0)
			dir.y = rand() % 3 - 1;
		break;
	case Qt::Key_R:
		reinit();
		return;
	case Qt::Key_Q:
		qApp->quit();
		return;
	default:
		return;
	}
	op(dir);
}

void Grid::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
	e->accept();
	mouse = e->pos();
}

void Grid::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
	e->accept();
	int x = e->pos().x() - mouse.x(), y = e->pos().y() - mouse.y();
	if (x == y)
		return;
	vect dir = {0, 0};
	if (abs(x) > abs(y))
		dir.x = x > 0 ? 1 : -1;
	else
		dir.y = y > 0 ? 1 : -1;
	op(dir);
}
