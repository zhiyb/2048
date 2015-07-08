#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainw.h"

MainW::MainW(QWidget *parent) :
	QMainWindow(parent)
{
	QWidget *w = new QWidget(this);
	QHBoxLayout *hlayout = new QHBoxLayout(w);
	QVBoxLayout *vlayout = new QVBoxLayout;

	scene = new QGraphicsScene(0, 0, 500, 500);
	grid = new Grid;
	scene->addItem(grid);
	grid->init();
	hlayout->addWidget(view = new View(scene));
	//view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	//view->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
	grid->setFocus();
	hlayout->addLayout(vlayout);

	vlayout->addWidget(lScore = new QLabel(tr("Score:\n0")));
	vlayout->addWidget(pbRestart = new QPushButton("&Restart"));
	vlayout->addWidget(pbQuit = new QPushButton("&Quit"));

	setCentralWidget(w);

	connect(grid, SIGNAL(scoreChanged(int)), this, SLOT(scoreUpdate(int)));
	connect(pbRestart, SIGNAL(clicked()), grid, SLOT(reinit()));
	connect(pbQuit, SIGNAL(clicked()), qApp, SLOT(quit()));

	grid->load("2048.dat");
}

void MainW::scoreUpdate(int score)
{
	lScore->setText(QString(tr("Score:\n%1")).arg(score));
}
